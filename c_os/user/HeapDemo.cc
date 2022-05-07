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
        kb.key_hit();
    } while (key.ascii() != '\n');
}

void heap_demo() {

    /* hier muss Code eingefuegt werden */
    allocator.dump_free_memory();

    MyObj* a = new MyObj(5);
    allocator.dump_free_memory();

    MyObj* b = new MyObj(10);
    allocator.dump_free_memory();

    allocator.alloc(1024 * 1024);  // should fail as only 1024 * 1024 - (4 + 4) bytes remain
    allocator.dump_free_memory();
}
