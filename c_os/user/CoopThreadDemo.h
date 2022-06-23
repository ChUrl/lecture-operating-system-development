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

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class CoopThreadDemo : public Thread {

private:
    CoopThreadDemo(const CoopThreadDemo& copy) = delete;  // Verhindere Kopieren

public:
    // Gib dem Anwendungsthread einen Stack.
    CoopThreadDemo() {
        kout << "Initialized CoopThreadDemo" << endl;
    }

    // Thread-Startmethode
    void run() override;
};

#endif
