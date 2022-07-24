/*****************************************************************************
 *                                                                           *
 *                             I D L E T H R E A D                           *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Wird nur aktiviert, wenn kein Thread arbeiten moechte.   *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 13.8.2016                      *
 *****************************************************************************/

#ifndef IdleThread_include__
#define IdleThread_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class IdleThread : public Thread {
public:
    IdleThread(const Thread& copy) = delete;  // Verhindere Kopieren

    IdleThread() : Thread("IdleThread") {}

    [[noreturn]] void run() override {
        // Idle-Thread läuft, ab jetzt ist der Scheduler fertig initialisiert
        log.info() << "IdleThread enabled preemption" << endl;
        scheduler.enable_preemption(tid);
        if (!scheduler.preemption_enabled()) {
            log.error() << "Preemption disabled" << endl;
        }

        while (true) {
            // kout << "Idle!" << endl;
            scheduler.yield();
        }
    }
};

#endif
