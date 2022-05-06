/*****************************************************************************
 *                                                                           *
 *                        K E Y B O A R D D E M O                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Testausgaben für den CGA-Treiber.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 26.10.2018                      *
 *****************************************************************************/

#include "kernel/Globals.h"

void keyboard_demo() {

    /* Hier muess Code eingefuegt werden */

    kout << "Keyboard Demo: " << endl;

    while (true) {
        kout << kb.key_hit();
        kout.flush();
    }
}
