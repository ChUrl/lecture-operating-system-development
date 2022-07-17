/*****************************************************************************
 *                                                                           *
 *                               G L O B A L S                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Globale Variablen des Systems.                           *
 *                                                                           *
 * Autor:           Michael Schoettner, 30.7.16                              *
 *****************************************************************************/
#ifndef __Globals_include__
#define __Globals_include__

#include "devices/CGA_Stream.h"
#include "devices/Keyboard.h"
#include "devices/PCSPK.h"
#include "devices/PIT.h"
#include "devices/VESA.h"
#include "kernel/allocator/BumpAllocator.h"
#include "kernel/allocator/LinkedListAllocator.h"
#include "kernel/allocator/TreeAllocator.h"
#include "kernel/BIOS.h"
#include "kernel/CPU.h"
#include "kernel/interrupts/IntDispatcher.h"
#include "kernel/interrupts/PIC.h"
#include "kernel/threads/Scheduler.h"
#include "user/devices/SerialOut.h"
#include "user/event/KeyEventManager.h"

extern CPU cpu;          // CPU-spezifische Funktionen
extern CGA_Stream kout;  // Ausgabe-Strom fuer Kernel
extern BIOS bios;        // Schnittstelle zum 16-Bit BIOS
extern VESA vesa;        // VESA-Treiber

extern PIC pic;               // Interrupt-Controller
extern IntDispatcher intdis;  // Unterbrechungsverteilung
extern PIT pit;               // Zeitgeber

extern PCSPK pcspk;  // PC-Lautsprecher

extern Keyboard kb;  // Tastatur
extern KeyEventManager kevman;

// extern BumpAllocator allocator;
extern LinkedListAllocator allocator;
// extern TreeAllocator allocator;

extern Scheduler scheduler;

extern SerialOut serial;

extern unsigned int total_mem;  // RAM total
extern unsigned long systime;   // wird all 10ms hochgezaehlt

#endif
