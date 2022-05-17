/*****************************************************************************
 *                                                                           *
 *                                M A I N                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Startroutine, wird direkt vom Bootlader angesprungen,    *
 *                  nachdem dieser in den Protected Mode geschaltet hat und  *
 *                  die GDT und IDT initalisiert hat.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 11.11.2016                      *
 *****************************************************************************/

#include "kernel/Allocator.h"
#include "kernel/Globals.h"
#include "user/HeapDemo.h"
#include "user/KeyIRQDemo.h"

int main() {
    kout.clear();

    // Speicherverwaltung initialisieren
    allocator.init();

    // Initialize scrollback buffer after allocator.init()
    kout.init(5);

    // Tastatur-Unterbrechungsroutine 'einstoepseln'
    /* hier muss Code eingefuegt werden */
    kb.plugin(intdis, pic);

    // Interrupts erlauben (Tastatur)
    /* hier muss Code eingefuegt werden */
    cpu.enable_int();

    // text_demo();
    // sound_demo();
    // keyboard_demo();
    heap_demo();
    // key_irq_demo();

    while (1) {};
    return 0;
}
