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
    void Thread_start(unsigned int esp);

    // NOTE: Only when backing up the previous thread the esp get's updated,
    //       so only esp_pre is a pointer
    void Thread_switch(unsigned int* esp_prev, unsigned int esp_next);
}

unsigned int ThreadCnt = 1;  // Skip tid 0 as the scheduler indicates no preemption with 0

/*****************************************************************************
 * Prozedur:        Coroutine_init                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Bereitet den Kontext der Koroutine fuer den ersten       *
 *                  Aufruf vor.                                              *
 *****************************************************************************/
void Thread_init(unsigned int* esp, unsigned int* stack, void (*kickoff)(Thread*), void* object) {

    // NOTE: c++17 doesn't allow register
    // register unsigned int** sp = (unsigned int**)stack;
    // unsigned int** sp = (unsigned int**)stack;

    // Stack initialisieren. Es soll so aussehen, als waere soeben die
    // eine Funktion aufgerufen worden, die als Parameter den Zeiger
    // "object" erhalten hat.
    // Da der Funktionsaufruf simuliert wird, kann fuer die Ruecksprung-
    // adresse nur ein unsinniger Wert eingetragen werden. Die aufgerufene
    // Funktion muss daher dafuer sorgen, dass diese Adresse nie benoetigt
    // wird, sie darf also nicht terminieren, sonst kracht's.

    // I thought this syntax was a bit clearer than decrementing a pointer
    stack[-1] = (unsigned int)object;
    stack[-2] = 0x131155U;
    stack[-3] = (unsigned int)kickoff;
    stack[-4] = 0;                         // EAX
    stack[-5] = 0;                         // ECX
    stack[-6] = 0;                         // EDX
    stack[-7] = 0;                         // EBX
    stack[-8] = (unsigned int)&stack[-3];  // ESP
    stack[-9] = 0;                         // EBP
    stack[-10] = 0;                        // ESI
    stack[-11] = 0;                        // EDI
    stack[-12] = 0x200U;

    *esp = (unsigned int)&stack[-12];
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
[[noreturn]] void kickoff(Thread* object) {
    object->run();

    // object->run() kehrt (hoffentlich) nie hierher zurueck
    while (true) {}
}

/*****************************************************************************
 * Methode:         Coroutine::Coroutine                                     *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Initialer Kontext einer Koroutine einrichten.            *
 *                                                                           *
 * Parameter:                                                                *
 *      stack       Stack für die neue Koroutine                             *
 *****************************************************************************/
Thread::Thread(char* name) : stack(new unsigned int[1024]), esp(0), log(name), name(name), tid(ThreadCnt++) {
    log.info() << "Initialized thread with ID: " << this->tid << " (" << name << ")" << endl;
    Thread_init(&esp, &stack[1024], kickoff, this);  // Stack grows from top to bottom
}

/*****************************************************************************
 * Methode:         Coroutine::switchToNext                                  *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Auf die nächste Koroutine umschalten.                    *
 *****************************************************************************/
void Thread::switchTo(Thread& next) {

    /* hier muss Code eingefügt werden */

    // log.trace() << name << ":: Has esp " << hex << esp << endl;
    Thread_switch(&esp, next.esp);
}

/*****************************************************************************
 * Methode:         Coroutine::start                                         *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Aktivierung der Koroutine.                               *
*****************************************************************************/
void Thread::start() const {

    /* hier muss Code eingefügt werden */

    Thread_start(this->esp);
}
