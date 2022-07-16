#include "user/demo/PreemptiveThreadDemo.h"

void PreemptiveLoopThread::run() {
    int cnt = 0;

    while (true) {
        // Basic synchronization by semaphore
        sem->p();

        // Saving + restoring kout position doesn't help much as preemption still occurs,
        // only LoopThreads are synchronized, so other output will be disturbed sometimes
        kout.setpos(55, this->id);
        kout << fillw(3) << this->id << fillw(0) << ": " << dec << cnt++ << endl;

        sem->v();
    }
}

void PreemptiveThreadDemo::run() {
    kout << "Preemptive Thread Demo" << endl;
    kout << "Initializing Semaphore" << endl;
    Semaphore* sem = new Semaphore(1);  // Create this semaphore on the heap as this thread exits itself,
                                        // so stack allocated objects will be lost

    // TODO: Threads disappear, the fewer loop the faster they vanish
    //       It seems like this happens with exactly 3 or 4 loopthreads
    Thread* threads[this->number_of_threads];
    kout << "Allocating LoopThreads" << endl;
    for (unsigned int i = 0; i < this->number_of_threads; ++i) {
        threads[i] = new PreemptiveLoopThread(i, sem);
    }

    kout << "Adding threads to ready queue" << endl;
    for (unsigned int i = 0; i < this->number_of_threads; ++i) {
        scheduler.ready(threads[i]);
    }

    kout << "Exiting main thread" << endl;
    scheduler.exit();
}
