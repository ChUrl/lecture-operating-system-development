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
#include "kernel/Paging.h"
#include "kernel/threads/IdleThread.h"
#include "user/CoopThreadDemo.h"
#include "user/HelloWorldThread.h"
#include "user/VBEdemo.h"

int main() {
    kout.clear();

    // Speicherverwaltung initialisieren
    allocator.init();

    // Initialize scrollback buffer after allocator.init()
    // kout.init(5);

    // Startmeldung
    if constexpr (!DEBUG) {
        kout << "HHUos 0.8\n"
             << "=========\n"
             << "Unterstuetzte Funktionen:\n"
             << "   - Bildschirmausgaben\n"
             << "   - Sound ueber den PC-Lautsprecher\n"
             << "   - Tastatureingaben per Abfrage\n"
             << "   - Einfache Heap verwaltung\n"
             << "   - Tastatureingaben per Interrupt\n"
             << "   - Kooperative Threads\n"
             << "   - VESA Graphics Mode\n"
             << "   - Paging\n"
             << endl;
    }

    // Tastatur-Unterbrechungsroutine 'einstoepseln'
    /* hier muss Code eingefuegt werden */
    kb.plugin();

    // Interrupts erlauben (Tastatur)
    /* hier muss Code eingefuegt werden */
    cpu.enable_int();

    // Activate paging
    // Scheduler triggers bluescreen?
    // pg_init();

    // Trigger Bluescreen
    // kout << "Trigger Bluescreen, if you can read this it didn't work" << endl;

    // BlueScreen 1
    // asm("int $3");

    // BlueScreen 2
    // unsigned int* page = pg_alloc_page();
    // *page = 42; // Should work
    // pg_write_protect_page(page);
    // *page = 42;  // should trigger BlueScreen

    // Demo threads anlegen
    // scheduler.ready(new HelloWorldThread());
    // scheduler.ready(new CoopThreadDemo());
    // scheduler.ready(new VBEdemo()); // Switch to VESA graphics mode

    // Scheduler starten (schedule() erzeugt den Idle-Thread)
    scheduler.schedule();

    // TODO: Use templates for queue so threads don't have to be casted down from chain
    // TODO: Move scrollback control to thread

    // Scheduler doesn't return
    return 0;
}
