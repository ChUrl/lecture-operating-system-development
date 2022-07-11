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

// Demos
// TODO: Make demo header or include through demo menu
#include "user/demo/BlueScreenDemo.h"
#include "user/demo/HeapDemo.h"
#include "user/demo/KeyboardDemo.h"
#include "user/demo/PCSPKdemo.h"
#include "user/demo/PreemptiveThreadDemo.h"
#include "user/demo/TextDemo.h"
#include "user/demo/VBEdemo.h"
#include "user/MainMenu.h"

int main() {
    kout.clear();

    // Speicherverwaltung initialisieren
    allocator.init();

    // Initialize scrollback buffer after allocator.init()
    kout.init(5);

    // Startmeldung
    kout << "HHUos 0.10\n"
         << "==========\n"
         << "Unterstuetzte Funktionen:\n"
         << "   - Bildschirmausgaben\n"
         << "   - Sound ueber den PC-Lautsprecher\n"
         << "   - Tastatureingaben per Abfrage\n"
         << "   - Einfache Heap verwaltung\n"
         << "   - Tastatureingaben per Interrupt\n"
         << "   - Kooperative Threads\n"
         << "   - VESA Graphics Mode\n"
         << "   - Einfaches Paging\n"
         << "   - Preemptive Threads\n"
         << "   - Einfache Synchronisierung\n"
         << endl;

    // Tastatur-Unterbrechungsroutine 'einstoepseln'
    /* hier muss Code eingefuegt werden */
    kb.plugin();
    pit.plugin();

    // Interrupts erlauben (Tastatur, PIT)
    /* hier muss Code eingefuegt werden */
    cpu.enable_int();

    // Activate paging
    // This has to happen after the allocator is initialized but before the scheduler is started
    pg_init();

    // Demos
    // scheduler.ready(new TextDemo());
    // scheduler.ready(new PCSPKdemo(&PCSPK::aerodynamic));
    // scheduler.ready(new KeyboardDemo());
    // scheduler.ready(new HeapDemo());
    // scheduler.ready(new VBEdemo());
    // scheduler.ready(new BlueScreenDemo());
    // scheduler.ready(new PreemptiveThreadDemo());

    // Scheduler starten (schedule() erzeugt den Idle-Thread)
    scheduler.ready(new MainMenu());
    scheduler.schedule();

    // NOTE: Enforced ToDo's (needed)
    // DONE: Rewrite demos for threads
    // TODO: Make menu for demos
    //
    // NOTE: Main ToDo's (extra)
    // TODO: Basic event management for keyboard events so threads can utilize interrupt based inputs
    //       This also works well with a blocked-queue in the scheduler for threads waiting for input
    // TODO: Textual UI with some sort of modeline
    // TODO: Serial output, output graphviz dot data for memory etc.
    // TODO: Some sort of extremely basic shell (basically can't do anything except start demos
    //                                           because there is no filesystem at all)
    // TODO: Fix the damn TreeAllocator: Allow root deletion without bluescreen
    //       Maybe just remove the red black tree stuff and replace with usual binary search tree?
    //       I can just balance this tree unefficiantly by reinserting all nodes
    //
    // NOTE: Cleanup + Refactor
    // TODO: Use templates for queue so threads don't have to be casted down from chain
    // TODO: Change scheduler to only use references instead of pointers
    // TODO: Unify debug output format
    // TODO: Drawing Circles
    // TODO: Cleanup: Remove I added this... Notes, just leave explanations
    // TODO: Remove Math "lib" or do something with it
    // TODO: Cleanup imports: Only import stuff in implementation when only needed there

    // Scheduler doesn't return
    return 0;
}
