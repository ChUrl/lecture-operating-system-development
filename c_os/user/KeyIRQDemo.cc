/*****************************************************************************
 *                                                                           *
 *                         K E Y I R Q D E M O                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Demo zu Interrupts.                                      *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 26.10.2018                      *
 *****************************************************************************/

#include "kernel/Globals.h"

void key_irq_demo() {

    /* Hier muss Code eingefuegt werden */

    // TODO: I don't understand the task

    while (true) {
        for (unsigned char i = 0; i < 10; ++i) {
            kout.show(kout.COLUMNS - 1, i, (char)(48 + i));
        }
    }
}
