#include "user/demo/PreemptiveThreadDemo.h"

void PreemptiveLoopThread::run() {
    int cnt = 0;

    while (true) {
        // Basic synchronization by semaphore
        // NOTE: I placed the semaphore inside the CGA_Stream so multiple demos can synchronize, not
        //       only this one. This is optional so disruptions can still occur because of preemption
        //       (I only use this for the user output (demos), anywhere else could become problematic
        //        quickly...)
        kout.lock();

        // Saving + restoring kout position doesn't help much as preemption still occurs
        kout.setpos(55, this->id);
        kout << fillw(3) << this->id << fillw(0) << ": " << dec << cnt++ << endl;

        kout.unlock();
    }
}

void PreemptiveThreadDemo::run() {
    kout << "Preemptive Thread Demo" << endl;

    kout << "Readying LoopThreads" << endl;
    for (unsigned int i = 0; i < this->number_of_threads; ++i) {
        threads.push_back(scheduler.ready<PreemptiveLoopThread>(i));
    }

    kout << "Exiting main thread" << endl;
    while (listener.waitForKeyEvent() != 'L') {}
    scheduler.exit();
}
