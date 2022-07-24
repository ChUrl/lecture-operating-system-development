/*****************************************************************************
 *                                                                           *
 *                                P C S P K                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Mit Hilfe dieser Klasse kann man Toene auf dem           *
 *                  PC-Lautsprecher ausgeben.                                *
 *                                                                           *
 * Achtung:         Qemu muss mit dem Parameter -soundhw pcspk aufgerufen    *
 *                  werden. Ansonsten kann man nichts hoeren.                *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 22.9.2016                       *
 *****************************************************************************/

#ifndef PCSPK_include__
#define PCSPK_include__

#include "kernel/IOport.h"

// Note, Frequenz
constexpr const float C0 = 130.81;
constexpr const float C0X = 138.59;
constexpr const float D0 = 146.83;
constexpr const float D0X = 155.56;
constexpr const float E0 = 164.81;
constexpr const float F0 = 174.61;
constexpr const float F0X = 185.00;
constexpr const float G0 = 196.00;
constexpr const float G0X = 207.65;
constexpr const float A0 = 220.00;
constexpr const float A0X = 233.08;
constexpr const float B0 = 246.94;

constexpr const float C1 = 261.63;
constexpr const float C1X = 277.18;
constexpr const float D1 = 293.66;
constexpr const float D1X = 311.13;
constexpr const float E1 = 329.63;
constexpr const float F1 = 349.23;
constexpr const float F1X = 369.99;
constexpr const float G1 = 391.00;
constexpr const float G1X = 415.30;
constexpr const float A1 = 440.00;
constexpr const float A1X = 466.16;
constexpr const float B1 = 493.88;

constexpr const float C2 = 523.25;
constexpr const float C2X = 554.37;
constexpr const float D2 = 587.33;
constexpr const float D2X = 622.25;
constexpr const float E2 = 659.26;
constexpr const float F2 = 698.46;
constexpr const float F2X = 739.99;
constexpr const float G2 = 783.99;
constexpr const float G2X = 830.61;
constexpr const float A2 = 880.00;
constexpr const float A2X = 923.33;
constexpr const float B2 = 987.77;
constexpr const float C3 = 1046.50;

class PCSPK {
private:
    static const IOport control;  // Steuerregister (write only)
    static const IOport data2;    // Zaehler-2 Datenregister
    static const IOport ppi;      // Status-Register des PPI

    // Verzoegerung um X ms (in 1ms Schritten; Min. 1ms)
    static inline void delay(int time);

public:
    PCSPK(const PCSPK& copy) = delete;  // Verhindere Kopieren

    // Konstruktor. Initialisieren der Ports.
    PCSPK() = default;

    // Can't make singleton because atexit

    // Demo Sounds
    static void tetris();
    static void aerodynamic();

    // Ton abspielen
    static void play(float f, int len);

    // Lautsprecher ausschalten
    static void off();
};

#endif
