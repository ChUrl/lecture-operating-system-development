/*****************************************************************************
 *                                                                           *
 *                  L I N K E D L I S T A L L O C A T O R                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einfache Speicherverwaltung, welche den freien Speicher  *
 *                  mithilfe einer einfach verketteten Liste verwaltet.      *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 13.6.2020                        *
 *****************************************************************************/

#include "kernel/allocator/LinkedListAllocator.h"
#include "kernel/Globals.h"
#include <stddef.h>

#define HEAP_MIN_FREE_BLOCK_SIZE 64  // min. Groesse eines freien Blocks

/*****************************************************************************
 * Methode:         LinkedListAllocator::init                                *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Liste der Freispeicherbloecke intitialisieren.           *
 *                  Anker zeigt auf ein Dummy-Element. Danach folgt          *
 *                  ein Block der den gesamten freien Speicher umfasst.      *
 *                                                                           *
 *                  Wird automatisch aufgerufen, sobald eine Funktion der    *
 *                  Speicherverwaltung erstmalig gerufen wird.               *
 *****************************************************************************/
void LinkedListAllocator::init() {

    /* Hier muess Code eingefuegt werden */

    this->free_start = (struct free_block*)this->heap_start;
    this->free_start->size = this->heap_size - sizeof(struct free_block);
    this->free_start->next = this->free_start;  // Only one block, points to itself

    kout << "Initialized LinkedList Allocator" << endl
         << "Heap Start: " << hex << (unsigned int)this->heap_start
         << ", Heap End: " << hex << (unsigned int)this->heap_end
         << ", Heap Size: " << hex << this->heap_size << endl;
    kout << endl;
}

/*****************************************************************************
 * Methode:         LinkedListAllocator::dump_free_memory                    *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Ausgabe der Freispeicherliste. Zu Debuggingzwecken.      *
 *****************************************************************************/
void LinkedListAllocator::dump_free_memory() {

    /* Hier muess Code eingefuegt werden */

    kout << "Freier Speicher:" << endl;

    if (this->free_start == NULL) {
        kout << "No free Blocks" << endl;
    } else {
        kout << "Freelist start: " << hex << (unsigned int)this->free_start << endl;

        struct free_block* current = this->free_start;
        do {
            kout << " - Free Block (Start: " << hex << (unsigned int)current
                 << " Size: " << hex << current->size << ")" << endl;
            current = current->next;
        } while (current != free_start);
    }

    kout << endl;
}

/*****************************************************************************
 * Methode:         LinkedListAllocator::alloc                               *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einen neuen Speicherblock allozieren.                    * 
 *****************************************************************************/
void* LinkedListAllocator::alloc(unsigned int req_size) {

    /* Hier muess Code eingefuegt werden */
    // NOTE: next pointer zeigt auf headeranfang, returned wird zeiger auf anfang des nutzbaren freispeichers

    kout << "Requested " << hex << req_size << " Bytes" << endl;

    if (this->free_start == NULL) {
        kout << "No free memory remaining :(" << endl;
        return NULL;
    }

    struct free_block* current = this->free_start;
    do {
        if (current->size >= req_size) {
            // Current block large enough

            // Don't subtract or underflow
            if (current->size >= req_size + sizeof(struct free_block) + HEAP_MIN_FREE_BLOCK_SIZE) {
                // Block so large it can be cut

                // Create new header after allocated memory and rearrange pointers
                struct free_block* new_next =
                  (struct free_block*)((unsigned int)current + sizeof(struct free_block) + req_size);
                new_next->next = current->next;
                new_next->size = current->size - (req_size + sizeof(struct free_block));
                current->next = new_next;
                current->size = req_size;

                // Next-fit
                this->free_start = new_next;

                kout << "Allocated " << hex << req_size << " Bytes with cutting" << endl;
                kout << endl;
            } else {
                // Block too small to be cut, allocate whole block

                // Next-fit
                this->free_start = current->next;  // Pointer keeps pointing to current if last block
                if (this->free_start == current) {
                    // No free block remaining
                    this->free_start = NULL;
                }

                kout << "Allocated " << hex << current->size << " Bytes without cutting" << endl;
                kout << endl;
            }

            // Block aushängen
            struct free_block* previous = this->find_previous_block(current);
            previous->next = current->next;

            return (void*)((unsigned int)current + sizeof(struct free_block));  // Speicheranfang, nicht header
        }

        current = current->next;
    } while (current != this->free_start);  // Stop when arriving at the first block again

    kout << "More memory requested than available :(" << endl;
    kout << endl;
    return NULL;
}

/*****************************************************************************
 * Methode:         LinkedListAllocator::free                                *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einen Speicherblock freigeben.                           *
 *****************************************************************************/
void LinkedListAllocator::free(void* ptr) {

    /* Hier muess Code eingefuegt werden */

    kout << "Free not implemented (yet)" << endl;
}

// NOTE: I added this
struct free_block* LinkedListAllocator::find_previous_block(struct free_block* next_block) {
    // Durchlaufe die ganze freispeicherliste bis zum Block der auf next_block zeigt
    struct free_block* current = next_block;
    while (current->next != next_block) {
        current = current->next;
    }
    return current;
}
