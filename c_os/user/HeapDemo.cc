/*****************************************************************************
 *                                                                           *
 *                            H E A P D E M O                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Demonstration der dynamischen Speicherverwaltung.        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 27.12.2016                      *
 *****************************************************************************/

#include "user/HeapDemo.h"
#include "kernel/Allocator.h"
#include "kernel/Globals.h"
#include "user/MyObj.h"

// Hilfsfunktion: Auf Return-Taste warten
void waitForReturn() {

    /* hier muss Code eingefuegt werden */

    Key key;
    do {
        key = kb.key_hit();
    } while (key.ascii() != '\n');
}

void heap_demo() {

    /* hier muss Code eingefuegt werden */
    allocator.dump_free_memory();

    // Some objects and forward/backward merging
    MyObj* a = new MyObj(5);
    // allocator.dump_free_memory();
    MyObj* b = new MyObj(10);
    // allocator.dump_free_memory();
    MyObj* c = new MyObj(15);
    allocator.dump_free_memory();
    delete b;
    // allocator.dump_free_memory();
    delete a;
    // allocator.dump_free_memory();
    delete c;
    allocator.dump_free_memory();

    // Allocate too whole heap
    // void* ptr = allocator.alloc(1024 * 1024 - 24);
    // allocator.dump_free_memory();
    // allocator.free(ptr);
    // allocator.dump_free_memory();

    // Allocate too much
    // allocator.alloc(1024 * 1024);  // should fail as only 1024 * 1024 - Headersize bytes are available
    // allocator.dump_free_memory();

    // A lot of allocations
    // MyObj* objs[1024];
    // for (unsigned int i = 0; i < 1024; ++i) {
    //     // This tries allocates a lot more than possible
    //     objs[i] = new MyObj(5);
    // }
    // allocator.dump_free_memory();
    // waitForReturn();
    // for (unsigned int i = 0; i < 1024; ++i) {
    //     delete objs[i];
    // }
    // allocator.dump_free_memory();

    // Array allocation
    // MyObj* objs = new MyObj[1024];
    // allocator.dump_free_memory();
    // delete[] objs;
    // allocator.dump_free_memory();
}
