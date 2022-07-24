#ifndef TreeAllocator_include__
#define TreeAllocator_include__

#include "kernel/Allocator.h"
#include "user/lib/utility/Logger.h"

// I can't imagine that this is fast with all the tree logic?

typedef struct list_block {
    // Doubly linked list for every block
    bool allocated;
    struct list_block* next;
    struct list_block* previous;
} list_block_t;

// The free blocks are organized in a red-black tree to enable fast insertion with best-fit strategy.
// To allow fast merging of freed blocks every block is part of a doubly linked list.
// Because the red-black tree only contains the free blocks, the memory overhead comes
// down to 4 + 4 + 4 Bytes for the allocated flag, next and previous pointers.
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

    NamedLogger log;

    // Returns the size of the usable memory of a block
    unsigned int get_size(list_block_t* block) const;
    unsigned int get_size(tree_block_t* block) const { return get_size(reinterpret_cast<list_block_t*>(block)); }

    void dump_free_memory(tree_block_t* node);

    // NOTE: Would be nice to have this stuff somewhere else for general use (scheduling?),
    //       makes no sense to have this as members. I'll move it later
    void rbt_rot_l(tree_block_t* x);
    void rbt_rot_r(tree_block_t* x);
    void rbt_transplant(tree_block_t* a, tree_block_t* b);
    tree_block_t* rbt_minimum(tree_block_t* node);

    void rbt_insert(tree_block_t* node);
    void rbt_fix_insert(tree_block_t* k);
    void rbt_remove(tree_block_t* z);
    void rbt_fix_remove(tree_block_t* x);

    tree_block_t* rbt_search_bestfit(tree_block_t* node, unsigned int req_size);
    tree_block_t* rbt_search_bestfit(unsigned int req_size) { return rbt_search_bestfit(free_start, req_size); }

    void dll_insert(list_block_t* previous, list_block_t* node);
    void dll_insert(tree_block_t* previous, tree_block_t* node) {
        dll_insert(reinterpret_cast<list_block_t*>(previous), reinterpret_cast<list_block_t*>(node));
    }
    void dll_remove(list_block_t* node);

public:
    TreeAllocator(Allocator& copy) = delete;  // Verhindere Kopieren

    TreeAllocator() : log("RBT-Alloc") {};

//    ~TreeAllocator() override = default;

    void init() override;
    void dump_free_memory() override;
    void* alloc(unsigned int req_size) override;
    void free(void* ptr) override;
};

#endif
