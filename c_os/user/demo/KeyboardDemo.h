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

class KeyboardDemo : public Thread {
private:
    KeyboardDemo(const KeyboardDemo& copy) = delete;

public:
    KeyboardDemo() {
        kout << "Initialized KeyboardDemo" << endl;
    }

    void run() override;
};

#endif
