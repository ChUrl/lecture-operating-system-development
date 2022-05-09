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

// TODO: Where to put this?
void scroll_mode() {
    Key key;
    // while (true) {
    //     key = kb.key_hit();

    //     switch (key.ascii()) {
    //     case 'k':
    //         kout.pageup();
    //     case 'j':
    //         kout.pagedown();
    //     }
    // }
}

int main() {
    kout.clear();

    // Speicherverwaltung initialisieren
    allocator.init();

    // Initialize scrollback buffer
    // kout.init();

    allocator.dump_free_memory();

    // text_demo();
    // sound_demo();
    // keyboard_demo();
    heap_demo();

    // scroll_mode();

    while (1) {};
    return 0;
}
