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

#include "kernel/interrupts/PIC.h"
#include "kernel/IOport.h"

static IOport IMR1(0x21);  // interrupt mask register von PIC 1
static IOport IMR2(0xa1);  // interrupt mask register von PIC 2

/*****************************************************************************
 * Methode:         PIC::allow                                               *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Sorgt dafuer, dass der uebergebene IRQ ab sofort durch   *
 *                  den PIC an den Prozessor weitergereicht wird. Um eine    *
 *                  Unterbrechungsbehandlung zu ermoeglichen, muss           *
 *                  zusaetzlich CPU::enable_int() aufgerufen werden.         *
 *                                                                           *
 * Parameter:                                                                *
 *      irq:        IRQ der erlaubt werden soll                              *
 *****************************************************************************/
void PIC::allow(int irq) {

    /* hier muss Code eingefuegt werden */

    // NOTE: allow sets the bit to 0

    unsigned char IMR;
    unsigned char mask = ~(0x1 << (irq % 8));
    if (irq < 8) {
        // PIC 1
        IMR = IMR1.inb();  // We don't want to change the other interrupt masks so use this as start value
        IMR1.outb(IMR & mask);
    } else {
        // PIC 2
        IMR = IMR2.inb();
        IMR2.outb(IMR & mask);
    }
}

/*****************************************************************************
 * Methode:         PIC::forbid                                              *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Unterdrueckt mit Hilfe des PICs einen bestimmten IRQ.    *
 *                                                                           *
 * Parameter:                                                                *
 *      interrupt:  IRQ der maskiert werden soll                             *
 *****************************************************************************/
void PIC::forbid(int irq) {

    /* hier muss Code eingefuegt werden */

    // NOTE: forbid sets the bit to 1

    unsigned char IMR;
    unsigned char mask = 0x1 << (irq % 8);
    if (irq < 8) {
        // PIC 1
        IMR = IMR1.inb();  // We don't want to change the other interrupt masks so use this as start value
        IMR1.outb(IMR | mask);
    } else {
        // PIC 2
        IMR = IMR2.inb();
        IMR2.outb(IMR | mask);
    }
}

/*****************************************************************************
 * Methode:         PIC::status                                              *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Liefert den aktuellen Zustand des Maskierbits eines      *
 *                  bestimmten IRQs.                                         *
 *                                                                           *
 * Parameter:                                                                *
 *      irq:  IRQ dessen Status erfragt werden soll                          *
 *****************************************************************************/
bool PIC::status(int irq) {

    /* hier muss Code eingefuegt werden */

    // TODO: How is IRQ2 handled (Slave PIC)?
    //       Does masking IRQ2 disable the whole slave?

    unsigned char IMR;
    if (irq < 8) {
        // PIC 1
        IMR = IMR1.inb();
    } else {
        // PIC 2
        IMR = IMR2.inb();
    }

    // Use % 8 to account for two PICs
    unsigned char mask = 0x1 << (irq % 8);
    return IMR & mask;
}
