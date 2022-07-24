/*****************************************************************************
 *                                                                           *
 *                       S T R I N G B U F F E R                             *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Die Klasse StringBuffer stellt einen Puffer fuer die     *
 *                  Sammlung von Zeichen zur Darstellung auf dem Bildschirm  * 
 *                  oder anderen Ausgabegeraeten bereit. Die Ausgabe der     *
 *                  Zeichen erfolgt, sobald der Puffer voll ist oder wenn    *
 *                  explizit die Methode flush() aufgerufen wird.            *
 *                  Da StringBuffer geraeteunabhaengig sein soll, ist        *
 *                  flush() eine virtuelle Methode, die von den abgeleiteten *
 *                  Klassen definiert werden muss.                           *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *                  Aenderungen von Michael Schoettner, HHU, 06.04.20        *
 *****************************************************************************/
#ifndef StringBuffer_include__
#define StringBuffer_include__

#include "user/lib/Array.h"

class StringBuffer {
    // Alle Variablen und Methoden dieser Klasse sind "protected",
    // da die abgeleiteten Klassen einen direkten Zugriff auf den
    // Puffer, den Konstruktor, den Destruktor und die Methode put
    // benoetigen. Die Methode flush() muss sowieso neu definiert
    // werden und kann dann auch public werden.

protected:
    bse::array<char, 80> buffer;
    int pos;

    // StringBuffer: Im Konstruktor wird der Puffer als leer markiert.
    StringBuffer() : pos(0) {}

    // Fuegt ein Zeichen in den Puffer ein. Wenn der Puffer
    virtual void put(char c);

    // Methode zur Ausgabe des Pufferinhalts
    virtual void flush() = 0;
public:
    StringBuffer(const StringBuffer& copy) = delete;  // Verhindere Kopieren

//    virtual ~StringBuffer() = default;
};

#endif
