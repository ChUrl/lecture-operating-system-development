/*****************************************************************************
 *                                                                           *
 *                                  P I C                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Mit Hilfe des PICs koennen Hardware-Interrupts (IRQs)    *
 *                  einzeln zugelassen oder unterdrueckt werden. Auf diese   *
 *                  Weise wird also bestimmt, ob die Unterbrechung eines     *
 *                  Geraetes ueberhaupt an den Prozessor weitergegeben wird. *
 *                  Selbst dann erfolgt eine Aktivierung der Unterbrechungs- *
 *                  routine nur, wenn der Prozessor bereit ist, auf Unter-   *
 *                  brechungen zu reagieren. Dies kann mit Hilfe der Klasse  *
 *                  CPU festgelegt werden.                                   *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *****************************************************************************/
#ifndef PIC_include__
#define PIC_include__

#include "kernel/IOport.h"

class PIC {
private:
    static const IOport IMR1;  // interrupt mask register von PIC 1
    static const IOport IMR2;  // interrupt mask register von PIC 2

public:
    PIC(const PIC& copy) = delete;  // Verhindere Kopieren

    PIC() = default;

    // IRQ-Nummern von Geraeten
    enum {
        timer = 0,     // Programmable Interrupt Timer (PIT)
        keyboard = 1,  // Tastatur
        com1 = 4
    };

    // Freischalten der Weiterleitung eines IRQs durch den PIC an die CPU
    static void allow(int irq);

    // Unterdruecken der Weiterleitung eines IRQs durch den PIC an die CPU
    static void forbid(int irq);

    // Abfragen, ob die Weiterleitung fuer einen bestimmten IRQ unterdrueckt ist
    static bool status(int interrupt_device);
};

#endif
