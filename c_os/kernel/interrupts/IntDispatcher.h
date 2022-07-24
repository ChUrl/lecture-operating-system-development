/*****************************************************************************
 *                                                                           *
 *                         I N T D I S P A T C H E R                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Zentrale Unterbrechungsbehandlungsroutine des Systems.   *
 *                  Der Parameter gibt die Nummer des aufgetretenen          *
 *                  Interrupts an. Wenn eine Interrupt Service Routine (ISR) *
 *                  in der Map registriert ist, so wird diese aufgerufen.    *
 *                                                                           *
 * Autor:           Michael Schoettner, 30.7.16                              *
 *****************************************************************************/
#ifndef IntDispatcher_include__
#define IntDispatcher_include__

#include "kernel/interrupts/ISR.h"
#include "user/lib/Array.h"
#include "user/lib/utility/Logger.h"

class IntDispatcher {
private:
    NamedLogger log;

    enum { size = 256 };
    bse::array<ISR*, size> map;

public:
    IntDispatcher(const IntDispatcher& copy) = delete;  // Verhindere Kopieren

    // Vektor-Nummern
    enum {
        timer = 32,
        keyboard = 33,
        com1 = 36
    };

    // Initialisierung der ISR map mit einer Default-ISR.
    IntDispatcher() : log("IntDis") {
        for (ISR*& slot : map) {
            slot = nullptr;
        }
    }

    // Registrierung einer ISR. (Rueckgabewert: 0 = Erfolg, -1 = Fehler)
    int assign(unsigned int vector, ISR& isr);

    // ISR fuer 'vector' ausfuehren
    int report(unsigned int vector);
};

#endif
