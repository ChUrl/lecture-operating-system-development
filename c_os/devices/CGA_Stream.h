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
#ifndef CGA_Stream_include_H_
#define CGA_Stream_include_H_

#include "devices/CGA.h"
#include "lib/OutStream.h"
#include "lib/Semaphore.h"

// Allow for easier stream-like color changing
class fgc {
public:
    constexpr fgc(const CGA::color fg) : fg(fg) {}
    const CGA::color fg;
};

class bgc {
public:
    constexpr bgc(const CGA::color bg) : bg(bg) {}
    const CGA::color bg;
};

constexpr const fgc white = fgc(CGA::WHITE);
constexpr const fgc black = fgc(CGA::BLACK);
constexpr const fgc green = fgc(CGA::GREEN);
constexpr const fgc red = fgc(CGA::RED);
constexpr const fgc lgrey = fgc(CGA::LIGHT_GREY);

class CGA_Stream : public OutStream, public CGA {
private:
    // Allow for synchronization of output text, needed when running something in parallel to
    // the PreemptiveThreadDemo for example
    // NOTE: Should only be used by threads (like the demos) to not lock the system
    Semaphore sem;

    CGA::color color_fg;
    CGA::color color_bg;
    bool blink;

    friend class Logger;  // Give access to the color

public:
    CGA_Stream(CGA_Stream& copy) = delete;  // Verhindere Kopieren

    CGA_Stream() : sem(1), color_fg(CGA::LIGHT_GREY), color_bg(CGA::BLACK), blink(false) {
        pos = 0;
    }

    // CAn't make singleton because atexit

//    ~CGA_Stream() override = default;

    void lock() { sem.p(); }
    void unlock() { sem.v(); }

    // Methode zur Ausgabe des Pufferinhalts der Basisklasse StringBuffer.
    void flush() override;

    // Change stream color
    template<typename T>
    friend T& operator<<(T& os, const fgc& fg) {
        CGA::color old_bg = os.color_bg;
        os.flush();
        os.color_bg = old_bg;
        os.color_fg = fg.fg;
        return os;
    }

    template<typename T>
    friend T& operator<<(T& os, const bgc& bg) {
        CGA::color old_fg = os.color_fg;
        os.flush();
        os.color_fg = old_fg;
        os.color_bg = bg.bg;
        return os;
    }
};

#endif
