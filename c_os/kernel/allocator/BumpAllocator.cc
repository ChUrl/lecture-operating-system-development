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

#include "kernel/Globals.h"
#include <stddef.h>
// #include "kernel/allocator/LinkedListAllocator.h"

/*****************************************************************************
 * Methode:         BumpAllocator::init                                      *
 *---------------------------------------------------------------------------*
 * Beschreibung:    BumpAllokartor intitialisieren.                          *
 *****************************************************************************/
void BumpAllocator::init() {

    /* Hier muess Code eingefuegt werden */

    this->heap_start = 3 * 1024 * 1024;                   // 3 MB
    this->heap_size = 1 * 1024 * 1024;                    // 1 MB
    this->heap_end = this->heap_start + this->heap_size;  // 4 MB
    this->allocations = 0;
    this->next = (unsigned char*)heap_start;

    this->initialized = true;
}

/*****************************************************************************
 * Methode:         BumpAllocator::dump_free_memory                          *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Ausgabe der Freispeicherinfos. Zu Debuggingzwecken.      *
 *****************************************************************************/
void BumpAllocator::dump_free_memory() {

    /* Hier muess Code eingefuegt werden */
    kout << "Freier Speicher:" << endl
         << "    Heap Start: " << this->heap_start << ", Heap End: " << this->heap_end << endl
         << "    Next: " << this->next << ", Allocations: " << this->allocations << endl;
}

/*****************************************************************************
 * Methode:         BumpAllocator::alloc                                     *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einen neuen Speicherblock allozieren.                    * 
 *****************************************************************************/
void* BumpAllocator::alloc(unsigned int req_size) {

    /* Hier muess Code eingefuegt werden */

    if (!this->initialized) {
        kout << "Allocator not initialized :(" << endl;
        return NULL;
    }

    if (req_size > this->heap_end - (unsigned int)this->next) {
        kout << "More memory requested than available :(" << endl;
        return NULL;
    }

    void* allocated = this->next;
    this->next = this->next + req_size;
    this->allocations = this->allocations + 1;

    return allocated;
}

/*****************************************************************************
 * Methode:         BumpAllocator::free                                      *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Nicht implementiert.                                     *
 *****************************************************************************/
void BumpAllocator::free(void* ptr) {
    kout << "   mm_free: ptr= " << hex << (unsigned int)ptr << ", not supported" << endl;
}
