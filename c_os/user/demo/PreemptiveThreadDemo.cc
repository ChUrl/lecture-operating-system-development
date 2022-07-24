#include "user/demo/PreemptiveThreadDemo.h"

void PreemptiveLoopThread::run() {
    int cnt = 0;

    while (running) {
        // Basic synchronization by semaphore
        kout.lock();

        // Saving + restoring kout position doesn't help much as preemption still occurs
        CGA_Stream::setpos(55, id);
        kout << fillw(3) << id << fillw(0) << ": " << dec << cnt++ << endl;

        kout.unlock();
    }

    scheduler.exit();
}

void PreemptiveThreadDemo::run() {
    kout.lock();
    kout.clear();

    kout << "Preemptive Thread Demo:" << endl;

    kout << "Readying LoopThreads" << endl;
    for (unsigned int i = 0; i < number_of_threads; ++i) {
        scheduler.ready<PreemptiveLoopThread>(i);
    }

    kout << "Exiting main thread" << endl;
    kout.unlock();
    scheduler.exit();
}
