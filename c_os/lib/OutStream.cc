/*****************************************************************************
 *                                                                           *
 *                           O U T S T R E A M                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Die Klasse OutStream enthaelt die Definition des         *
 *                  << Operators fuer die wichtigsten der vordefinierten     *
 *                  Datentypen und realisiert somit die bekannte Ausgabe-    *
 *                  funktion der C++ iO_Stream Bibliothek. Zur Zeit wird     *
 *                  die Darstellung von Zeichen, Zeichenketten und ganzen    *
 *                  Zahlen unterstuetzt. Ein weiterer << Operator erlaubt    *
 *                  die Verwendung von Manipulatoren.                        *
 *                                                                           *
 *                  Neben der Klasse OutStream sind hier auch die            *
 *                  Manipulatoren hex, dec, oct und bin fuer die Wahl der    *
 *                  Basis bei der Zahlendarstellung, sowie endl fuer den     *
 *                  Zeilenumbruch definiert.                                 *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *                  Aenderungen von Michael Schoettner, HHU, 1.8.16          *
 *****************************************************************************/

#include "lib/OutStream.h"

void OutStream::put(char c) {
    if (fill_width == 0) {
        StringBuffer::put(c);
    } else if (fill_used == fill_width - 1) {
        // This indicates that content has been cut
        StringBuffer::put('@');
        fill_use_char();
    } else if (fill_used == fill_width) {
        // do nothing
    } else {
        StringBuffer::put(c);
        fill_use_char();
    }
}

void OutStream::fill_finalize() {
    if (fill_width == 0 || fill_used == fill_width) {
        // do nothing
    } else if (fill_used > fill_width) {
        // should never happen
    } else {
        for (unsigned char i = 0; i < fill_width - fill_used; ++i) {
            StringBuffer::put(fill_char);
        }
    }

    fill_used = 0;
}

void OutStream::fill_use_char() {
    if (fill_width == 0) {
        return;
    }
    fill_used++;
}

void OutStream::flush() {
    // Flushing resets fixed width
    base = 10;
    fill_char = ' ';
    fill_width = 0;
    fill_used = 0;
}

//
// Zeichen und Zeichenketten in Stream ausgeben
//
// NOTE: The implementations now reside in the OutStream.h as I switched them to templated functions

//
// Manipulatorfunktionen
//
// Die folgenden Funktionen erhalten und liefern jeweils eine Referenz auf
// ein OutStream Objekt. Da die Klasse O_Stream einen Operator << fuer
// derartige Funktionen definiert, koennen sie mit Hilfe dieses Operators
// aufgerufen und sogar in weitere Eingaben eingebettet werden.
// Aufgabe der Manipulatoren ist, die Darstellung der nachfolgenden Ausgaben
// zu beeinflussen, z.B durch die Wahl des Zahlensystems.

// NOTE: The implementations now resides in the OutStream.h as I switched them to templated functions
