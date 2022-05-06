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
#include "kernel/allocator/BumpAllocator.h"
#include "kernel/CPU.h"

extern CPU cpu;                 // CPU-spezifische Funktionen
extern PCSPK pcspk;             // PC-Lautsprecher
extern CGA_Stream kout;         // Ausgabe-Strom fuer Kernel
extern Keyboard kb;             // Tastatur
extern unsigned int total_mem;  // RAM total
extern BumpAllocator allocator;
//extern LinkedListAllocator   allocator;

#endif
