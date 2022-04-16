/*****************************************************************************
 *                                                                           *
 *                         I N T D I S P A T C H E R                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Zentrale Unterbrechungsbehandlungsroutine des Systems.   *
 *                  Der Parameter gibt die Nummer des aufgetretenen          *
 *                  Interrupts an. Wenn eine Interrupt Service Routine       *
 *                  registriert ist, wird diese aufgerufen.                  *
 *                                                                           *
 * Autor:           Michael Schoettner, 30.7.16                              *
 *****************************************************************************/


extern "C" void int_disp (unsigned int slot);


// Low-Level Interrupt-Behandlung. (Die Funktion wird spaeter noch erweitert)
void int_disp (unsigned int slot) {
}

