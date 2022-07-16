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
    this->ready_queue.insert(that);

    log << TRACE << "Adding to ready_queue, ID: " << dec << that->tid << endl;
    this->ready_queue.print(log << TRACE);
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
        log << ERROR << "Can't exit last thread, active ID: " << dec << this->get_active()->tid << endl;
        cpu.enable_int();
        return;
    }
    Thread& next = *(Thread*)this->ready_queue.remove_first();

    log << TRACE << "Exiting thread, ID: " << dec << this->get_active()->tid << " => " << next.tid << endl;
    this->ready_queue.print(log << TRACE);

    this->dispatch(next);

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
    if (this->ready_queue.remove(that) == -1) {
        log << ERROR << "Can't kill thread that is not in ready_queue, ID: " << dec << that->tid << endl;
        cpu.enable_int();
        return;
    }

    log << TRACE << "Killing thread, ID: " << dec << that->tid << endl;
    this->ready_queue.print(log << TRACE);
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
        log << TRACE << "Skipping yield as no thread is waiting, active ID: " << dec << this->get_active()->tid << endl;
        cpu.enable_int();
        return;
    }

    Thread& next = *(Thread*)this->ready_queue.remove_first();
    this->ready_queue.insert(this->get_active());

    log << TRACE << "Yielding, ID: " << dec << this->get_active()->tid << " => " << next.tid << endl;
    this->ready_queue.print(log << TRACE);

    this->dispatch(next);
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

    this->yield();  // We already have that
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

    this->exit();  // If I'm not mistaken block/deblock is the same functionality we already have
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

    this->ready(that);
}
