/*****************************************************************************
 *                                                                           *
 *                        K E Y B O A R D D E M O                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Testausgaben für den CGA-Treiber.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 26.10.2018                      *
 *****************************************************************************/

#ifndef KeyboardDemo_include__
#define KeyboardDemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "user/event/KeyEventListener.h"

class KeyboardDemo : public Thread {
private:
    KeyEventListener listener;

public:
    KeyboardDemo(const KeyboardDemo& copy) = delete;

    KeyboardDemo() : Thread("KeyboardDemo"), listener(tid) {
        kevman.subscribe(listener);
    }

    // Base class destructor will be called automatically
    ~KeyboardDemo() override {
        if (running) {
            // NOTE: If the thread was exited nicely it can unlock before destructor,
            //       but on forced kill kout has to be unlocked in the destructor.
            //       This is bad since it could release the lock although some other
            //       thread set it (so use nice_kill)
            kout.unlock();
        }
        kevman.unsubscribe(listener);
    }

    void run() override;
};

#endif
