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
#include "lib/Input.h"
#include "user/BlueScreenDemo.h"
#include "user/CoopThreadDemo.h"
#include "user/HeapDemo.h"
#include "user/HelloWorldThread.h"
#include "user/KeyboardDemo.h"
#include "user/KeyIRQDemo.h"
#include "user/PCSPKdemo.h"
#include "user/PreemptiveThreadDemo.h"
#include "user/SoundDemo.h"
#include "user/TextDemo.h"
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

    kout << "Demos:\n"
         << "1 - VBEdemo\n"
         << "2 - PCSPKdemo\n"
         << "3 - PreemptiveThreadDemo\n"
         << "4 - BlueScreenDemo\n"
         << "5 - HeapDemo\n"
         << "6 - KeyboardDemo\n"
         << "7 - TextDemo" << endl;

    char input = getch();
    switch (input) {
    case '1':
        scheduler.ready(new VBEdemo());  // Switch to VESA graphics mode
        break;
    case '2':
        scheduler.ready(new PCSPKdemo());
        break;
    case '3':
        scheduler.ready(new PreemptiveThreadDemo());
        break;
    case '4':
        bluescreen_demo();
        break;
    case '5':
        heap_demo();
        break;
    case '6':
        keyboard_demo();
        break;
    case '7':
        text_demo();
        break;
    }
    // scheduler.ready(new HelloWorldThread());
    // scheduler.ready(new CoopThreadDemo());

    // Scheduler starten (schedule() erzeugt den Idle-Thread)
    scheduler.schedule();

    // Scheduler doesn't return
    return 0;
}
