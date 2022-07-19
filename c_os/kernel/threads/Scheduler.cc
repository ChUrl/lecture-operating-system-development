/*****************************************************************************
 *                                                                           *
 *                          S C H E D U L E R                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines einfachen Zeitscheiben-Schedulers. *
 *                  Rechenbereite Threads werden in 'readQueue' verwaltet.   *
 *                                                                           *
 *                  Der Scheduler wird mit 'schedule' gestartet. Neue Threads*
 *                  können mit 'ready' hinzugefügt werden. Ein Thread muss   *
 *                  die CPU freiwillig mit 'yield' abgeben, damit andere auch*
 *                  rechnen koennen. Ein Thread kann sich selbst mit 'exit'  *
 *                  terminieren. Ein Thread kann einen anderen Thread mit    *
 *                  'kill' beenden. Ein erzwungener Threadwechsel erfolgt    *
 *                  mit der Funktion 'preempt', welche von der Timer-ISR     *
 *                  aufgerufen wird.                                         *
 *                                                                           *
 *                  Zusaetzlich gibt es nun fuer die Semaphore zwei neue     *
 *                  Funktionen 'block' und 'deblock'.                        *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 23.11.2018                     *
 *****************************************************************************/

#include "kernel/threads/Scheduler.h"
#include "kernel/threads/IdleThread.h"
#include <optional>

/*****************************************************************************
 * Methode:         Dispatcher::start                                        *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread starten.                                          *
 *                                                                           *
 * Parameter:                                                                *
 *      first       Zu startender Thread.                                    *
 *****************************************************************************/
void Scheduler::start(Thread& first) {
    if (active == nullptr) {
        active = &first;
        active->start();
    }
}

/*****************************************************************************
 * Methode:         Dispatcher::dispatch                                     *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Auf einen gegebenen Thread wechseln.                     *
 *                                                                           *
 * Parameter:                                                                *
 *      next        Thread der die CPU erhalten soll.                        *
 *****************************************************************************/
void Scheduler::dispatch(Thread& next) {
    Thread* current = active;
    active = &next;
    current->switchTo(next);
}

/*****************************************************************************
 * Methode:         Scheduler::schedule                                      *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Scheduler starten. Wird nur einmalig aus main.cc gerufen.*
 *****************************************************************************/
void Scheduler::schedule() {

    /* hier muss Code eingefuegt werden */

    // We need to start the idle thread first as this one sets the scheduler to initialized
    // Other wise preemption will be blocked and nothing will happen if the first threads
    // ready() function is blocking
    this->start(*(Thread*)new IdleThread());  // The idle thread set initialized to true, so preemption
                                              // only starts after this
}

/*****************************************************************************
 * Methode:         Scheduler::ready                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread in readyQueue eintragen.                          *
 *                                                                           *
 * Parameter:                                                                *
 *      that        Einzutragender Thread                                    *
 *****************************************************************************/
void Scheduler::ready(Thread* that) {

    /* hier muss Code eingefuegt werden */

    // Thread-Wechsel durch PIT verhindern
    cpu.disable_int();
    this->ready_queue.insert_last(that);

    log << DEBUG << "Adding to ready_queue, ID: " << dec << that->tid << endl;
    cpu.enable_int();
}

/*****************************************************************************
 * Methode:         Scheduler::exit                                          *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread ist fertig und terminiert sich selbst. Hier muss  *
 *                  nur auf den naechsten Thread mithilfe des Dispatchers    *
 *                  umgeschaltet werden. Der aktuell laufende Thread ist     *
 *                  nicht in der readyQueue.                                 *
 *****************************************************************************/
void Scheduler::exit() {

    /* hier muss Code eingefuegt werden */

    // Thread-Wechsel durch PIT verhindern
    cpu.disable_int();
    if (this->ready_queue.empty()) {
        log << ERROR << "Can't exit last thread, active ID: " << dec << this->active->tid << endl;
        cpu.enable_int();
        return;
    }

    Thread* next = this->ready_queue.remove_first().value_or(nullptr);
    if (next == nullptr) {
        log << ERROR << "(Exit) Couldn't remove thread from ready_queue" << endl;
        cpu.enable_int();
        return;
    }

    log << DEBUG << "Exiting thread, ID: " << dec << this->active->tid << " => " << next->tid << endl;
    delete this->active;

    this->dispatch(*next);

    // Interrupts werden in Thread_switch in Thread.asm wieder zugelassen
    // dispatch kehr nicht zurueck
}

/*****************************************************************************
 * Methode:         Scheduler::kill                                          *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread mit 'Gewalt' terminieren. Er wird aus der         *
 *                  readyQueue ausgetragen und wird dann nicht mehr aufge-   *
 *                  rufen. Der Aufrufer dieser Methode muss ein anderer      *
 *                  Thread sein.                                             *
 *                                                                           *
 * Parameter:                                                                *
 *      that        Zu terminierender Thread                                 *
 *****************************************************************************/
void Scheduler::kill(Thread* that) {

    /* hier muss Code eingefuegt werden */

    // Thread-Wechsel durch PIT verhindern
    cpu.disable_int();
    if (!this->ready_queue.remove(that)) {
        // Not in ready queue
        if (!this->block_queue.remove(that)) {
            // Not in block queue
            log << ERROR << "Can't kill thread that is not in any queue, ID: " << dec << that->tid << endl;
            cpu.enable_int();
            return;
        }
    }

    log << DEBUG << "Killing thread, ID: " << dec << that->tid << endl;
    delete that;
    cpu.enable_int();
}

