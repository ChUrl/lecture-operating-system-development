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
#include <utility>

void Scheduler::lock() {
    cpu.disable_int();
}

void Scheduler::unlock() {
    cpu.enable_int();
}

/*****************************************************************************
 * Methode:         Dispatcher::dispatch                                     *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Auf den active thread wechseln.                          *
 *                                                                           *
 * Parameter:                                                                *
 *      next        Thread der die CPU erhalten soll.                        *
 *****************************************************************************/
void Scheduler::dispatch(Thread* prev_raw) {
    prev_raw->switchTo(**active);  // First dereference the Iterator, then the unique_ptr to get Thread
}

/*****************************************************************************
 * Methode:         Scheduler::schedule                                      *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Scheduler starten. Wird nur einmalig aus main.cc gerufen.*
 *****************************************************************************/
void Scheduler::schedule() {

    /* hier muss Code eingefuegt werden */

    // We need to start the idle thread first as this one sets the scheduler to initialized
    // and enables preemption.
    // Otherwise preemption will be blocked and nothing will happen if the first threads
    // run() function is blocking

    ready_queue.push_back(std::move(bse::make_unique<IdleThread>()));
    active = ready_queue.end() - 1;
    (*active)->start();
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
    lock();
    if (ready_queue.size() == 1) {
        log << ERROR << "Can't exit last thread, active ID: " << dec << (*active)->tid << endl;
        unlock();
        return;
    }

    Thread* prev_raw = (*active).get();
    log << DEBUG << "Exiting thread, ID: " << dec << (*active)->tid << endl;

    active = ready_queue.erase(active);
    if (active == ready_queue.end()) {
        active = ready_queue.begin();
    }

    dispatch(prev_raw);

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
void Scheduler::kill(unsigned int tid) {
    unsigned int (*pred)(const bse::unique_ptr<Thread>&) =
      [](const bse::unique_ptr<Thread>& ptr) -> unsigned int { return ptr->tid; };

    lock();
    Thread* prev_raw = (*active).get();
    std::size_t erased_els = bse::erase_if(ready_queue, pred, tid);
    erased_els += bse::erase_if(block_queue, pred, tid);

    if (erased_els == 0) {
        log << ERROR << "Couldn't find thread with id: " << tid << " in ready- or block-queue" << endl;
        unlock();
        return;
    }

    if (erased_els > 1) {
        log << ERROR << "Killed more than 1 thread (oops)" << endl;
        unlock();
        return;
    }

    // Active thread could have been killed
    if (active >= ready_queue.end()) {
        active = ready_queue.begin();
    }
    dispatch(prev_raw);

    unlock();
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
    lock();
    if (ready_queue.size() == 1) {
        // log << TRACE << "Skipping yield as no thread is waiting, active ID: " << dec << active->tid << endl;
        unlock();
        return;
    }

    // log << TRACE << "Yielding, ID: " << dec << active->tid << " => " << next.tid << endl;

    Thread* prev_raw = (*active).get();
    ++active;
    if (active == ready_queue.end()) {
        active = ready_queue.begin();
    }
    dispatch(prev_raw);
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

    lock();
    yield();
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

    lock();
    if (ready_queue.size() == 1) {
        log << ERROR << "Can't block last thread, active ID: " << dec << (*active)->tid << endl;
        unlock();
        return;
    }

    Thread* prev_raw = (*active).get();
    std::size_t active_idx = bse::distance(ready_queue.begin(), active);
    block_queue.push_back(std::move(ready_queue[active_idx]));

    active = ready_queue.erase(active);
    if (active == ready_queue.end()) {
        active = ready_queue.begin();
    }

    dispatch(prev_raw);
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
void Scheduler::deblock(unsigned int tid) {

    /* hier muss Code eingefuegt werden */

    lock();

    bse::unique_ptr<Thread> thread;
    for (bse::Vector<bse::unique_ptr<Thread>>::Iterator it = block_queue.begin(); it != block_queue.end(); ++it) {
        if ((*it)->tid == tid) {
            // Found thread with correct tid
            std::size_t pos = bse::distance(block_queue.begin(), it);
            thread = std::move(block_queue[pos]);
            block_queue.erase(it);
            break;
        }
    }

    if (!thread) {
        log << ERROR << "Couldn't deblock thread with id: " << tid << endl;
        unlock();
        return;
    }

    ready_queue.push_back(std::move(thread));

    unlock();
}
