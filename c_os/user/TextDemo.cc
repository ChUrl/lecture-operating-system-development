/*****************************************************************************
 *                                                                           *
 *                           T E X T D E M O                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Testausgaben für den CGA-Treiber.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 26.10.2018                       *
 *****************************************************************************/

#include "kernel/Globals.h"

void text_demo() {

    /* Hier muess Code eingefuegt werden */
    kout << "Test der Zahlenausgabefunktion:" << endl;
    kout << endl;
    kout << "|  dec  |  hex  |  bin  |" << endl;
    kout << "+-------+-------+-------+" << endl;

    // TODO: should fillw just work for the next << ?
    for (unsigned short num = 0; num < 17; ++num) {
        kout << fillw(0) << "| " << fillw(6) << dec << num;
        kout << fillw(0) << "| " << fillw(6) << hex << num;
        kout << fillw(0) << "| " << fillw(6) << bin << num;
        kout << fillw(0) << "|" << endl;
    }
}