void Scheduler::kill(unsigned int id) {
    cpu.disable_int();
    Thread* to_remove = NULL;
    for (Thread* thread : this->ready_queue) {
        if (thread->tid == id) {
            this->ready_queue.remove(to_remove);
            log << DEBUG << "Killing thread, ID: " << dec << id << endl;
            delete to_remove;

            cpu.enable_int();
            return;
        }
    }

    for (Thread* thread : this->block_queue) {
        if (thread->tid == id) {
            this->block_queue.remove(to_remove);
            log << DEBUG << "Killing thread, ID: " << dec << id << endl;
            delete to_remove;

            cpu.enable_int();
            return;
        }
    }

    log << ERROR << "Can't kill thread that is not in any queue, ID: " << dec << id << endl;
    cpu.enable_int();
}

/*****************************************************************************
 * Methode:         Scheduler::yield                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    CPU freiwillig abgeben und Auswahl des naechsten Threads.*
 *                  Naechsten Thread aus der readyQueue holen, den aktuellen *
 *                  in die readyQueue wieder eintragen. Das Umschalten soll  *
 *                  mithilfe des Dispatchers erfolgen.                       *
 *                                                                           *
 *                  Achtung: Falls nur der Idle-Thread läuft, so ist die     *
 *                           readyQueue leer.                                *
 *****************************************************************************/
void Scheduler::yield() {

    /* hier muss Code eingefuegt werden */

    // Thread-Wechsel durch PIT verhindern
    cpu.disable_int();
    if (this->ready_queue.empty()) {
        // log << TRACE << "Skipping yield as no thread is waiting, active ID: " << dec << this->active->tid << endl;
        cpu.enable_int();
        return;
    }

    Thread* next = this->ready_queue.remove_first().value_or(nullptr);
    if (next == nullptr) {
        log << ERROR << "(Yield) Couldn't remove thread from ready_queue" << endl;
        cpu.enable_int();
        return;
    }

    this->ready_queue.insert_last(this->active);

    // log << TRACE << "Yielding, ID: " << dec << this->active->tid << " => " << next.tid << endl;

    this->dispatch(*next);
}

/*****************************************************************************
 * Methode:         Scheduler::preempt                                       *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Diese Funktion wird aus der ISR des PITs aufgerufen und  *
 *                  schaltet auf den naechsten Thread um, sofern einer vor-  *
 *                  handen ist.                                              *
 *****************************************************************************/
void Scheduler::preempt() {

    /* Hier muss Code eingefuegt werden */

    cpu.disable_int();
    this->yield();
}

/*****************************************************************************
 * Methode:         Scheduler::block                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Aufrufer ist blockiert. Es soll auf den naechsten Thread *
 *                  umgeschaltet werden. Der Aufrufer soll nicht in die      *
 *                  readyQueue eingefuegt werden und wird extern verwaltet.  *
 *                  Wird bei uns nur fuer Semaphore verwendet. Jede Semaphore*
 *                  hat eine Warteschlange wo der Thread dann verwaltet wird.*
 *                  Die Methode kehrt nicht zurueck, sondern schaltet um.    *
 *****************************************************************************/
void Scheduler::block() {

    /* hier muss Code eingefuegt werden */

    // Basically the same as exit()

    cpu.disable_int();
    if (this->ready_queue.empty()) {
        log << ERROR << "Can't block last thread, active ID: " << dec << this->active->tid << endl;
        cpu.enable_int();
        return;
    }

    this->block_queue.insert_last(this->active);  // Thread that will be blocked waits in block_queue, so the scheduler can also
                                                  // kill blocked threads (for example keyboard demo needs this)

    Thread* next = this->ready_queue.remove_first().value_or(nullptr);
    if (next == nullptr) {
        log << ERROR << "(Block) Couldn't remove thread from ready_queue" << endl;
        cpu.enable_int();
        return;
    }

    log << TRACE << "Blocking thread, ID: " << dec << this->active->tid << " => " << next->tid << endl;

    this->dispatch(*next);
}

/*****************************************************************************
 * Methode:         Scheduler::deblock                                       *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Thread 'that' deblockieren. 'that' wird nur in die       *
 *                  readyQueue eingefuegt und dann zurueckgekehrt. In der    *
 *                  einfachsten Form entspricht diese Funktion exakt 'ready' *
 *                  Man koennte alternativ aber den deblockierten Thread auch*
 *                  am Anfang der readyQueue einfuegen, um ihn zu beorzugen. *
 *                                                                           *
 * Parameter:       that:  Thread der deblockiert werden soll.               *
 *****************************************************************************/
void Scheduler::deblock(Thread* that) {

    /* hier muss Code eingefuegt werden */

    // Basically the same as ready()

    cpu.disable_int();
    if (!this->block_queue.remove(that)) {
        log << ERROR << "Unblocked thread wasn't in block_queue" << endl;
    }
    this->ready_queue.insert_at(that, 0);  // Prefer deblocked

    log << TRACE << "Adding to start of ready_queue, ID: " << dec << that->tid << endl;
    cpu.enable_int();
}
