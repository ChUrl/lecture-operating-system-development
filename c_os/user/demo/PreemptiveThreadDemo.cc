#include "user/demo/PreemptiveThreadDemo.h"

void PreemptiveLoopThread::run() {
    int cnt = 0;

    while (true) {
        // Basic synchronization by semaphore
        // NOTE: I placed the semaphore inside the CGA_Stream so multiple demos can synchronize, not
        //       only this one. This is optional so disruptions can still occur because of preemption
        kout.lock();

        // Saving + restoring kout position doesn't help much as preemption still occurs
        kout.setpos(55, this->id);
        kout << fillw(3) << this->id << fillw(0) << ": " << dec << cnt++ << endl;

        kout.unlock();
    }
}

void PreemptiveThreadDemo::run() {
    kout << "Preemptive Thread Demo" << endl;
    Thread* threads[this->number_of_threads];
    kout << "Allocating LoopThreads" << endl;
    for (unsigned int i = 0; i < this->number_of_threads; ++i) {
        threads[i] = new PreemptiveLoopThread(i);
    }

    kout << "Adding threads to ready queue" << endl;
    for (unsigned int i = 0; i < this->number_of_threads; ++i) {
        scheduler.ready(threads[i]);
    }

    kout << "Exiting main thread" << endl;
    scheduler.exit();
}
