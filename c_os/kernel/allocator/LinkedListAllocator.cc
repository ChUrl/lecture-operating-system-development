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
#include "user/lib/Logger.h"

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

    this->free_start = (free_block_t*)this->heap_start;
    this->free_start->allocated = false;
    this->free_start->size = this->heap_size - sizeof(free_block_t);
    this->free_start->next = this->free_start;  // Only one block, points to itself

    log.info() << "Initialized LinkedList Allocator" << endl;
}

/*****************************************************************************
 * Methode:         LinkedListAllocator::dump_free_memory                    *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Ausgabe der Freispeicherliste. Zu Debuggingzwecken.      *
 *****************************************************************************/
void LinkedListAllocator::dump_free_memory() {

    /* Hier muess Code eingefuegt werden */

    kout << "Freier Speicher:" << endl;

    if (this->free_start == nullptr) {
        kout << " - No free Blocks" << endl;
    } else {
        kout << " - Freelist start: " << hex << (unsigned int)this->free_start << endl;

        free_block_t* current = this->free_start;
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
    this->lock.acquire();

    /* Hier muess Code eingefuegt werden */
    // NOTE: next pointer zeigt auf headeranfang, returned wird zeiger auf anfang des nutzbaren freispeichers

    log.debug() << "Requested " << hex << req_size << " Bytes" << endl;

    if (this->free_start == nullptr) {
        log.error() << " - No free memory remaining :(" << endl;
        this->lock.release();
        return nullptr;
    }

    // Round to word borders
    unsigned int req_size_diff = (BASIC_ALIGN - req_size % BASIC_ALIGN) % BASIC_ALIGN;
    unsigned int rreq_size = req_size + req_size_diff;
    if (req_size_diff > 0) {
        log.trace() << " - Rounded to word border (+" << dec << req_size_diff << " bytes)" << endl;
    }

    free_block_t* current = this->free_start;
    do {
        if (current->size >= rreq_size) {  // Size doesn't contain header, only usable
            // Current block large enough
            // We now have: [<> | current | <>]

            // Don't subtract to prevent underflow
            if (current->size >= rreq_size + sizeof(free_block_t) + HEAP_MIN_FREE_BLOCK_SIZE) {
                // Block so large it can be cut

                // Create new header after allocated memory and rearrange pointers
                // [<> | current | new_next | <>]
                // In case of only one freeblock:
                // [current | new_next]
                free_block_t* new_next =
                  (free_block_t*)((unsigned int)current + sizeof(free_block_t) + rreq_size);

                // If only one block exists, current->next is current
                // This shouldn't be a problem since the block gets removed from the list later
                new_next->next = current->next;
                new_next->size = current->size - (rreq_size + sizeof(free_block_t));
                new_next->allocated = false;

                current->next = new_next;  // We want to reach the next free block from the allocated block
                current->size = rreq_size;

                // Next-fit
                this->free_start = new_next;

                log.trace() << " - Allocated " << hex << rreq_size << " Bytes with cutting" << endl;
            } else {
                // Block too small to be cut, allocate whole block

                // Next-fit
                this->free_start = current->next;  // Pointer keeps pointing to current if last block
                if (this->free_start == current) {
                    // No free block remaining
                    log.trace() << " - Disabled freelist" << endl;
                    this->free_start = nullptr;
                }

                log.trace() << " - Allocated " << hex << current->size << " Bytes without cutting" << endl;
            }

            // Block aushängen
            // If block was cut this is obvious, because current->next is a free block
            // If block wasn't cut it also works as current was a free block before, so it's next
            // block should also be free
            free_block_t* previous = LinkedListAllocator::find_previous_block(current);
            previous->next = current->next;  // Current block was free so previous block pointed at it
            current->allocated = true;

            // We leave the current->next pointer intact although the block is allocated
            // to allow easier merging of adjacent free blocks

            // HACK: Checking list integrity
            // free_block_t* c = current;
            // log.debug() << "Checking list Integrity" << endl;
            // while (c->allocated) {
            //     log.debug() << hex << (unsigned int)c << endl;
            //     c = c->next;
            // }
            // log.debug() << "Finished check" << endl;

            log.debug() << "Returning memory address " << hex << ((unsigned int)current + sizeof(free_block_t)) << endl;
            this->lock.release();
            return (void*)((unsigned int)current + sizeof(free_block_t));  // Speicheranfang, nicht header
        }

        current = current->next;
    } while (current != this->free_start);  // Stop when arriving at the first block again

    log.error() << " - More memory requested than available :(" << endl;
    this->lock.release();
    return nullptr;
}

/*****************************************************************************
 * Methode:         LinkedListAllocator::free                                *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einen Speicherblock freigeben.                           *
 *****************************************************************************/
void LinkedListAllocator::free(void* ptr) {
    this->lock.acquire();

    /* Hier muess Code eingefuegt werden */

    // Account for header
    free_block_t* block_start = (free_block_t*)((unsigned int)ptr - sizeof(free_block_t));

    log.debug() << "Freeing " << hex << (unsigned int)ptr << ", Size: " << block_start->size << endl;

    if (!block_start->allocated) {
        log.error() << "Block already free" << endl;
        this->lock.release();
        return;
    }

    // Reenable the freelist if no block was available
    // This also means that no merging can be done
    if (this->free_start == nullptr) {
        this->free_start = block_start;
        block_start->allocated = false;
        block_start->next = block_start;

        log.trace() << " - Enabling freelist with one block" << endl;
        this->lock.release();
        return;
    }

    free_block_t* next_block =
      (free_block_t*)((unsigned int)block_start + sizeof(free_block_t) + block_start->size);

    // Find the next free block, multiple next blocks can be allocated so walk through them
    free_block_t* next_free = block_start->next;
    while (next_free->allocated) {
        next_free = next_free->next;
    }

    free_block_t* previous_free = LinkedListAllocator::find_previous_block(next_free);
    free_block_t* previous_free_next =
      (free_block_t*)((unsigned int)previous_free + sizeof(free_block_t) + previous_free->size);

    // We have: [previous_free | previous_free_next | <> | block_start | next_block | <> | next_free]
    // The <> spaces don't have to exist and next_block could be the same as next_free
    // or previous_free_next the same as block_start
    // Also next_block and previous_free_next could be allocated blocks

    // If previous_free/next_free and block_start are adjacent and free, they can be merged:
    //  - If next_block and next_free are the same block we can merge forward
    //    Should result in: [previous_free | previous_free_next | <> | block_start]
    //  - If previous_free_next and block_start are the same block we can merge backward
    //    Should result in: [block_start]

    // log.trace() << "Before doing any merging:" << endl;
    // log.trace() << "previous_free:" << hex << (unsigned int)previous_free << "Size:" << previous_free->size << "Next:" << (unsigned int)previous_free->next << endl;
    // log.trace() << "previous_free_next:" << hex << (unsigned int)previous_free_next << "Size:" << previous_free_next->size << "Next:" << (unsigned int)previous_free_next->next << endl;
    // log.trace() << "block_start:" << hex << (unsigned int)block_start << "Size:" << block_start->size << "Next:" << (unsigned int)block_start->next << endl;
    // log.trace() << "next_block:" << hex << (unsigned int)next_block << "Size:" << next_block->size << "Next:" << (unsigned int)next_block->next << endl;
    // log.trace() << "next_free:" << hex << (unsigned int)next_free << "Size:" << next_free->size << "Next:" << (unsigned int)next_free->next << endl;

    // Try to merge forward ========================================================================
    if (next_block == next_free) {
        log.trace() << " - Merging block forward" << endl;

        // Current and next adjacent block can be merged
        // [previous_free | previous_free_next | <> | block_start | next_free]

        // [block_start | next_free | next_free->next] => [block_start | next_free->next]
        block_start->next = next_free->next;  // We make next_free disappear
        if (block_start->next == next_free) {
            // If next_free is the only free block it points to itself, so fix that
            // [block_start | next_free], but we want to remove next_free
            block_start->next = block_start;
            // [block_start]
        }
        block_start->size = block_start->size + sizeof(free_block_t) + next_free->size;

        // There shouldn't exist any other allocated blocks pointing to next_free,
        // the current one should be the only one (or else I have done something wrong)
        // If thats the case I should set the next pointer to the next adjacent block
        // when allocating a new block

        if (this->free_start == next_free) {
            // next_free is now invalid after merge
            log.trace() << " - Moving freelist start to " << hex << (unsigned int)block_start << endl;
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
        log.trace() << " - Merging block backward" << endl;

        // Current and previous adjacent block can be merged
        // [previous_free | block_start]

        previous_free->next = block_start->next;
        previous_free->size = previous_free->size + sizeof(free_block_t) + block_start->size;

        // For pointers to block_start the same as above applies
        // so I don't think I have to manage anything else here

        if (this->free_start == block_start) {
            // block_start is now invalid after merge
            log.trace() << " - Moving freelist start to " << hex << (unsigned int)previous_free << endl;
            this->free_start = previous_free;
        }
    }

    // Depending on the merging this might write into the block, but doesn't matter
    block_start->allocated = false;
    this->lock.release();
}

free_block_t* LinkedListAllocator::find_previous_block(free_block_t* next_block) {
    // Durchlaufe die ganze freispeicherliste bis zum Block der auf next_block zeigt
    free_block_t* current = next_block;
    while (current->next != next_block) {
        // NOTE: This will get stuck if called on the wrong block
        current = current->next;
    }

    // if (current == next_block) {
    //     kout << "LinkedListAllocator::find_previous_block returned the input block" << endl;
    // }

    return current;
}
