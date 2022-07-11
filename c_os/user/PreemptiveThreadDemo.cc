#include "user/PreemptiveThreadDemo.h"
#include "lib/Semaphore.h"

void PreemptiveThreadDemo::run() {
    kout << "PreemptiveThreadDemo ====================================" << endl;
    kout << "Initializing Semaphore" << endl;
    Semaphore* sem = new Semaphore(1); // Create this semaphore on the heap as this thread exits itself,
                                       // so stack allocated objects will be lost

    kout << "Allocating LoopThread A" << endl;
    Thread* cntA = new PreemptiveLoopThread(0, sem);
    kout << "Allocating LoopThread B" << endl;
    Thread* cntB = new PreemptiveLoopThread(1, sem);
    kout << "Allocating LoopThread C" << endl;
    Thread* cntC = new PreemptiveLoopThread(2, sem);

    kout << "Adding threads to ready queue" << endl;
    scheduler.ready(cntA);
    scheduler.ready(cntB);
    scheduler.ready(cntC);

    kout << "Exiting main thread" << endl;
    scheduler.exit();
}
