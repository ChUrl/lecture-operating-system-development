/*****************************************************************************
 *                                                                           *
 *                          S C H E D U L E R                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines einfachen Zeitscheiben-Schedulers. *
 *                  Rechenbereite Threads werden in 'readQueue' verwaltet.   *
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

    Thread* idle = new IdleThread();
    // this->readyQueue.enqueue(idle);
    this->ready(idle);

    /* Bevor diese Methode anufgerufen wird, muss zumindest der Idle-Thread
     * in der Queue eingefuegt worden sein. 
     */

    Thread& first = *(Thread*)this->readyQueue.dequeue();
    this->start(first);
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
    cpu.disable_int ();

    this->readyQueue.enqueue(that);

    // Thread-Wechsel durch PIT jetzt wieder erlauben
    cpu.enable_int ();
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
    cpu.disable_int ();

    Thread& next = *(Thread*)this->readyQueue.dequeue();
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
    cpu.disable_int ();

    this->readyQueue.remove(that);

    // Thread-Wechsel durch PIT jetzt wieder erlauben
    cpu.enable_int ();
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
    cpu.disable_int ();

    // When only one thread exists (IdleThread) it can't yield as the readyqueue becomes empty
    // and this is not handled anywhere else
    if (this->readyQueue.isEmpty()) {
        // Idle thread running
        return;
    }

    Thread& next = *(Thread*)this->readyQueue.dequeue();
    this->readyQueue.enqueue(this->get_active());
    this->dispatch(next);

    // Thread-Wechsel durch PIT jetzt wieder erlauben
    cpu.enable_int ();
}

/*****************************************************************************
 * Methode:         Scheduler::preempt                                       *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Diese Funktion wird aus der ISR des PITs aufgerufen und  *
 *                  schaltet auf den naechsten Thread um, sofern einer vor-  *
 *                  handen ist.                                              *
 *****************************************************************************/
void Scheduler::preempt () {

   /* Hier muss Code eingefuegt werden */

}
