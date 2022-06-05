/*****************************************************************************
 *                                                                           *
 *             C O O P E R A T I V E T H R E A D D E M O                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Beispiel für kooperative Threads.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 21.8.2016                       *
 *****************************************************************************/
#ifndef __coopthreaddemo_include__
#define __coopthreaddemo_include__


#include "kernel/threads/Thread.h"


class CoopThreadDemo : public Thread {
    
private:
    CoopThreadDemo (const CoopThreadDemo &copy); // Verhindere Kopieren

public:
    // Gib dem Anwendungsthread einen Stack.
    CoopThreadDemo () : Thread () { }

    // Thread-Startmethode
    void run ();

 };

#endif
