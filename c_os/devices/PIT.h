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

class PIT : public ISR {
private:
    PIT(const PIT& copy) = delete;  // Verhindere Kopieren

    enum { time_base = 838 }; /* ns */
    int timer_interval;

    char indicator[4] = {'|', '/', '-', '\\'};
    unsigned int indicator_pos = 0;
    unsigned long last_indicator_refresh = 0;

public:
    // Zeitgeber initialisieren.
    PIT(int us) {
        this->interval(us);
    }

    // Konfiguriertes Zeitintervall auslesen.
    int interval() const {
        return timer_interval;
    }

    // Zeitintervall in Mikrosekunden, nachdem periodisch ein Interrupt
    //erzeugt werden soll.
    void interval(int us);

    // Aktivierung der Unterbrechungen fuer den Zeitgeber
    void plugin();

    // Unterbrechnungsroutine des Zeitgebers.
    void trigger() override;
};

#endif
