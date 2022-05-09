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

// Waits for keys to control the scrollback buffer display
void scroll_mode() {
    kout.show(kout.COLUMNS - 1, 0, (char)(48 + kout.current_page));

    Key key;
    while (true) {
        key = kb.key_hit();

        switch ((char)key) {
        case 'k':
            kout.scroll_page_backward();
            kout.show(kout.COLUMNS - 1, 0, (char)(48 + kout.current_page));
            break;
        case 'j':
            kout.scroll_page_forward();
            kout.show(kout.COLUMNS - 1, 0, (char)(48 + kout.current_page));
            break;
        }
    }
}

int main() {
    kout.clear();

    // Speicherverwaltung initialisieren
    allocator.init();

    // Initialize scrollback buffer after allocator.init()
    kout.init();

    // text_demo();
    // sound_demo();
    // keyboard_demo();
    heap_demo();

    scroll_mode();

    while (1) {};
    return 0;
}
