/*****************************************************************************
 *                                                                           *
 *                            L F B G R A P H I C S                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Zeichenfunktionen fuer Grafikmodi, die auf einem         *
 *                  linearen Framebuffer basieren. Verwendet in VESA und     *
 *                  QemuVGA.                                                 *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 19.9.2016                       *
 *                  Der Code fuer das Zeichnen der Linie ist von Alan Wolfe  *
 *         https://blog.demofox.org/2015/01/17/bresenhams-drawing-algorithms *
 *****************************************************************************/

#ifndef LFBgraphics_include__
#define LFBgraphics_include__

#include "devices/fonts/Fonts.h"

// Hilfsfunktionen um Farbwerte fuer einen Pixel zu erzeugen
constexpr unsigned int RGB_24(unsigned int r, unsigned int g, unsigned int b) {
    return ((r << 16) + (g << 8) + b);
}

constexpr const bool BUFFER_INVISIBLE = false;
constexpr const bool BUFFER_VISIBLE  = true;

class LFBgraphics {
private:
    // Hilfsfunktion fuer drawString
    void drawMonoBitmap(unsigned int x, unsigned int y,
                        unsigned int width, unsigned int height,
                        const unsigned char* bitmap, unsigned int col) const;

public:
    LFBgraphics(const LFBgraphics& copy) = delete;  // Verhindere Kopieren

    LFBgraphics() : mode(BUFFER_VISIBLE) {};

    unsigned int xres, yres;  // Aufloesung in Pixel
    unsigned int bpp;         // Farbtiefe (Bits per Pixel)
    unsigned int lfb;         // Adresse des Linearen Framebuffers
    unsigned int hfb;         // Adresse des versteckten Buffers (optional, fuer Animationen)
    unsigned int mode;        // Zeichnen im sichtbaren = 1 oder unsichtbaren = 0 Puffer

    void clear() const;
    void drawPixel(unsigned int x, unsigned int y, unsigned int col) const;

    void drawString(const Font& fnt, unsigned int x, unsigned int y, unsigned int col, const char* str, unsigned int len) const;

    void drawCircle(unsigned int x, unsigned int y, unsigned int rad, unsigned int col) const;
    void drawStraightLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int col) const;
    void drawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int col) const;

    void drawSprite(unsigned int width, unsigned int height, unsigned int bytes_pp, const unsigned char* pixel_data) const;

    // stellt ein, ob in den sichtbaren Puffer gezeichnet wird
    void setDrawingBuff(int v);

    // kopiert 'hfb' nach 'lfb'
    void copyHiddenToVisible() const;
};

#endif
