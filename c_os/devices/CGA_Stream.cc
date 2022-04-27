/*****************************************************************************
 *                                                                           *
 *                     C G A _ S T R E A M                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Die Klasse CGA_Stream ermoeglicht die Ausgabe verschied. *
 *                  Datentypen als Zeichenketten auf dem CGA-Bildschirm eines*
 *                  PCs. Fuer weitergehende Formatierung oder spezielle      *
 *                  Effekte stehen die Methoden der Klasse CGA_Stream zur    *
 *                  Verfuegung.                                              *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *                  Aenderungen von Michael Schoettner, HHU, 1.8.16          *
 *****************************************************************************/

#include "devices/CGA_Stream.h"

/*****************************************************************************
 * Methode:         CGA_Stream::flush                                        *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Methode zur Ausgabe des Pufferinhalts der Basisklasse    *
 *                  StringBuffer. Die Methode wird implizit aufgerufen,      *
 *                  sobald der Puffer voll ist, kann aber auch explizit      *
 *                  verwendet werden, um eine Ausgabe zu erzwingen.          *
 *****************************************************************************/
void CGA_Stream::flush() {
    print(buffer, pos, attribute(this->color_bg, this->color_fg, this->blink));
    pos = 0;
}

// NOTE: I added this
// TODO
// CGA_Stream& CGA_Stream::operator<<(const fgc& fg) {
//     this->color_fg = fg.fg;
//     return *this;
// }
// CGA_Stream& CGA_Stream::operator<<(const bgc& bg) {
//     this->color_bg = bg.bg;
//     return *this;
// }
