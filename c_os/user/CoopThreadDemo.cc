/*****************************************************************************
 *                                                                           *
 *             C O O P E R A T I V E T H R E A D D E M O                     *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Beispiel für kooperative Threads.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 21.8.2016                       *
 *****************************************************************************/

#include "user/CoopThreadDemo.h"
#include "kernel/Globals.h"
#include "user/LoopThread.h"

/*****************************************************************************
 * Methode:         CoopThreadDemo::run                                      *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Der Anwendungsthread erzeugt drei Threads die Zaehler    *
 *                  ausgeben und terminiert sich selbst.                     *
 *****************************************************************************/
void CoopThreadDemo::run() {

    /* Hier muss Code eingefuegt werden */

    kout << "Allocating LoopThread A" << endl;
    Thread* cntA = new LoopThread(0);
    kout << "Allocating LoopThread B" << endl;
    Thread* cntB = new LoopThread(1);
    kout << "Allocating LoopThread C" << endl;
    Thread* cntC = new LoopThread(2);

    kout << "Adding threads to ready queue" << endl;
    scheduler.ready(cntA);
    scheduler.ready(cntB);
    scheduler.ready(cntC);

    int cnt = 0;
    while (cnt++ < 1000) {
        scheduler.yield();
    }

    kout << "Killing LoopThread A" << endl;
    scheduler.kill(cntA);

    kout << "Exiting main thread" << endl;
    scheduler.exit();
}
