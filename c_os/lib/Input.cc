/*****************************************************************************
 *                                                                           *
 *                                I N P U T                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Hilfsfunktion zum Warten bis auf der Tastatur die Ein-   * 
 *                  -gabetaste gedrückt wird.                                * 
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 2.05.2022                       *
 *****************************************************************************/
#include "devices/Keyboard.h"
#include "kernel/Globals.h"

void waitForReturn() {
    // Warten bis <ENTER> gedrueckt wird
    kb.lastkey = '\0';  // lastKey loeschen
    while (kb.lastkey != 10) {};
}

char getch() {
    kb.lastkey = '\0';  // lastKey loeschen
    while (kb.lastkey == '\0') {};
    return kb.lastkey;
}
