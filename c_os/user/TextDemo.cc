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

    // TODO:
    // kout << fixedwidth(5) << "Test" << endl;

    kout << "Test der Zahlenausgabefunktion:" << endl << endl;
    kout << "| dec | hex  | bin   |" << endl;
    kout << "+-----+------+-------+" << endl;

    for (unsigned char num = 0; num < 17; ++num) {
        kout << "|     |      |       |" << endl;
    }
}
