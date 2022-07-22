/*****************************************************************************
 *                                                                           *
 *                                 V E S A                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    VESA-Treiber ueber 16-Bit BIOS.                          *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 19.5.2022                       *
 *****************************************************************************/

#ifndef __VESA_include__
#define __VESA_include__

#include "devices/LFBgraphics.h"
#include "user/lib/Logger.h"

// Ausgewaehlte Grafikmodi mit Mode-Nummer
#define MODE_640_480_16BITS 0x111
#define MODE_640_480_24BITS 0x112
#define MODE_800_600_16BITS 0x114
#define MODE_800_600_24BITS 0x115
#define MODE_1024_768_16BITS 0x117
#define MODE_1024_768_24BITS 0x118

class VESA : public LFBgraphics {
private:
    int mode_nr;  // Nummer des Modus
    NamedLogger log;

    VESA(const VESA& copy) = delete;  // Verhindere Kopieren

public:
    VESA() : log("VESA") {}

    // Bestimmten Grafikmodus einschalten
    bool initGraphicMode(unsigned short mode);
    void initTextMode();
};

#endif
