/*****************************************************************************
 *                                                                           *
 *                                B I O S                                    *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    BIOS-Schnittstelle                                       *
 *                                                                           *
 * Autor:           Michael Schoettner, 29.11.2018                           *
 *****************************************************************************/

#include "kernel/BIOS.h"
#include "kernel/Globals.h"

// 16-Bit Code aufrufen, siehe Konstruktor und Aufruf in startup.asm
extern "C" {
    void bios_call();
}

// in startup.asm im GDT-Eintrag so festgeschrieben!
constexpr const unsigned int BIOS16_CODE_MEMORY_START = 0x24000;

// Parameter fuer BIOS-Aufrufe (Register)
constexpr const unsigned int BIOS16_PARAM_BASE = 0x26000;

// Zeiger auf Speichbereich fuer Parameter fuer BIOS-Aufruf (siehe BIOS.h)
BIOScall_params* BC_params = reinterpret_cast<BIOScall_params*>(BIOS16_PARAM_BASE);

/*****************************************************************************
 * Methode:         BIOS::BIOS                                               *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Konstruktor. Baut manuell ein 16-Bit Code Segment fuer   * 
 *                  den BIOS-Aufruf. Startadresse dieser Funktion steht      *
 *                  im 4. GDT-Eintrag (siehe startup.asm).                   *
 *****************************************************************************/
