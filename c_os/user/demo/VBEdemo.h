/*****************************************************************************
 *                                                                           *
 *                             V B E D E M O                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Demo zu VESA.                                            *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 26.12.2016                      *
 *****************************************************************************/
#ifndef VBEdemo_include__
#define VBEdemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class VBEdemo : public Thread {
private:
    // Hilfsfunktionen fuer drawColors()
    static int linInterPol1D(int x, int xr, int l, int r);
    static int linInterPol2D(int x, int y, int lt, int rt, int lb, int rb);

public:
    VBEdemo(const VBEdemo& copy) = delete;  // Verhindere Kopieren

    // Gib dem Anwendungsthread einen Stack.
    VBEdemo() : Thread("VBEdemo") {}

    ~VBEdemo() override {
        allocator.free(reinterpret_cast<void*>(vesa.hfb));  // Memory is allocated after every start and never deleted, so add that
        VESA::initTextMode();
    }

    // Thread-Startmethode
    void run() override;

    // Farbraum ausgeben
    void drawColors();

    // Bitmap aus GIMP ausgeben
    static void drawBitmap();

    // Fonts ausgeben
    static void drawFonts();
};

#endif
