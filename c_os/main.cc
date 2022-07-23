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
#include "user/PCSPKdemo.h"
#include "user/PreemptiveThreadDemo.h"
#include "user/VBEdemo.h"

int main() {
    kout.clear();

    // Speicherverwaltung initialisieren
    allocator.init();

    // Startmeldung
    kout << "HHUos 0.10\n"
         << "=========\n"
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

    // TODO: Move this to demo
    // Trigger Bluescreen
    // kout << "Trigger Bluescreen, if you can read this it didn't work" << endl;

    // BlueScreen 1
    // asm("int $3");

    // BlueScreen 2
    // unsigned int* page = pg_alloc_page();
    // *page = 42;
    // pg_write_protect_page(page);
    // invalidate_tlb_entry(page); // If we don't invalidate after first access the write protection
    //                             // won't work as no lookup is performed (address in tlb)
    // *page = 42; // We map logical to physical 1:1 so no need to do any lookup
    //             // If tlb is invalidated this access produces a pagefault

    // TODO: Make menu for demos
    // Demo threads anlegen
    // scheduler.ready(new HelloWorldThread());
    // scheduler.ready(new CoopThreadDemo());
    // scheduler.ready(new VBEdemo()); // Switch to VESA graphics mode
    // scheduler.ready(new PCSPKdemo());
    scheduler.ready(new PreemptiveThreadDemo());

    // Scheduler starten (schedule() erzeugt den Idle-Thread)
    scheduler.schedule();

    // TODO: Use templates for queue so threads don't have to be casted down from chain
    // TODO: Change scheduler to only use references instead of pointers
    // TODO: Rewrite all demos to threads
    // TODO: Unify debug output format
    // TODO: Serial output, output graphviz dot data for memory etc.

    // Scheduler doesn't return
    return 0;
}
