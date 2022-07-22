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

#include "devices/CGA.h"
#include "lib/OutStream.h"
#include "lib/Semaphore.h"

// NOTE: I added these classes to allow for easier stream-like color changing
class fgc {
public:
    constexpr fgc(const CGA::color fg) : fg(fg) {}
    const CGA::color fg;
};

// NOTE: I could have used a struct to remove the public, but I like to use
//       structs exclusively for C like memory "descriptions", although they're
//       techinally the same to classes
class bgc {
public:
    constexpr bgc(const CGA::color bg) : bg(bg) {}  // Make this into a literal type to allow macro-like usage
    const CGA::color bg;
};

constexpr fgc white = fgc(CGA::WHITE);
constexpr fgc black = fgc(CGA::BLACK);
constexpr fgc green = fgc(CGA::GREEN);
constexpr fgc red = fgc(CGA::RED);
constexpr fgc lgrey = fgc(CGA::LIGHT_GREY);

class CGA_Stream : public OutStream, public CGA {
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

    // Change stream color
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
