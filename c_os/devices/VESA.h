/*****************************************************************************
 *                                                                           *
 *                                 V E S A                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    VESA-Treiber ueber 16-Bit BIOS.                          *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 19.5.2022                       *
 *****************************************************************************/

#ifndef VESA_include__
#define VESA_include__

#include "devices/LFBgraphics.h"
#include "user/lib/utility/Logger.h"

// Ausgewaehlte Grafikmodi mit Mode-Nummer
constexpr const unsigned int MODE_640_480_16BITS = 0x111;
constexpr const unsigned int MODE_640_480_24BITS = 0x112;
constexpr const unsigned int MODE_800_600_16BITS = 0x114;
constexpr const unsigned int MODE_800_600_24BITS = 0x115;
constexpr const unsigned int MODE_1024_768_16BITS = 0x117;
constexpr const unsigned int MODE_1024_768_24BITS = 0x118;

class VESA : public LFBgraphics {
private:
    int mode_nr;  // Nummer des Modus
    NamedLogger log;

public:
    VESA(const VESA& copy) = delete;  // Verhindere Kopieren

    VESA() : log("VESA") {}

    // Can't make singleton because atexit

    // Bestimmten Grafikmodus einschalten
    bool initGraphicMode(unsigned short mode);
    static void initTextMode();
};

#endif
