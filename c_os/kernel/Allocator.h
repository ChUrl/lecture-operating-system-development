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
 *                    0x100000:	System-Code, kopiert nach Umschalten in      *
 *                              den Protected Mode kopiert (GRUB kann nur    *
 *                              an Adressen >1M laden)                       *
 *           Globale Variablen: Direkt nach dem Code liegen die globalen     *
 *                              Variablen.                                   *
 *                        Heap:                                              *
 *                    0x300000:	Start-Adresse der Heap-Verwaltung            *
 *                    0x400000: Letzte Adresse des Heaps                     *
 *                                                                           *
 * Achtung:         Benötigt einen PC mit mindestens 4 MB RAM!               *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 13.6.2020                        *
 *****************************************************************************/
#ifndef Allocator_include__
#define Allocator_include__

constexpr const unsigned int BASIC_ALIGN = 4;                // 32 Bit so 4 Bytes?
constexpr const unsigned int HEAP_MIN_FREE_BLOCK_SIZE = 64;  // min. Groesse eines freien Blocks

class Allocator {
public:
    Allocator(Allocator& copy) = delete;  // Verhindere Kopieren

    Allocator();

//    virtual ~Allocator() = default;

    unsigned int heap_start;
    unsigned int heap_end;
    unsigned int heap_size;
    unsigned int initialized;

    virtual void init() = 0;
    virtual void dump_free_memory() = 0;
    virtual void* alloc(unsigned int req_size) = 0;
    virtual void free(void* ptr) = 0;
};

#endif
