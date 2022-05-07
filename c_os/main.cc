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

int main() {
    kout.clear();

    // Speicherverwaltung initialisieren
    allocator.init();

    // text_demo();
    // sound_demo();
    // keyboard_demo();
    heap_demo();

    while (1) {};
    return 0;
}
