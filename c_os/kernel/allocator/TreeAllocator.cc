#include "kernel/allocator/TreeAllocator.h"
#include "kernel/Globals.h"
#include <stddef.h>

// NOTE: I added this file

void TreeAllocator::init() {
    this->free_start = (tree_block_t*)this->heap_start;
    this->free_start->allocated = false;
    this->free_start->left = NULL;
    this->free_start->right = NULL;
    this->free_start->parent = NULL;
    this->free_start->red = false; // The root is always black
    this->free_start->next = (list_block_t*)this->free_start;
    this->free_start->previous = (list_block_t*)this->free_start;

    if constexpr (DEBUG) { kout << "Initialized Tree Allocator" << endl; }
}

void TreeAllocator::dump_free_memory() {
    kout << "Free Memory:" << endl;
    list_block_t* current = (list_block_t*)this->heap_start;
    do {
        if (!current->allocated) {
            kout << " - Free Block at " << (unsigned int)current << ", Size: " << (unsigned int)current->next - (unsigned int)current << endl;
        }
        current = current->next;
    } while ((unsigned int)current != this->heap_start);
}

void* TreeAllocator::alloc(unsigned int req_size) {
    if constexpr (DEBUG) { kout << "Requested " << dec << req_size << " Bytes" << endl; }

    // Round to word borders + tree_block size
    unsigned int rreq_size = req_size;
    if (rreq_size < sizeof(tree_block_t) - sizeof(list_block_t)) {
        // the list_block_t is part of every block, but when freeing
        // memory we need enough space to store the rbt metadata
        rreq_size = sizeof(tree_block_t) - sizeof(list_block_t);
        if constexpr (DEBUG) { kout << " - Increased block size for rbt metadata" << endl; }
    }
    unsigned int req_size_diff = (BASIC_ALIGN - rreq_size % BASIC_ALIGN) % BASIC_ALIGN;
    rreq_size = rreq_size + req_size_diff;
    if (req_size_diff > 0) {
        if constexpr (DEBUG) { kout << " - Rounded to word border (+" << dec << req_size_diff << " bytes)" << endl; }
    }

    // Finds smallest block that is large enough
    tree_block_t* best_fit = this->rbt_search_bestfit(rreq_size);
    if (best_fit == NULL) {
        if constexpr (DEBUG) { kout << " - No block found" << endl; }
        return NULL;
    }
    if (best_fit->allocated) {
        // Something went really wrong
        if constexpr (DEBUG) { kout << " - Block already allocated :(" << endl; }
        return NULL;
    }
    best_fit->allocated = true;
    unsigned int size = this->get_size(best_fit);
    if constexpr (DEBUG) { kout << " - Found best-fit: " << hex << (unsigned int)best_fit << endl; }

    this->rbt_remove(best_fit); // BUG: On first allocation this crashes as the tree root is being removed
    if (size > HEAP_MIN_FREE_BLOCK_SIZE + rreq_size + sizeof(list_block_t)) {
        // Block can be cut
        if constexpr (DEBUG) { kout << " - Allocating " << dec << rreq_size << " Bytes with cutting" << endl; }

        // [best_fit_start | sizeof(list_block_t) | rreq_size | new_block_start]
        tree_block_t* new_block = (tree_block_t*)((char*)best_fit + sizeof(list_block_t) + rreq_size);
        new_block->allocated = false;
        this->dll_insert(best_fit, new_block);
        this->rbt_insert(new_block);
    } else {
        // Don't cut block
        // The block is already correctly positioned in the linked list so we only
        // need to remove it from the freelist, which is done for both cases
        if constexpr (DEBUG) { kout << " - Allocating " << dec << rreq_size << " Bytes without cutting" << endl; }
    }

    // Remove the old block from the freelist
    // NOTE: Originally this was placed before the leading if/else, but the red black tree code crashes
    //       when removing the tree root (NullPointerException bluescreen) so I moved the removal down.
    //       This could influence how the above rbt_insertion places the new_block, resulting in a broken
    //       freelist (unbalanced red black tree, not completely broken), but I didn't have time to fix this
    //       correctly
    // BUG: If the first allocation allocates the whole heap, this removal will crash the paging
    //      as the freelist will no longer contain any nodes, gives NullPointerException as it's not handled
    //      in the red black tree code (I think)
    // kout << " - Removing block from freelist" << endl;
    // this->rbt_remove(best_fit); // HACK

    if constexpr (DEBUG) { kout << " - Returned address " << hex << (unsigned int)((char*)best_fit + sizeof(list_block_t)) << endl; }
    return (void*)((char*)best_fit + sizeof(list_block_t));
}

void TreeAllocator::free(void* ptr) {
    if constexpr (DEBUG) { kout << "Freeing " << hex << (unsigned int)ptr << endl; }

    list_block_t* block = (list_block_t*)((char*)ptr - sizeof(list_block_t));
    if (!block->allocated) {
        // Block already free
        return;
    }
    block->allocated = false; // If the block is merged backwards afterwards this is unnecessary

    list_block_t* previous = block->previous;
    list_block_t* next = block->next;

    if (next->allocated && previous->allocated) {
        // No merge
        this->rbt_insert((tree_block_t*)block);
    }

    if (!next->allocated) {
        // Merge forward
        if constexpr (DEBUG) { kout << " - Merging forward" << endl; }

        // Remove the next block from all lists as it is now part of our freed block
        this->dll_remove(next);
        this->rbt_remove((tree_block_t*)next); // BUG: Bluescreen if next is the only block in the freelist
        if (previous->allocated) {
            // Don't insert if removed later because of backward merge
            this->rbt_insert((tree_block_t*)block);
        }
        // NOTE: Originally this was placed before the leading if (after this->dll_remove(next)),
        //       but the red black tree code crashes when removing the tree node (like on insertion).
        //       Placing this here could lead to an unbalanced freelist.
        // this->rbt_remove((tree_block_t*)next); // HACK
    }

    if (!previous->allocated) {
        // Merge backward
        if constexpr (DEBUG) { kout << " - Merging backward" << endl; }

        // Remove the current block from all lists as it is now part of the previous block
        // It doesn't have to be removed from rbt as it wasn't in there as it was allocated before
        this->dll_remove(block);
        this->rbt_remove((tree_block_t*)previous);
        this->rbt_insert((tree_block_t*)previous);  // Reinsert with new size
    }
}

unsigned int TreeAllocator::get_size(list_block_t* block) const {
    if (block->next == block) {
        // Only one block exists
        return this->heap_end - ((unsigned int)block + sizeof(list_block_t));
    }

    if ((unsigned int)block->next > (unsigned int)block) {
        // Next block is placed later in memory
        return (unsigned int)block->next - ((unsigned int)block + sizeof(list_block_t));
    }

    // Next block is placed earlier in memory which means block is at memory end
    return (unsigned int)this->heap_end - ((unsigned int)block + sizeof(list_block_t));
}
