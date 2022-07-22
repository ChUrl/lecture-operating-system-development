/*****************************************************************************
 *                                                                           *
 *                        K E Y B O A R D D E M O                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Testausgaben für den CGA-Treiber.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 26.10.2018                      *
 *****************************************************************************/

#include "user/demo/KeyboardDemo.h"

void KeyboardDemo::run() {

    /* Hier muess Code eingefuegt werden */

    kout << "Keyboard Demo: " << endl;

    // kout.lock();
    kout.clear();

    while (true) {
        kout << listener.waitForKeyEvent();
        kout.flush();
    }

    // kout.unlock();
    scheduler.exit();
}
