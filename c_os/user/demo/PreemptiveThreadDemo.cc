#include "user/demo/PreemptiveThreadDemo.h"

Semaphore PreemptiveLoopThread::sem(1);

void PreemptiveLoopThread::run() {
    int cnt = 0;

    while (true) {
        // Basic synchronization by semaphore
        sem.p();

        // Saving + restoring kout position doesn't help much as preemption still occurs
        kout.setpos(55, this->id);
        kout << fillw(3) << this->id << fillw(0) << ": " << dec << cnt++ << endl;

        sem.v();
    }
}

void PreemptiveThreadDemo::run() {
    kout.clear();

    kout << "Preemptive Thread Demo:" << endl;

    kout << "Readying LoopThreads" << endl;
    for (unsigned int i = 0; i < this->number_of_threads; ++i) {
        scheduler.ready<PreemptiveLoopThread>(i);
    }

    kout << "Exiting main thread" << endl;
    scheduler.exit();
}
