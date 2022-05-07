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

    this->allocations = 0;
    this->next = (unsigned char*)heap_start;

    kout << "Initialized Bump Allocator" << endl
         << "Heap Start: " << hex << this->heap_start << ", Heap End: " << this->heap_end << endl;
    kout << endl;
}

/*****************************************************************************
 * Methode:         BumpAllocator::dump_free_memory                          *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Ausgabe der Freispeicherinfos. Zu Debuggingzwecken.      *
 *****************************************************************************/
void BumpAllocator::dump_free_memory() {

    /* Hier muess Code eingefuegt werden */

    kout << "Freier Speicher:" << endl
         << "Next: " << hex << this->next << ", Allocations: " << dec << this->allocations << endl;
    kout << endl;
}

/*****************************************************************************
 * Methode:         BumpAllocator::alloc                                     *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einen neuen Speicherblock allozieren.                    * 
 *****************************************************************************/
void* BumpAllocator::alloc(unsigned int req_size) {

    /* Hier muess Code eingefuegt werden */

    kout << "Requested " << dec << req_size << " Bytes" << endl;

    if (req_size > this->heap_end - (unsigned int)this->next) {
        kout << "More memory requested than available :(" << endl;
        kout << endl;
        return NULL;
    }

    void* allocated = this->next;
    this->next = this->next + req_size;
    this->allocations = this->allocations + 1;

    kout << "Allocated " << dec << req_size << " Bytes." << endl;
    kout << endl;

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