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
#include "user/KeyEventListener.h"

class KeyboardDemo : public Thread {
private:
    KeyboardDemo(const KeyboardDemo& copy) = delete;

    KeyEventListener listener;

public:
    KeyboardDemo() : listener(*this) {
        kout << "Initialized KeyboardDemo" << endl;
        kevman.subscribe(listener);
    }

    void run() override;
};

#endif
