#ifndef __TreeAllocator_include__
#define __TreeAllocator_include__

#include "kernel/Allocator.h"

#define HEAP_MIN_FREE_BLOCK_SIZE 64  // min. Groesse eines freien Blocks
#define BASIC_ALIGN 8

// NOTE: I added this file

typedef struct list_block {
    // Doubly linked list for every block
    bool allocated;
    struct list_block* next;
    struct list_block* previous;
} list_block_t;

// Format eines freien Blocks
// The free blocks are organized in a red-black tree to enable fast insertion with best-fit strategy.
// To allow fast merging of freed blocks every block is part of a doubly linked list.
// Because the red-black tree only contains the free blocks, the memory overhead comes
// down to only 4 + 4 + 4 Bytes for the allocated flag, next and previous pointers.
// The size can be calculated by using the next pointer so it doesn't have to be stored.
typedef struct tree_block {
    // Doubly linked list for every block
    // Locate this at the beginning so we can just cast to allocated_block_t and overwrite the rbt data
    bool allocated;
    struct list_block* next;
    struct list_block* previous;
    // RB tree for free blocks
    struct tree_block* left;
    struct tree_block* right;
    struct tree_block* parent;
    bool red;  //  RB tree node color
} tree_block_t;

class TreeAllocator : Allocator {

private:
    // Root of the rbt
    tree_block_t* free_start;

    TreeAllocator(Allocator& copy);  // Verhindere Kopieren

    // Returns the size of the usable memory of a block
    unsigned int get_size(list_block_t* block) const;
    unsigned int get_size(tree_block_t* block) const { return this->get_size((list_block_t*)block); }

    void dump_free_memory(tree_block_t* node);

    // NOTE: Would be nice to have this stuff somewhere else for general use,
    //       but that would require different rbt_node/dll_node structures.
    //       If I need this again later I should look into it.
    void rbt_rot_l(tree_block_t* x);
    void rbt_rot_r(tree_block_t* x);
    void rbt_transplant(tree_block_t* a, tree_block_t* b);
    tree_block_t* rbt_minimum(tree_block_t* node);

    void rbt_insert(tree_block_t* node);
    void rbt_fix_insert(tree_block_t* k);
    void rbt_remove(tree_block_t* z);
    void rbt_fix_remove(tree_block_t* x);

    tree_block_t* rbt_search_bestfit(tree_block_t* node, unsigned int req_size);
    tree_block_t* rbt_search_bestfit(unsigned int req_size) { return this->rbt_search_bestfit(this->free_start, req_size); }

    void dll_insert(list_block_t* previous, list_block_t* node);
    void dll_insert(tree_block_t* previous, tree_block_t* node) { this->dll_insert((list_block_t*)previous, (list_block_t*)node); }
    void dll_remove(list_block_t* node);

public:
    TreeAllocator() {};

    void init();
    void dump_free_memory();
    void* alloc(unsigned int req_size);
    void free(void* ptr);
};

#endif
