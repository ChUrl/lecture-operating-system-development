/*****************************************************************************
 *                                                                           *
 *                             S E M A P H O R E                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung von Sempahor-Objekten.                   *
 *                                                                           *
 * Autor:           Michael Schoettner, 2.9.2016                             *
 *****************************************************************************/

#ifndef __Semaphore_include__
#define __Semaphore_include__

#include "kernel/threads/Thread.h"
#include "lib/SpinLock.h"
#include "user/lib/Vector.h"

class Semaphore {
private:
    Semaphore(const Semaphore& copy) = delete;  // Verhindere Kopieren

    // Queue fuer wartende Threads.
    bse::vector<unsigned int> wait_queue;
    SpinLock lock;

    int counter;

public:
    // Konstruktor: Initialisieren des Semaphorzaehlers
    Semaphore(int c) : wait_queue(true), counter(c) {}

    // 'Passieren': Warten auf das Freiwerden eines kritischen Abschnitts.
    void p();

    // 'Vreigeben': Freigeben des kritischen Abschnitts.
    void v();
};

#endif
