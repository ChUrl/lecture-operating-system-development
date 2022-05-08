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

// I don't order the list by size so that the block order corresponds to the location in memory
// Then I can easily merge adjacent free blocks by finding the previous block without looking at
// memory addresses of each block
// (That was the plan at least)

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
    this->free_start->allocated = false;
    this->free_start->size = this->heap_size - sizeof(struct free_block);
    this->free_start->next = this->free_start;  // Only one block, points to itself

    kout << "Initialized LinkedList Allocator" << endl
         << " - Heap Start: " << hex << (unsigned int)this->heap_start
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
        kout << " - No free Blocks" << endl;
    } else {
        kout << " - Freelist start: " << hex << (unsigned int)this->free_start << endl;

        struct free_block* current = this->free_start;
        do {
            kout << " - Free Block (Start: " << hex << (unsigned int)current
                 << " Size: " << hex << current->size << ")" << endl;
            current = current->next;
        } while (current != free_start);
    }
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
        kout << " - No free memory remaining :(" << endl;
        return NULL;
    }

    struct free_block* current = this->free_start;
    do {
        if (current->size >= req_size) {
            // Current block large enough
            // We now have: [<> | current | <>]

            // Don't subtract to prevent underflow
            if (current->size >= req_size + sizeof(struct free_block) + HEAP_MIN_FREE_BLOCK_SIZE) {
                // Block so large it can be cut

                // Create new header after allocated memory and rearrange pointers
                // [<> | current | new_next | <>]
                // In case of only one freeblock:
                // [current | new_next]
                struct free_block* new_next =
                  (struct free_block*)((unsigned int)current + sizeof(struct free_block) + req_size);

                // If only one block exists, current->next is current
                // This shouldn't be a problem since the block gets removed from the list later
                new_next->next = current->next;
                new_next->size = current->size - (req_size + sizeof(struct free_block));

                current->next = new_next;  // We want to reach the next free block from the allocated block
                current->size = req_size;

                // Next-fit
                this->free_start = new_next;

                kout << " - Allocated " << hex << req_size << " Bytes with cutting" << endl;
            } else {
                // Block too small to be cut, allocate whole block

                // Next-fit
                this->free_start = current->next;  // Pointer keeps pointing to current if last block
                if (this->free_start == current) {
                    // No free block remaining
                    kout << " - Disabled freelist" << endl;
                    this->free_start = NULL;
                }

                kout << " - Allocated " << hex << current->size << " Bytes without cutting" << endl;
            }

            // Block aushängen
            struct free_block* previous = this->find_previous_block(current);
            previous->next = current->next;
            current->allocated = true;

            // We leave the current->next pointer intact although the block is allocated
            // to allow easier merging of adjacent free blocks

            return (void*)((unsigned int)current + sizeof(struct free_block));  // Speicheranfang, nicht header
        }

        current = current->next;
    } while (current != this->free_start);  // Stop when arriving at the first block again

    kout << " - More memory requested than available :(" << endl;
    return NULL;
}

/*****************************************************************************
 * Methode:         LinkedListAllocator::free                                *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einen Speicherblock freigeben.                           *
 *****************************************************************************/
void LinkedListAllocator::free(void* ptr) {

    /* Hier muess Code eingefuegt werden */

    kout << "Freeing " << hex << (unsigned int)ptr << endl;

    struct free_block* block_start = (struct free_block*)((unsigned int)ptr - sizeof(struct free_block));

    // Reenable the freelist if no block was available
    // This also means that no merging can be done
    if (this->free_start == NULL) {
        this->free_start = block_start;
        block_start->allocated = false;
        block_start->next = block_start;

        kout << " - Enabling freelist with one block" << endl;

        return;
    }

    struct free_block* next_block =
      (struct free_block*)((unsigned int)block_start + sizeof(struct free_block) + block_start->size);

    // Find the next free block, multiple next blocks can be allocated so walk through them
    struct free_block* next_free = block_start->next;
    while (next_free->allocated) {
        next_free = next_free->next;
    }

    struct free_block* previous_free = this->find_previous_block(next_free);
    struct free_block* previous_free_next =
      (struct free_block*)((unsigned int)previous_free + sizeof(struct free_block) + previous_free->size);

    // We have: [previous_free | previous_free_next | <> | block_start | next_block | <> | next_free]
    // The <> spaces don't have to exist and next_block could be the same as next_free
    // or previous_free_next the same as block_start
    // Also next_block and previous_free_next could be allocated blocks
    //
    // If previous_free/next_free and block_start are adjacent and free, they can be merged:
    //  - If next_block and next_free are the same block we can merge forward
    //    Should result in: [previous_free | previous_free_next | <> | block_start]
    //  - If previous_free_next and block_start are the same block we can merge backward
    //    Should result in: [block_start]

    // Try to merge forward ========================================================================
    if (next_block == next_free) {
        kout << " - Merging block forward" << endl;

        // Current and next adjacent block can be merged
        // [previous_free | previous_free_next | <> | block_start | next_free]

        block_start->next = next_free->next;  // We make next_free disappear
        if (block_start->next == next_free) {
            // If next_free is the only free block it points to itself, so fix that
            block_start->next = block_start;
        }
        block_start->size = block_start->size + sizeof(struct free_block) + next_free->size;

        // There shouldn't exist any other allocated blocks pointing to next_free,
        // the current one should be the only one (or else I have done something wrong)
        // If thats the case I should set the next pointer to the next adjacent block
        // when allocating a new block

        if (this->free_start == next_free) {
            // next_free is now invalid after merge
            kout << " - Moving freelist start to " << hex << (unsigned int)block_start << endl;
            this->free_start = block_start;
        }
    } else {
        // Can't merge forward so size stays the same
        // [previous_free | previous_free_next | <> | block_start | <> | next_free]

        block_start->next = next_free;
    }

    // Attach new free block to freelist
    // This could write into a free block, but doesn't matter
    previous_free->next = block_start;

    // Try to merge backward   =====================================================================
    if (previous_free_next == block_start) {
        kout << " - Merging block backward" << endl;

        // Current and previous adjacent block can be merged
        // [previous_free | block_start]

        previous_free->next = block_start->next;
        previous_free->size = previous_free->size + sizeof(struct free_block) + block_start->size;

        // For pointers to block_start the same as above applies
        // so I don't think I have to manage anything else here

        if (this->free_start == block_start) {
            // block_start is now invalid after merge
            kout << " - Moving freelist start to " << hex << (unsigned int)previous_free << endl;
            this->free_start = previous_free;
        }
    }

    // Depending on the merging this might write into the block, but doesn't matter
    block_start->allocated = false;
}

// NOTE: I added this
struct free_block* LinkedListAllocator::find_previous_block(struct free_block* next_block) {
    // Durchlaufe die ganze freispeicherliste bis zum Block der auf next_block zeigt
    struct free_block* current = next_block;
    while (current->next != next_block) {
        // NOTE: This will get stuck if called on the wrong block
        current = current->next;
    }

    // if (current == next_block) {
    //     kout << "LinkedListAllocator::find_previous_block returned the input block" << endl;
    // }

    return current;
}
