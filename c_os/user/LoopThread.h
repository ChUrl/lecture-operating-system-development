/*****************************************************************************
 *                                                                           *
 *                        L O O P T H R E A D                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Demo eines Threads. Schleife die Zahlen ausgibt.         *
 *****************************************************************************/

#ifndef __loopthread_include__
#define __loopthread_include__

#include "kernel/threads/Thread.h"

class LoopThread : public Thread {
    
private:
    int id;

    LoopThread (const LoopThread &copy); // Verhindere Kopieren
    
public:
    // Gibt der Loop einen Stack und eine Id.
    LoopThread (int i) : Thread () { id = i; }

    // Zaehlt einen Zaehler hoch und gibt ihn auf dem Bildschirm aus.
    void run ();
};

#endif
