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
#include "lib/Input.h"

void KeyboardDemo::run() {

    /* Hier muess Code eingefuegt werden */

    kout << "Keyboard Demo: " << endl;

    while (true) {
        kout << listener.waitForKeyEvent();

        // kout << getch();
        kout.flush();
    }
}
