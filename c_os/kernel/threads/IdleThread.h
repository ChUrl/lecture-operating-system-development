/*****************************************************************************
 *                                                                           *
 *                             I D L E T H R E A D                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Wird nur aktiviert, wenn kein Thread arbeiten moechte.   *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 13.8.2016                      *
 *****************************************************************************/

#ifndef __IdleThread_include__
#define __IdleThread_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class IdleThread : public Thread {
private:
    IdleThread(const Thread& copy) = delete;  // Verhindere Kopieren

public:
    IdleThread() {}

    void run() override {
        while (true) {
            scheduler.yield();
            // kout << "Idle!" << endl;
        }
    }
};

#endif
