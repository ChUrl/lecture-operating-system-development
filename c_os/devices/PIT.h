/*****************************************************************************
 *                                                                           *
 *                                   P I T                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Programmable Interval Timer.                             * 
 *                                                                           *
 * Autor:           Michael Schoettner, 23.8.2016                            *
 *****************************************************************************/

#ifndef __PIT_include__
#define __PIT_include__

#include "kernel/interrupts/ISR.h"
#include "kernel/IOport.h"

class PIT : public ISR {
private:
    const static IOport control;
    const static IOport data0;

    enum { time_base = 838 }; /* ns */
    int timer_interval;

    const char indicator[4] = {'|', '/', '-', '\\'};
    unsigned int indicator_pos = 0;
    unsigned long last_indicator_refresh = 0;

public:
    PIT(const PIT& copy) = delete;  // Verhindere Kopieren

    // Zeitgeber initialisieren.
    explicit PIT(int us) {
        PIT::interval(us);
    }

    // Konfiguriertes Zeitintervall auslesen.
    int interval() const { return timer_interval; }

    // Zeitintervall in Mikrosekunden, nachdem periodisch ein Interrupt
    //erzeugt werden soll.
    static void interval(int us);

    // Aktivierung der Unterbrechungen fuer den Zeitgeber
    void plugin();

    // Unterbrechnungsroutine des Zeitgebers.
    void trigger() override;
};

#endif
