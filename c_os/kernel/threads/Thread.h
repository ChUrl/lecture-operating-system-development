/*****************************************************************************
 *                                                                           *
 *                                 T H R E A D                               *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Implementierung eines kooperativen Thread-Konzepts.      *
 *                  Thread-Objekte werden vom Scheduler in einer verketteten *
 *                  Liste 'readylist' verwaltet.                             *
 *                                                                           *
 *                  Im Konstruktor wird der initialie Kontext des Threads    * 
 *                  eingerichtet. Mit 'start' wird ein Thread aktiviert.     *
 *                  Die CPU sollte mit 'yield' freiwillig abgegeben werden.  * 
 *                  Um bei einem Threadwechsel den Kontext sichern zu        *
 *                  koennen, enthaelt jedes Threadobjekt eine Struktur       *
 *                  ThreadState, in dem die Werte der nicht-fluechtigen      *
 *                  Register gesichert werden koennen.                       *
 *                                                                           *
 * Autor:           Michael, Schoettner, HHU, 16.12.2016                     *
 *****************************************************************************/

#ifndef __Thread_include__
#define __Thread_include__

#include "kernel/threads/ThreadState.h"
#include "lib/Chain.h"

class Thread : public Chain {
    
private:
    Thread(const Thread &copy); // Verhindere Kopieren

private:
    struct ThreadState regs;
    unsigned int *stack;
   
public:
    unsigned int tid;       // Thread-ID (wird im Konstruktor vergeben)

    Thread () { }
    
    // Thread aktivieren
    void start ();

    // Umschalten auf Thread 'next'
    void switchTo (Thread& next);

    // Methode des Threads, muss in Sub-Klasse implementiert werden
    virtual void run () = 0;

    ~Thread ();

 };

#endif
