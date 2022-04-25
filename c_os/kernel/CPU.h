/*****************************************************************************
 *                                                                           *
 *                                   C P U                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung einer Abstraktion fuer den Prozessor.    *
 *                                                                           *
 * Autor:           Michael Schoettner, 30.7.16                              *
 *****************************************************************************/
#ifndef __CPU_include__
#define __CPU_include__

class CPU {

private:
    CPU(const CPU& copy);  // Verhindere Kopieren

public:
    CPU() {}

    // Time-Stamp-Counter auslesen
    inline unsigned long long int rdtsc() {
        unsigned long long int ret;
        asm volatile("rdtsc"
                     : "=A"(ret));
        return ret;
    }
};

#endif
