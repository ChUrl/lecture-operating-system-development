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

#ifndef __LinkedListAllocator_include__
#define __LinkedListAllocator_include__

#include "kernel/Allocator.h"
#include "lib/SpinLock.h"
#include "user/lib/Logger.h"

// Format eines freien Blocks, 4 + 4 + 4 Byte
typedef struct free_block {
    bool allocated;  // NOTE: I added this to allow easier merging of free blocks:
                     //       When freeing an allocated block, its next-pointer can
                     //       point to another allocated block, the next free block
                     //       can be found by traversing the leading allocated blocks.
                     //       We only need a way to determine when the free block is reached.
                     //       This also means that the whole list has to be traversed
                     //       to merge blocks. Would be faster with doubly linked list.
    unsigned int size;
    struct free_block* next;
} free_block_t;

class LinkedListAllocator : Allocator {
private:
    // freie Bloecke werden verkettet
    struct free_block* free_start;

    LinkedListAllocator(Allocator& copy) = delete;  // Verhindere Kopieren

    // Traverses the whole list forward till previous block is reached.
    // This can only be called on free blocks as allocated blocks
    // aren't reachable from the freelist.
    static struct free_block* find_previous_block(struct free_block*);

    Logger log;
    SpinLock lock;

public:
    LinkedListAllocator() : log("LL-Alloc") {}

    void init() override;
    void dump_free_memory() override;
    void* alloc(unsigned int req_size) override;
    void free(void* ptr) override;
};

#endif
