/*****************************************************************************
 *                                                                           *
 *                              C O R O U T I N E                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines Koroutinen-Konzepts.               *
 *                  Die Koroutinen sind miteinander verkettet, weswegen die  *
 *                  Klasse Coroutine ein Subtyp von 'Chain' ist.             *
 *                                                                           *
 *                  Im Konstruktor wird der initialie Kontext der Koroutine  *
 *                  eingerichtet. Mit 'start' wird ein Koroutine aktiviert.  *
 *                  Das Umschalten auf die naechste Koroutine erfolgt durch  *
 *                  Aufruf von 'switchToNext'.                               *
 *                                                                           *
 *                  Um bei einem Koroutinenwechsel den Kontext sichern zu    *
 *                  koennen, enthaelt jedes Koroutinenobjekt eine Struktur   *
 *                  CoroutineState, in dem die Werte der nicht-fluechtigen   *
 *                  Register gesichert werden koennen.                       *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 13.08.2020                     *
 *****************************************************************************/

#include "kernel/threads/Thread.h"

// Funktionen, die auf der Assembler-Ebene implementiert werden, muessen als
// extern "C" deklariert werden, da sie nicht dem Name-Mangeling von C++
// entsprechen.
extern "C" {
    void Thread_start(struct ThreadState* regs);
    void Thread_switch(struct ThreadState* regs_now, struct ThreadState* reg_then);
}

Logger Thread::log = Logger("Thread");
unsigned int ThreadCnt = 0;

/*****************************************************************************
 * Prozedur:        Coroutine_init                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Bereitet den Kontext der Koroutine fuer den ersten       *
 *                  Aufruf vor.                                              *
 *****************************************************************************/
void Thread_init(struct ThreadState* regs, unsigned int* stack, void (*kickoff)(Thread*), void* object) {

    register unsigned int** sp = (unsigned int**)stack;

    // Stack initialisieren. Es soll so aussehen, als waere soeben die
    // eine Funktion aufgerufen worden, die als Parameter den Zeiger
    // "object" erhalten hat.
    // Da der Funktionsaufruf simuliert wird, kann fuer die Ruecksprung-
    // adresse nur ein unsinniger Wert eingetragen werden. Die aufgerufene
    // Funktion muss daher dafuer sorgen, dass diese Adresse nie benoetigt
    // wird, sie darf also nicht terminieren, sonst kracht's.

    *(--sp) = (unsigned int*)object;    // Parameter
    *(--sp) = (unsigned int*)0x131155;  // Ruecksprungadresse (Dummy)

    // Nun legen wir noch die Adresse der Funktion "kickoff" ganz oben auf
    // den Stack. Wenn dann bei der ersten Aktivierung dieser Koroutine der
    // Stackpointer so initialisiert wird, dass er auf diesen Eintrag
    // verweist, genuegt ein ret, um die Funktion kickoff zu starten.
    // Genauso sollen auch alle spaeteren Threadwechsel ablaufen.

    *(--sp) = (unsigned int*)kickoff;  // Adresse

    // Initialisierung der Struktur ThreadState mit den Werten, die die
    // nicht-fluechtigen Register beim ersten Starten haben sollen.
    // Wichtig ist dabei nur der Stackpointer.

    regs->ebx = 0;
    regs->esi = 0;
    regs->edi = 0;
    regs->ebp = 0;
    regs->esp = sp;  // esp now points to the location of the address of kickoff

    // nachfolgend die fluechtige Register
    // wichtig fuer preemptives Multitasking
    regs->eax = 0;
    regs->ecx = 0;
    regs->edx = 0;

    // flags initialisieren
    regs->efl = (void*)0x200;  // Interrupt-Enable
}

/*****************************************************************************
 * Funktion:        kickoff                                                  *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Funktion zum Starten einer Korutine. Da diese Funktion   *
 *                  nicht wirklich aufgerufen, sondern nur durch eine        *
 *                  geschickte Initialisierung des Stacks der Koroutine      *
 *                  angesprungen wird, darf er nie terminieren. Anderenfalls *
 *                  wuerde ein sinnloser Wert als Ruecksprungadresse         * 
 *                  interpretiert werden und der Rechner abstuerzen.         *
 *****************************************************************************/
void kickoff(Thread* object) {
    object->run();

    // object->run() kehrt hoffentlich nie hierher zurueck
    for (;;) {}
}

/*****************************************************************************
 * Methode:         Coroutine::Coroutine                                     *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Initialer Kontext einer Koroutine einrichten.            *
 *                                                                           *
 * Parameter:                                                                *
 *      stack       Stack für die neue Koroutine                             *
 *****************************************************************************/
Thread::Thread() : stack(new unsigned int[1024]), tid(ThreadCnt++) {
    Thread::log << INFO << "Initialized thread with ID: " << this->tid << endl;
    Thread_init(&regs, stack + 1024, kickoff, this);  // Stack grows from top to bottom
}

/*****************************************************************************
 * Methode:         Coroutine::switchToNext                                  *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Auf die nächste Koroutine umschalten.                    *
 *****************************************************************************/
void Thread::switchTo(Thread& next) {

    /* hier muss Code eingefügt werden */

    Thread_switch(&this->regs, &next.regs);
}

/*****************************************************************************
 * Methode:         Coroutine::start                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Aktivierung der Koroutine.                               *
*****************************************************************************/
void Thread::start() {

    /* hier muss Code eingefügt werden */

    Thread_start(&this->regs);
}
