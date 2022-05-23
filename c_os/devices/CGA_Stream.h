/*****************************************************************************
 *                                                                           *
 *                     C G A _ S T R E A M                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Die Klasse CGA_Stream ermoeglicht die Ausgabe verschied. *
 *                  Datentypen als Zeichenketten auf dem CGA-Bildschirm eines*
 *                  PCs. Fuer weitergehende Formatierung oder spezielle      *
 *                  Effekte stehen die Methoden der Klasse CGA zur           *
 *                  Verfuegung.                                              *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *                  Aenderungen von Michael Schoettner, HHU, 06.04.20        *
 *****************************************************************************/
#ifndef __CGA_Stream_include__
#define __CGA_Stream_include__

#include "devices/BufferedCGA.h"
#include "lib/OutStream.h"

// NOTE: I added this
class fgc {
public:
    fgc() : fg(CGA::LIGHT_GREY) {}
    fgc(CGA::color fg) : fg(fg) {}
    CGA::color fg;
};
class bgc {
public:
    bgc() : bg(CGA::BLACK) {}
    bgc(CGA::color bg) : bg(bg) {}
    CGA::color bg;
};

// NOTE: I added this (changed this) to use BufferedCGA
class CGA_Stream : public OutStream, public BufferedCGA {
private:
    CGA_Stream(CGA_Stream& copy) = delete;  // Verhindere Kopieren

public:
    CGA::color color_fg;
    CGA::color color_bg;
    bool blink;

    CGA_Stream() : color_fg(CGA::LIGHT_GREY), color_bg(CGA::BLACK), blink(false) {
        flush();
    }

    // Methode zur Ausgabe des Pufferinhalts der Basisklasse StringBuffer.
    void flush() override;

    // NOTE: I added this
    template<typename T>
    // requires std::derived_from<T, CGA_Stream>
    friend T& operator<<(T& os, const fgc& fg) {
        os.flush();
        os.color_fg = fg.fg;
        return os;
    }

    template<typename T>
    // requires std::derived_from<T, CGA_Stream>
    friend T& operator<<(T& os, const bgc& bg) {
        os.flush();
        os.color_fg = bg.bg;
        return os;
    }
};

#endif
