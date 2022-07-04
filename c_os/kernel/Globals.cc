/*****************************************************************************
 *                                                                           *
 *                               G L O B A L S                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Globale Variablen des Systems.                           *
 *                                                                           *
 * Autor:           Michael Schoettner, 30.7.16                              *
 *****************************************************************************/

#include "kernel/Globals.h"

CPU cpu;                 // CPU-spezifische Funktionen
PCSPK pcspk;             // PC-Lautsprecher
CGA_Stream kout;         // Ausgabe-Strom fuer Kernel
Keyboard kb;             // Tastatur
IntDispatcher intdis;    // Unterbrechungsverteilung
PIC pic;                 // Interrupt-Controller
unsigned int total_mem;  // RAM total
unsigned long systime = 0;
// BumpAllocator         allocator;
// LinkedListAllocator allocator;
TreeAllocator allocator;
Scheduler scheduler;
BIOS bios;  // Schnittstelle zum 16-Bit BIOS
VESA vesa;  // VESA-Treiber
PIT pit(10000);
