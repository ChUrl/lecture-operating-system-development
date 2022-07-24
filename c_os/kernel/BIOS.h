/*****************************************************************************
 *                                                                           *
 *                                B I O S                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Zugriff auf das 16-Bit BIOS. Fuer VESA-Funktionen.       *
 *                                                                           *
 * Autor:           Michael Schoettner, 13.9.2016                            *
 *****************************************************************************/
#ifndef BIOS_include__
#define BIOS_include__

// Speicherseite fuer Rueckgabewerte von BIOS-Aufrufen
constexpr const unsigned int RETURN_MEM = 0x9F000;

// Struktur fuer Parameteruebergabe fuer einen BIOS-Aufruf
struct BIOScall_params {
    unsigned short DS;
    unsigned short ES;
    unsigned short FS;
    unsigned short Flags;
    unsigned int DI;
    unsigned int SI;
    unsigned int BP;
    unsigned int SP;
    unsigned int BX;
    unsigned int DX;
    unsigned int CX;
    unsigned int AX;
} __attribute__((packed));
// kein Auffuellen von bytes auf Wortgrenzen

// Zeiger auf Speichbereich fuer Parameter fuer BIOS-Aufruf
extern  BIOScall_params* BC_params;

class BIOS {
private:
    // Initialisierung: manuelles Anlegen einer Funktion
    BIOS();

public:
    BIOS(const BIOS& copy) = delete;  // Verhindere Kopieren

    static BIOS& instance() {
        static BIOS bios;
        return bios;
    }

    // BIOS-Aufruf, per Software-Interrupt
    static void Int(int inter);
};

#endif
