#include "user/PreemptiveThreadDemo.h"

void PreemptiveThreadDemo::run() {
    kout << "Allocating LoopThread A" << endl;
    Thread* cntA = new PreemptiveLoopThread(0);
    kout << "Allocating LoopThread B" << endl;
    Thread* cntB = new PreemptiveLoopThread(1);
    kout << "Allocating LoopThread C" << endl;
    Thread* cntC = new PreemptiveLoopThread(2);

    kout << "Adding threads to ready queue" << endl;
    scheduler.ready(cntA);
    scheduler.ready(cntB);
    scheduler.ready(cntC);

    kout << "Exiting main thread" << endl;
    scheduler.exit();
}
