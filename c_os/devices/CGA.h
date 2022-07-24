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
#ifndef CGA_include_H_
#define CGA_include_H_

#include "kernel/IOport.h"
#include "user/lib/Array.h"
#include "user/lib/Span.h"
#include "user/lib/String.h"

class CGA {
private:
    static const IOport index_port;  // Auswahl eines Register der Grafikkarte
    static const IOport data_port;   // Lese-/Schreib-Zugriff auf Register der Grafikk.

public:
    // Copy Konstrutkor unterbinden
    CGA(const CGA& copy) = delete;

    // Konstruktur mit Initialisierung der Ports
    CGA() {
        CGA::setpos(0, 0);
    }

//    virtual ~CGA() = default;

    // Konstanten fuer die moeglichen Farben im Attribut-Byte.
    typedef enum {
        BLACK,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHT_GREY,
        DARK_GREY,
        LIGHT_BLUE,
        LIGHT_GREEN,
        LIGHT_CYAN,
        LIGHT_RED,
        LIGHT_MAGENTA,
        YELLOW,
        WHITE
    } color;

    // Standardzeichenfarbe
    enum { STD_ATTR = BLACK << 4 | LIGHT_GREY };

    // Groesse des Bildschirms (25 Zeilen, 80 Spalten)
    enum { ROWS = 25,
           COLUMNS = 80 };

    // Easier access to memory (also easier copying of lines/pages etc)
    struct cga_char_t {
        char cga_char;
        unsigned char cga_attribute;
    };

    struct cga_line_t {
        // Can use these arrays since they don't have memory overhead (except for the methods that are elsewhere)
        bse::array<cga_char_t, COLUMNS> cga_line;
    };

    struct cga_page_t {
        bse::array<cga_line_t, ROWS> cga_page;
    };

    static bse::span<cga_char_t, ROWS * COLUMNS> SCREEN;
    static bse::span<cga_line_t, ROWS> SCREEN_ROWS;
    static cga_page_t* SCREEN_PAGE;  // No span because can't address anything in [0, 1]

    // Setzen des Cursors in Spalte x und Zeile y.
    static void setpos(unsigned int x, unsigned int y);

    // Abfragen der Cursorpostion
    static void getpos(unsigned int& x, unsigned int& y) ;

    // Anzeige eines Zeichens mit Attribut an einer bestimmten Stelle
    static void show(unsigned int x, unsigned int y, char character, unsigned char attrib = STD_ATTR);

    // Anzeige mehrerer Zeichen ab der aktuellen Cursorposition
    void print(const char* string, unsigned int n, unsigned char attrib = STD_ATTR) const;
    void print(const bse::string& string, unsigned int n, unsigned char attrib = STD_ATTR) const;
    void print(const bse::string& string, unsigned char attrib = STD_ATTR) const;

    // Verschiebt den Bildschirminhalt um eine Zeile nach oben.
    // Neue Zeile am unteren Bildrand mit Leerzeichen fuellen
    virtual void scrollup() const;

    // Lösche den Textbildschirm
    virtual void clear();

    // Hilfsfunktion zur Erzeugung eines Attribut-Bytes
    static unsigned char attribute(CGA::color bg, CGA::color fg, bool blink);
};

#endif
