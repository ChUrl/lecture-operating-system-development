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
    kout << "Attribut (GREEN on WHITE): "
         << bgc(CGA::WHITE) << fgc(CGA::GREEN) << "GREEN on WHITE" << endl
         << "Attribut (WHITE on BLACK): "
         << bgc(CGA::BLACK) << fgc(CGA::WHITE) << "WHITE on BLACK" << endl
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
