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
    buffer[pos] = '\0';  // I removed the n argument from print so nullterminate the string
    print(buffer.data(), attribute(color_bg, color_fg, blink));  // print(buffer...) would work syntactically
                                                                // but the system wouldn't start, as the bse::array
                                                                // would be implicitly converted to bse::string and
                                                                // that is dynamically allocated.
                                                                // print(buffer.data()...) just uses the stack location of
                                                                // the internal buffer of bse::array

    // Flushing resets attributes
    blink = false;
    color_bg = CGA::BLACK;
    color_fg = CGA::LIGHT_GREY;

    pos = 0;
}
