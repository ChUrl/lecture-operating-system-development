/*****************************************************************************
 *                                                                           *
 *                                   C P U                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung einer Abstraktion fuer den Prozessor.    *
 *                  Derzeit wird nur angeboten, Interrupts zuzulassen, zu    *
 *                  verbieten oder den Prozessor anzuhalten.                 *
 *                                                                           *
 * Autor:           Michael Schoettner, 30.7.16                              *
 *****************************************************************************/
#ifndef CPU_include__
#define CPU_include__

class CPU {
public:
    CPU(const CPU& copy) = delete;  // Verhindere Kopieren

    CPU() = default;

    // Erlauben von (Hardware-)Interrupts
    static inline void enable_int() {
        asm volatile("sti");
    }

    // Interrupts werden ignoriert/verboten
    static inline void disable_int() {
        asm volatile("cli");
    }

    // Prozessor bis zum naechsten Interrupt anhalten
    static inline void idle() {
        asm volatile("sti;"
                     "hlt");
    }

    // Prozessor anhalten
    static inline void halt() {
        asm volatile("cli;"
                     "hlt");
    }

    // Time-Stamp-Counter auslesen
    static inline unsigned long long int rdtsc() {
        unsigned long long int ret;
        asm volatile("rdtsc"
                     : "=A"(ret));
        return ret;
    }
};

#endif
