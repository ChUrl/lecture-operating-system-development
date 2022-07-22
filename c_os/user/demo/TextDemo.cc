/*****************************************************************************
 *                                                                           *
 *                           T E X T D E M O                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Testausgaben für den CGA-Treiber.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 26.10.2018                       *
 *****************************************************************************/

#include "user/demo/TextDemo.h"

void TextDemo::run() {

    /* Hier muess Code eingefuegt werden */

    kout.lock();
    kout.clear();
    kout << "TextDemo\n"
         << endl;

    kout << "Attribut (GREEN on WHITE): "
         << bgc(CGA::WHITE) << green << "GREEN on WHITE" << endl
         << "Attribut (WHITE on BLACK): "
         << bgc(CGA::BLACK) << white << "WHITE on BLACK" << endl;
    kout << endl;

    kout << "Test der Zahlenausgabefunktion:" << endl
         << "|  dec  |  hex  |  bin  |" << endl
         << "+-------+-------+-------+" << endl;

    for (unsigned short num = 0; num < 17; ++num) {
        kout << fillw(0) << "| " << fillw(6) << dec << num
             << fillw(0) << "| " << fillw(6) << hex << num
             << fillw(0) << "| " << fillw(6) << bin << num
             << fillw(0) << "|" << endl;
    }

    kout << endl;

    kout.unlock();
    scheduler.exit();
}
