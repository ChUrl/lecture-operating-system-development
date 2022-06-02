/*****************************************************************************
 *                                                                           *
 *                          S C H E D U L E R                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines einfachen Zeitscheiben-Schedulers. *
 *                  Rechenbereite Threads werden in 'readQueue' verwaltet.   *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 22.8.2016                      *
 *****************************************************************************/

#ifndef __Scheduler_include__
#define __Scheduler_include__

#include "kernel/threads/Dispatch.h"
#include "kernel/threads/Thread.h"
#include "lib/Queue.h"

class Scheduler : public Dispatcher {
    
private:
    Scheduler (const Scheduler &copy); // Verhindere Kopieren
     
private:
    Queue readyQueue;   // auf die CPU wartende Threads
   
public:
    Scheduler () {}

    // Scheduler starten
    void schedule ();
    
    // Thread in readyQueue eintragen
    void ready (Thread * that);
    
    // Thread terminiert sich selbst
    void exit ();

    // Thread mit 'Gewalt' terminieren
    void kill (Thread * that);

    // CPU freiwillig abgeben und Auswahl des naechsten Threads
    void yield ();
};

#endif
