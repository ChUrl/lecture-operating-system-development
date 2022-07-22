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
    IdleThread() : Thread("IdleThread") {}

    void run() override {
        // Idle-Thread läuft, ab jetzt ist der Scheduler fertig initialisiert
        log << INFO << "IdleThread enabled preemption" << endl;
        scheduler.enable_preemption(this->tid);
        if (!scheduler.preemption_enabled()) {
            log << ERROR << "Preemption disabled" << endl;
        }

        while (true) {
            // kout << "Idle!" << endl;
            scheduler.yield();
        }
    }
};

#endif
