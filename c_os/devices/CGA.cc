/*****************************************************************************
 *                                                                           *
 *                                    C G A                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Mit Hilfe dieser Klasse kann man auf den Bildschirm des  *
 *                  PCs zugreifen. Der Zugriff erfolgt direkt auf der Hard-  *
 *                  wareebene, d.h. ueber den Bildschirmspeicher und den     *
 *                  I/O-Ports der Grafikkarte.                               *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *                  Aenderungen von Michael Schoettner, HHU, 21.8.2016       *
 *****************************************************************************/
#include "devices/CGA.h"


/*****************************************************************************
 * Methode:         CGA::setpos                                              *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Setzen des Cursors in Spalte x und Zeile y.              *
 *****************************************************************************/
void CGA::setpos (int x, int y) {

    /* Hier muess Code eingefuegt werden */

    // NOTE: The cursor addresses positions on screen, not bytes
    unsigned short pos = x + y * 80;
    unsigned char cursor_low = pos & 0xFF;
    unsigned char cursor_high = (pos >> 8) & 0xFF;

    index_port.outb(0xF); // Cursor(low)
    data_port.outb(cursor_low);

    index_port.outb(0xE); // Cursor(high)
    data_port.outb(cursor_high);

}


/*****************************************************************************
 * Methode:         CGA::getpos                                              *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Abfragem der Cursorposition                              *
 *                                                                           *
 * Rückgabewerte:   x und y                                                  *
 *****************************************************************************/
void CGA::getpos (int &x, int &y) {

    /* Hier muess Code eingefuegt werden */

    index_port.outb(0xF); // Cursor(low)
    unsigned char cursor_low = data_port.inb();

    index_port.outb(0xE); // Cursor(high)
    unsigned char cursor_high = data_port.inb();

    unsigned short cursor =
        (cursor_low & 0xFF)
        | ((cursor_high << 8) & 0xFF00);

    x = cursor % 80;
    y = (int)(cursor / 80);

}


/*****************************************************************************
 * Methode:         CGA::show                                                *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Anzeige eines Zeichens mit Attribut an einer bestimmten  *
 *                  Stelle auf dem Bildschirm.                               *
 *                                                                           *
 * Parameter:                                                                *
 *      x,y         Position des Zeichens                                    *
 *      character   Das auszugebende Zeichen                                 *
 *      attrib      Attributbyte fuer das Zeichen                            *
 *****************************************************************************/
void CGA::show (int x, int y, char character, unsigned char attrib) {

    /* Hier muess Code eingefuegt werden */

    char* pos = (char*)(CGA_START + 2 * (x + y * 80)); // cast to char* to make writable
    *pos = character;
    *(pos + 1) = attrib;

    // TODO: screen border check

}


/*****************************************************************************
 * Methode:         CGA::print                                               *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Anzeige mehrerer Zeichen ab der aktuellen Cursorposition *
 *                  '\n' fuer Zeilenvorschub.                                *
 *                                                                           *
 * Parameter:                                                                *
 *      string      Auszugebende Zeichenkette                                *
 *      n           Laenger der Zeichenkette                                 *
 *      attrib      Attributbyte fuer alle Zeichen der Zeichenkette          *
 *****************************************************************************/
void CGA::print (char* string, int n, unsigned char attrib) {

    /* Hier muess Code eingefuegt werden */

    int cursor_x, cursor_y;
    this->getpos(cursor_x, cursor_y);

    for (unsigned short byte = 0; byte < n; ++byte) {

        char current = *(string + byte);
        if (current == '\n') {
            // TODO: Screen bounds
            cursor_x = 0;
            cursor_y = cursor_y + 1;
            continue;
        } else if (current == '\0') {
            break;
        }

        this->show(cursor_x, cursor_y, current, attrib);
        cursor_x = cursor_x + 1;
    }

    this->setpos(cursor_x, cursor_y);

    // TODO: automatic line breaking, automatic scrolling
    // TODO: printing doesn't work after first newline character

}


/*****************************************************************************
 * Methode:         CGA::scrollup                                            *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Verschiebt den Bildschirminhalt um eine Zeile nach oben. *
 *                  Die neue Zeile am unteren Bildrand wird mit Leerzeichen  *
 *                  gefuellt.                                                *
 *****************************************************************************/
void CGA::scrollup () {

    /* Hier muess Code eingefuegt werden */

    // Move up
    for (unsigned short byte = 2 * 80 * 1; byte < 2 * 80 * 25; ++byte) {
        *((char*)(CGA_START + byte - 2 * 80 * 1)) = *(CGA_START + byte);
    }

    // Clear last line
    for (unsigned short byte = 2 * 80 * 24; byte < 2 * 80 * 25; ++byte) {
        *((char*)(CGA_START + byte)) = '\0';
    }

}


/*****************************************************************************
 * Methode:         CGA::clear                                               *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Lösche den Textbildschirm.                               *
 *****************************************************************************/
void CGA::clear () {

    /* Hier muess Code eingefuegt werden */
    for (unsigned short byte = 2 * 80 * 0; byte < 2 * 80 * 25; ++byte) {
        *((char*)(CGA_START + byte)) = '\0';
    }

}


/*****************************************************************************
 * Methode:         CGA::attribute                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Hilfsfunktion zur Erzeugung eines Attribut-Bytes aus     *
 *                  Hintergrund- und Vordergrundfarbe und der Angabe, ob das *
 *                  Zeichen blinkend darzustellen ist.                       *
 *                                                                           *
 * Parameter:                                                                *
 *      bg          Background color                                         *
 *      fg          Foreground color                                         *
 *      blink       ywa/no                                                   *
 *****************************************************************************/
unsigned char CGA::attribute (CGA::color bg, CGA::color fg, bool blink) {

    /* Hier muess Code eingefuegt werden */
    
}
