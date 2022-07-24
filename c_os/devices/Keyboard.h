/*****************************************************************************
 *                                                                           *
 *                                K E Y B O A R D                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Treiber für den Tastaturcontroller des PCs.              *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *                  Modifikationen, Michael Schoettner, 2.6.2022             *
 *****************************************************************************/
#ifndef Keyboard_include__
#define Keyboard_include__

#include "devices/Key.h"
#include "kernel/interrupts/ISR.h"
#include "kernel/IOport.h"

class Keyboard : public ISR {
private:
    unsigned char code;    // Byte von Tastatur
    unsigned char prefix;  // Prefix von Tastatur
    Key gather;            // letzter dekodierter Key
    char leds;             // Zustand LEDs

    // Benutzte Ports des Tastaturcontrollers
    static const IOport ctrl_port;  // Status- (R) u. Steuerregister (W)
    static const IOport data_port;  // Ausgabe- (R) u. Eingabepuffer (W)

    // Bits im Statusregister
    enum { outb = 0x01,
           inpb = 0x02,
           auxb = 0x20 };

    // Kommandos an die Tastatur
    struct kbd_cmd {
        enum { set_led = 0xed,
               set_speed = 0xf3 };
    };
    enum { cpu_reset = 0xfe };

    // Namen der LEDs
    struct led {
        enum { caps_lock = 4,
               num_lock = 2,
               scroll_lock = 1 };
    };

    // Antworten der Tastatur
    struct kbd_reply {
        enum { ack = 0xfa };
    };

    // Konstanten fuer die Tastaturdekodierung
    enum { break_bit = 0x80,
           prefix1 = 0xe0,
           prefix2 = 0xe1 };

    // Klassenvariablen
    static const unsigned char normal_tab[];
    static const unsigned char shift_tab[];
    static const unsigned char alt_tab[];
    static const unsigned char asc_num_tab[];
    static const unsigned char scan_num_tab[];

    // Interpretiert die Make und Break-Codes der Tastatur.
    bool key_decoded();

    // Ermittelt anhand von Tabellen den ASCII-Code.
    void get_ascii_code();

    // Tastaturabfrage (vorerst Polling)
    Key key_hit();

public:
    Keyboard(const Keyboard& copy) = delete;  // Verhindere Kopieren

    // Initialisierung der Tastatur.
    Keyboard();

//    ~Keyboard() override = default;

    // unsigned int lastkey;  // speichert den ASCII-Code der zuletzt gedrückten Taste

    // Fuehrt einen Neustart des Rechners durch.
    static void reboot();

    // Einstellen der Wiederholungsrate der Tastatur.
    void set_repeat_rate(int speed, int delay);

    // Setzt oder loescht die angegebene Leuchtdiode.
    void set_led(char led, bool on);

    // Aktivierung der Unterbrechungen fuer die Tastatur
    void plugin();

    // Unterbrechnungsroutine der Tastatur.
    void trigger() override;
};

#endif
