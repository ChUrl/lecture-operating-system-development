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
    kout << "Attribut (GREEN on WHITE, blinking): "
         << (unsigned short)kout.attribute(CGA::WHITE, CGA::GREEN, true) << endl
         << "Attribut (WHITE on BLACK, no blink): "
         << (unsigned short)kout.attribute(CGA::BLACK, CGA::WHITE, false) << endl
         << endl;

    kout << "Test der Zahlenausgabefunktion:" << endl
         << endl
         << "|  dec  |  hex  |  bin  |" << endl
         << "+-------+-------+-------+" << endl;

    for (unsigned short num = 0; num < 17; ++num) {
        kout << fillw(0) << "| " << fillw(6) << dec << num
             << fillw(0) << "| " << fillw(6) << hex << num
             << fillw(0) << "| " << fillw(6) << bin << num
             << fillw(0) << "|" << endl;
    }

    kout << endl;
}
