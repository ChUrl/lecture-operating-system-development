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
    print((char*)buffer, pos, attribute(this->color_bg, this->color_fg, this->blink));

    // TODO: Should not be reset like this
    // Flushing resets attributes
    this->blink = false;
    this->color_bg = CGA::BLACK;
    this->color_fg = CGA::LIGHT_GREY;

    pos = 0;
}

// Alternative way to write the templates which keeps definition/declaration separated
// Usable for our case but somehow defeats the purpose of templates
// template<typename T>
// T& operator<<(T& os, const fgc& fg) {
//     os.color_fg = fg.fg;
//     return os;
// }

// template CGA_Stream& operator<<<CGA_Stream>(CGA_Stream&, const fgc&);
