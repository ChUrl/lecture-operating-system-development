/*****************************************************************************
 *                                                                           *
 *                        B U M P A L L O C A T O R                          *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Eine sehr einfache Heap-Verwaltung, welche freigegebenen *
 *                  Speicher nicht mehr nutzen kann.                         *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 3.3.2022                        *
 *****************************************************************************/

#ifndef __BumpAllocator_include__
#define __BumpAllocator_include__

#include "kernel/Allocator.h"
#include "user/lib/Logger.h"

class BumpAllocator : Allocator {

private:
    unsigned char* next;
    unsigned int allocations;

    NamedLogger log;

    BumpAllocator(Allocator& copy) = delete;  // Verhindere Kopieren

public:
    BumpAllocator() : log("BMP-Alloc") {};  // Allocator() called implicitely in C++

    void init() override;
    void dump_free_memory() override;
    void* alloc(unsigned int req_size) override;
    void free(void* ptr) override;
};

#endif
