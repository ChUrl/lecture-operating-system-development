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

CPU cpu;          // CPU-spezifische Funktionen
CGA_Stream kout;  // Ausgabe-Strom fuer Kernel
BIOS bios;        // Schnittstelle zum 16-Bit BIOS
VESA vesa;        // VESA-Treiber

PIC pic;               // Interrupt-Controller
IntDispatcher intdis;  // Unterbrechungsverteilung
PIT pit(10000);        // 10000
PCSPK pcspk;           // PC-Lautsprecher

Keyboard kb;  // Tastatur
KeyEventManager kevman;

// BumpAllocator allocator;
LinkedListAllocator allocator;
// TreeAllocator allocator;

Scheduler scheduler;

SerialOut serial;

unsigned int total_mem;  // RAM total
unsigned long systime = 0;
