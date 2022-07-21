/*****************************************************************************
 *                                                                           *
 *                        K E Y B O A R D D E M O                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Testausgaben für den CGA-Treiber.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 26.10.2018                      *
 *****************************************************************************/

#ifndef __KeyboardDemo_include__
#define __KeyboardDemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "user/event/KeyEventListener.h"

class KeyboardDemo : public Thread {
private:
    KeyboardDemo(const KeyboardDemo& copy) = delete;

    KeyEventListener listener;

public:
    KeyboardDemo() : listener(this->tid) {
        log << INFO << "Initialized KeyboardDemo with ID: " << dec << this->tid << endl;
        kevman.subscribe(this->listener);
    }

    // Base class destructor will be called automatically
    ~KeyboardDemo() override {
        log << INFO << "Uninitialized KeyboardDemo" << endl;
        kevman.unsubscribe(this->listener);
        kout.unlock();
    }

    void run() override;
};

#endif