BIOS::BIOS() {
    unsigned char* codeAddr = reinterpret_cast<unsigned char*>(BIOS16_CODE_MEMORY_START);

    // mov eax, 25000 (Adresse wohin aktuelles esp gesichert wird)
    *codeAddr = 0x66;
    codeAddr++;
    *codeAddr = 0xB8;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;
    *codeAddr = 0x50;
    codeAddr++;
    *codeAddr = 0x02;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;

    // mov [eax], esp (esp abspeichern)
    *codeAddr = 0x66;
    codeAddr++;
    *codeAddr = 0x67;
    codeAddr++;
    *codeAddr = 0x89;
    codeAddr++;
    *codeAddr = 0x20;
    codeAddr++;

    // mov eax,cr0 (cr0 auslesen)
    *codeAddr = 0x0F;
    codeAddr++;
    *codeAddr = 0x20;
    codeAddr++;
    *codeAddr = 0xC0;
    codeAddr++;

    // and eax, 7FFEFFFE   (Bitmaske zum Abschlaten des Protected-Mode)
    *codeAddr = 0x66;
    codeAddr++;
    *codeAddr = 0x25;
    codeAddr++;
    *codeAddr = 0xFE;
    codeAddr++;
    *codeAddr = 0xFF;
    codeAddr++;
    *codeAddr = 0xFE;
    codeAddr++;
    *codeAddr = 0x7F;
    codeAddr++;

    // mov cr0, eax (cr0 syetzen um den Protected-Mode auszuschalten)
    *codeAddr = 0x0F;
    codeAddr++;
    *codeAddr = 0x22;
    codeAddr++;
    *codeAddr = 0xC0;
    codeAddr++;

    // jmp 2400:001B Instruktions-Pipeline leeren und Dekodierungseinheit auf 16-Bit code umschalten
    // Wir springen hier zur naechsten Instruktion (*)
    // 2400:001B (2400<<4 = 24000 + 1B)
    *codeAddr = 0xEA;
    codeAddr++;
    *codeAddr = 0x1B;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;
    *codeAddr = 0x24;
    codeAddr++;

    // (*) mov dx,2400 (Lade 0x2400 nach dx (fuer Parameter-Zugriff -> BIOS16_PARAM_BAS)
    *codeAddr = 0xBA;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;
    *codeAddr = 0x24;
    codeAddr++;

    // mov ss,dx (Lade Stack-Segment-Register)
    *codeAddr = 0x8E;
    codeAddr++;
    *codeAddr = 0xD2;
    codeAddr++;

    // mov gs,dx
    *codeAddr = 0x8E;
    codeAddr++;
    *codeAddr = 0xEA;
    codeAddr++;

    // mov esp,2000 -> BIOS16_PARAM_BASE 0x260000 (= 0x2400:2000)
    *codeAddr = 0x66;
    codeAddr++;
    *codeAddr = 0xBC;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;
    *codeAddr = 0x20;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;

    // Register laden (stehen in BIOS16_PARAM_BASE, ab 0x260000)
    // (pop erhöht die Adressen)

    // pop ds
    *codeAddr = 0x1F;
    codeAddr++;

    // pop es
    *codeAddr = 0x07;
    codeAddr++;

    // pop fs
    *codeAddr = 0x0f;
    codeAddr++;
    *codeAddr = 0xa1;
    codeAddr++;

    // pop ax
    *codeAddr = 0x58;
    codeAddr++;

    // popad
    *codeAddr = 0x66;
    codeAddr++;
    *codeAddr = 0x61;
    codeAddr++;

    // int(nr)
    *codeAddr = 0xCD;
    codeAddr++;  // 'int' Instruktion
    *codeAddr = 0x00;
    codeAddr++;  // Nummer (wird direkt von BIOS::Int direkt hier reingeschrieben)

    // Register speichern in BIOS16_PARAM_BASE (ab 0x260000)

    // pushad
    *codeAddr = 0x66;
    codeAddr++;
    *codeAddr = 0x60;
    codeAddr++;

    // pushf
    *codeAddr = 0x9C;
    codeAddr++;

    // push fs
    *codeAddr = 0x0f;
    codeAddr++;
    *codeAddr = 0xa0;
    codeAddr++;

    // push es
    *codeAddr = 0x06;
    codeAddr++;

    // push ds
    *codeAddr = 0x1E;
    codeAddr++;

    // mov eax,cr0
    *codeAddr = 0x0F;
    codeAddr++;
    *codeAddr = 0x20;
    codeAddr++;
    *codeAddr = 0xC0;
    codeAddr++;

    // or eax, 00010001 (protected mode without paging)
    *codeAddr = 0x66;
    codeAddr++;
    *codeAddr = 0x0D;
    codeAddr++;
    *codeAddr = 0x01;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;
    *codeAddr = 0x01;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;

    // mov cr0, eax
    *codeAddr = 0x0F;
    codeAddr++;
    *codeAddr = 0x22;
    codeAddr++;
    *codeAddr = 0xC0;
    codeAddr++;

    // jmp 0018:0049, flush pipeline & switch decoding (active 32 Bit PM)
    // 0018:0049
    *codeAddr = 0xEA;
    codeAddr++;
    *codeAddr = 0x49;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;
    *codeAddr = 0x18;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;

    // mov dx,0010
    *codeAddr = 0xBA;
    codeAddr++;
    *codeAddr = 0x10;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;

    // mov ds,dx
    *codeAddr = 0x8E;
    codeAddr++;
    *codeAddr = 0xDA;
    codeAddr++;

    // mov es,dx
    *codeAddr = 0x8E;
    codeAddr++;
    *codeAddr = 0xC2;
    codeAddr++;

    // mov es,dx
    *codeAddr = 0x8E;
    codeAddr++;
    *codeAddr = 0xE2;
    codeAddr++;

    // mov fs,dx
    *codeAddr = 0x8E;
    codeAddr++;
    *codeAddr = 0xEA;
    codeAddr++;

    // mov ss,dx
    *codeAddr = 0x8E;
    codeAddr++;
    *codeAddr = 0xD2;
    codeAddr++;

    // mov eax, 25000
    *codeAddr = 0x66;
    codeAddr++;
    *codeAddr = 0xB8;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;
    *codeAddr = 0x50;
    codeAddr++;
    *codeAddr = 0x02;
    codeAddr++;
    *codeAddr = 0x00;
    codeAddr++;

    // mov esp, [eax]
    *codeAddr = 0x66;
    codeAddr++;
    *codeAddr = 0x67;
    codeAddr++;
    *codeAddr = 0x8B;
    codeAddr++;
    *codeAddr = 0x20;
    codeAddr++;

    // far ret
    *codeAddr = 0x66;
    codeAddr++;
    *codeAddr = 0xCB;
}

/*****************************************************************************
 * Methode:         BIOS::Int                                                *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Fuehrt einen BIOS-Aufruf per Software-Interrupt durch.   *
 *****************************************************************************/
void BIOS::Int(int inter) {
    unsigned char* ptr = reinterpret_cast<unsigned char*>(BIOS16_CODE_MEMORY_START);

    // Interrupt-Nummer in 16-Bit Code-Segment schreiben (unschoen, aber ...)
    *(ptr + 48) = static_cast<unsigned char>(inter);

    CPU::disable_int();  // Interrupts abschalten
    bios_call();
    CPU::enable_int();
}
