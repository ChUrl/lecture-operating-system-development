/*****************************************************************************
 *                                                                           *
 *                            A L L O C A T O R                              *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Einfache Speicherverwaltung. 'new' und 'delete' werden   *
 *                  durch Ueberladen der entsprechenden Operatoren           *
 *                  realisiert.                                              *
 *                                                                           *
 * Memory-Laylout                                                            *
 *                                                                           *
 *                  boot.asm                                                 *
 *                      0x07c0: Bootsector vom BIOS geladen                  *
 *                      0x0060: Boot-Code verschiebt sich hier hin           *
 *                      0x9000: Setup-Code (max. 64K inkl. Stack) vom        *
 *                              Bootsector-Code geladen                      *
 *                  setup.asm                                                *
 *                      0x1000: System-Code (max. 512K) geladen              *
 *                  System-Code                                              *
 *                    0x100000: System-Code, kopiert nach Umschalten in      *
 *                              den Protected Mode kopiert (GRUB kann nur    *
 *                              an Adressen >1M laden)                       *
 *           Globale Variablen: Direkt nach dem Code liegen die globalen     *
 *                              Variablen.                                   *
 *                        Heap:                                              *
 *                    0x300000: Start-Adresse der Heap-Verwaltung            *
 *                    0x400000: Letzte Adresse des Heaps                     *
 *                                                                           *
 * Achtung:         Benötigt einen PC mit mindestens 8 MB RAM!               *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 1.3.2022                        *
 *****************************************************************************/

#include "kernel/Allocator.h"
#include "kernel/Globals.h"
#include <stddef.h>

#define MEM_SIZE_DEF 8 * 1024 * 1024  // Groesse des Speichers = 8 MB

#define HEAP_START 0x300000    // Startadresse des Heaps
#define HEAP_SIZE 1024 * 1024  // Default-Groesse des Heaps, falls \
                               // nicht über das BIOS ermittelbar

/*****************************************************************************
 * Konstruktor:     Allocator::Allocator                                     *
 *****************************************************************************/
Allocator::Allocator() : heap_start(HEAP_START), heap_end(HEAP_START + HEAP_SIZE), heap_size(HEAP_SIZE), initialized(1) {
    // Groesse des Hauptspeichers (kann über das BIOS abgefragt werden,
    // aber sehr umstaendlich, daher hier fest eingetragen
    total_mem = MEM_SIZE_DEF;
}

/*****************************************************************************
 * Nachfolgend sind die Operatoren von C++, die wir hier ueberschreiben      *
 * und entsprechend 'mm_alloc' und 'mm_free' aufrufen.                       *
 *****************************************************************************/
void* operator new(size_t size) {
    return allocator.alloc(size);
}

void* operator new[](size_t count) {
    return allocator.alloc(count);
}

void operator delete(void* ptr) {
    allocator.free(ptr);
}

void operator delete[](void* ptr) {
    allocator.free(ptr);
}

void operator delete(void* ptr, unsigned int sz) {
    allocator.free(ptr);
}
