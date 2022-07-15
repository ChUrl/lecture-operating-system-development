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
#include "user/MainMenu.h"

int main() {
    kout.clear();

    // Speicherverwaltung initialisieren
    allocator.init();

    // Initialize scrollback buffer after allocator.init()
    // kout.init(5);

    // Initialize SerialPort
    serial.init();

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
    kb.plugin();
    pit.plugin();

    // Interrupts erlauben (Tastatur, PIT)
    cpu.enable_int();

    // Activate paging
    // This has to happen after the allocator is initialized but before the scheduler is started
    pg_init();

    // NOTE: Because I introduced dynamic memory inside the scheduler (probably bad idea?) I have to add
    //       these init methods (allocator has to be initialized before but scheduler is constructed in the globals)
    scheduler.init();

    // Test
    serial.write("Hello, this is a test message\r\n");
    serial.write("Hello, this is a test message\r\n");

    // Scheduler starten (schedule() erzeugt den Idle-Thread)
    scheduler.ready(new MainMenu());  // NOTE: A thread that manages other threads has to be added before scheduler.schedule(),
                                      //       because scheduler.schedule() doesn't return, only threads get cpu time
    scheduler.schedule();

    // BUG: Timer interrupt preemption works normal at first, but after a time there is a heap of output
    //      from the scheduler that is not triggered by PIT, I don't know why this occurs

    // NOTE: Enforced ToDo's (needed)
    // DONE: Rewrite demos for threads
    // TODO: Make menu for demos
    // TODO: Thread switching stops after a while
    // TODO: Prefer deblocked thread and find out why it doesn't work
    // TODO: Threads are not cleanup after exit, use managed pointer?
    //
    // NOTE: Main ToDo's (extra)
    // DONE: Basic event management for keyboard events so threads can utilize interrupt based inputs
    //       This also works well with a blocked-queue in the scheduler for threads waiting for input
    //
    // TODO: Serial output, output graphviz dot data for memory etc.
    // TODO: Fix the damn TreeAllocator: Allow root deletion without bluescreen
    //       Maybe just remove the red black tree stuff and replace with usual binary search tree?
    //       I can just balance this tree unefficiantly by reinserting all nodes
    // TODO: Implement realloc so ArrayList can realloc instead of newly allocate bigger block
    // TODO: Array wrapper
    // TODO: Rewrite Logging with a basic (synchronized) logger
    // TODO: String wrapper
    //
    // NOTE: Cleanup + Refactor
    // TODO: Use templates for queue so threads don't have to be casted down from chain
    // TODO: Change scheduler to only use references instead of pointers
    // TODO: Unify debug output format
    // TODO: Drawing Circles
    // TODO: Cleanup: Remove I added this... Notes, just leave explanations
    // TODO: Remove Math "lib" or do something with it
    // TODO: Cleanup imports: Only import stuff in implementation when only needed there
    // TODO: Switch cpu_disableint() to semaphore etc (Spinlock in the scheduler?)
    // TODO: Change mylib types to not use T* but T and call with memcpy<Type*> instead of memcpy<Type>
    // TODO: Make more stuff const
    // TODO: Remove ArrayList init and do this inside ArrayList when an operation on the list is done
    // TODO: Remove CoroutineState/ThreadState and just use pusha/popa, start/switch methods should
    //       just get esp as argument
    // TODO: Kevman unsubscribe is needed, because exited threads will still be woken up by kevman
    //       Or check if thread is still running

    // Scheduler doesn't return
    return 0;
}
