/*****************************************************************************
 *                                                                           *
 *                                   I S R                                   *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Interrupt Service Routine. Jeweils ein Objekt pro ISR.   *
 *                  Erlaubt es einen Kontext mit Variablen fuer die Unter-   *
 *                  brechungsroutine bereitzustellen.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, 06.04.20                             *
*****************************************************************************/
#ifndef ISR_include__
#define ISR_include__

class ISR {
public:
    ISR(const ISR& copy) = delete;  // Verhindere Kopieren

//    virtual ~ISR() = default;

    ISR() = default;

    // Unterbrechungsbehandlungsroutine
    virtual void trigger() = 0;
};

#endif
