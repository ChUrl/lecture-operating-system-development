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

void print_startup_message() {
    kout.lock();
    kout.clear();
    kout << "BSEos 1.0\n"
         << "=========\n"
         << "Unterstuetzte Funktionen:\n"
         << "   - Bildschirmausgaben\n"
         << "   - Sound ueber den PC-Lautsprecher\n"
         << "   - Tastatureingaben per Abfrage\n"
         << "   - Einfache Heapverwaltung\n"
         << "   - Tastatureingaben per Interrupt\n"
         << "   - Kooperative Threads\n"
         << "   - VESA Graphics Mode\n"
         << "   - Einfaches Paging\n"
         << "   - Preemptive Threads\n"
         << "   - Einfache Synchronisierung\n"

         << "   - Einfache (Tastatur-)Eventverwaltung\n"
         << "   - Serial Output Logging\n"
         << "\nPress Enter to continue\n"
         << endl;
    kout.unlock();
}

// #include "test/VectorTest.h"

int main() {
    Logger::set_level(Logger::TRACE);
    Logger::disable_kout();
    Logger::enable_serial();

    // Speicherverwaltung initialisieren
    allocator.init();

    // Tastatur-Unterbrechungsroutine 'einstoepseln'
    kb.plugin();
    pit.plugin();

    // Interrupts erlauben (Tastatur, PIT)
    cpu.enable_int();

    // Activate paging
    // This has to happen after the allocator is initialized but before the scheduler is started
    pg_init();

    // Startmeldung
    print_startup_message();

    // Scheduler starten (schedule() erzeugt den Idle-Thread)
    scheduler.ready<MainMenu>();  // NOTE: A thread that manages other threads has to be added before scheduler.schedule(),
                                  //       because scheduler.schedule() doesn't return, only threads get cpu time
    scheduler.schedule();

    // NOTE: Enforced ToDo's (needed)
    // DONE: Rewrite demos for threads
    // DONE: Make menu for demos
    // DONE: Thread switching stops after a while
    // DONE: Threads are not cleanup after exit, use managed pointer?
    // DONE: Fix PCSKP/Keyboard Demos: Keyboard Thread gets missing after blocking waiting for input, kevman doesn't wake up...
    //
    // NOTE: Main ToDo's (extra)
    // DONE: Basic event management for keyboard events so threads can utilize interrupt based inputs
    //       This also works well with a blocked-queue in the scheduler for threads waiting for input
    //
    // DONE: Serial output
    // CANCELED: Output graphviz stuff over serial?
    // CANCELED: Fix the damn TreeAllocator: Allow root deletion without bluescreen
    //           Maybe just remove the red black tree stuff and replace with usual binary search tree?
    //           I can just balance this tree unefficiantly by reinserting all nodes
    // CANCELED: Implement BST data structure with Tree interface?
    // CANCELED: Implement RBT tree interface implementation?
    // CANCELED: Switch treealloc so the underlying tree can be swapped easily
    // CANCELED: Implement realloc so ArrayList can realloc instead of newly allocate bigger block
    // DONE: Array wrapper
    // DONE: Rewrite Logging with a basic logger
    // DONE: Static Logger
    // CANCELED: String wrapper
    // DONE: Linked List
    // DONE: Iterator support for structures
    // DONE: Implement own basic managed pointers
    // DONE: Scheduler should own threads
    // DONE: Remove CoroutineState/ThreadState and just use pusha/popa, start/switch methods should
    //       just get esp as argument
    // CANCELED: Use singleton pattern for some device classes/classes used only in globals (needs stdlib for exita)
    // DONE: Introduce name to threads?
    // DONE: Allow to gracefully kill threads
    // CANCELED: Request thread status from scheduler (don't need it)
    //
    // NOTE: Cleanup
    // DONE: Use templates for queue so threads don't have to be casted down from chain
    // TODO: Only use references instead of pointers where possible
    // DONE: Unify debug output format
    // TODO: Cleanup: Remove I added this... Notes, just leave explanations
    // DONE: Remove Math "lib" or do something with it
    // CANCELED: Add some fixed point math like sine approximation or fractions?
    // TODO: Cleanup imports: Only import stuff in implementation when only needed there
    // CANCELED: Switch cpu_disableint() to semaphore etc (Spinlock in the scheduler?)
    // CANCELED: Change mylib types to not use T* but T and call with memcpy<Type*> instead of memcpy<Type>?
    // TODO: Make more stuff const and static and static constexpr const
    // DONE: Remove ArrayList init and do this inside ArrayList when an operation on the list is done
    // DONE: Kevman unsubscribe is needed, because exited threads will still be woken up by kevman
    //       Or check if thread is still running
    // TODO: Delete copy constructors that weren't already deleted
    // DONE: Switch out semaphore Queue with ArrayList? Or switch back Scheduler to Queue?
    // TODO: Add virtual destructors and make sure to call them with delete when objects are removed
    // TODO: Replace empty constructors/destructors with default keyword
    // DONE: Synchronize the outstream
    // DONE: Remove Iterator from List.h
    // DONE: Move Array/ArrayList/LinkedList/List to bse namespace
    // DONE: Remove the Input.h file and replace functionality with kevman

    // Scheduler doesn't return
    return 0;
}
