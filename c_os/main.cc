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

#include "kernel/Globals.h"
#include "user/CoopThreadDemo.h"
#include "user/HelloWorldThread.h"

int main() {
    kout.clear();

    // Speicherverwaltung initialisieren
    allocator.init();

    // Initialize scrollback buffer after allocator.init()
    kout.init(5);

    // Startmeldung
    if constexpr (!DEBUG) {
        kout << "HHUos 0.6\n"
             << "=========\n"
             << "Unterstuetzte Funktionen:\n"
             << "   - Bildschirmausgaben\n"
             << "   - Sound ueber den PC-Lautsprecher\n"
             << "   - Tastatureingaben per Abfrage\n"
             << "   - Einfache Heap verwaltung\n"
             << "   - Tastatureingaben per Interrupt\n"
             << "   - Kooperative Threads\n"
             << endl;
    }

    // Tastatur-Unterbrechungsroutine 'einstoepseln'
    /* hier muss Code eingefuegt werden */
    kb.plugin();

    // Interrupts erlauben (Tastatur)
    /* hier muss Code eingefuegt werden */
    cpu.enable_int();

    // text_demo();
    // sound_demo();
    // keyboard_demo();
    // heap_demo();
    // key_irq_demo();
    // coroutineDemo.main();

    // Threads anlegen
    // scheduler.ready(new HelloWorldThread());
    scheduler.ready(new CoopThreadDemo());

    // Scheduler starten (schedule() erzeugt den Idle-Thread)
    scheduler.schedule();

    // TODO: Use templates for queue so threads don't have to be casted down from chain
    // TODO: Move scrollback control to thread

    return 0;
}
