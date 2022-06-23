/*****************************************************************************
 *                                                                           *
 *                        L O O P T H R E A D                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Demo eines Threads. Schleife die Zahlen ausgibt.         *
 *****************************************************************************/

#include "user/LoopThread.h"
#include "kernel/Globals.h"

/*****************************************************************************
 * Methode:         LoopThread::run                                          *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Code des Threads.                                        *
 *****************************************************************************/
void LoopThread::run() {

    /* Hier muss Code eingefuegt werden */

    int cnt = 0;
    while (true) {
        kout.setpos(55, this->id);
        kout << this->id << ": " << dec << cnt++ << endl;
        scheduler.yield();
    }
}
