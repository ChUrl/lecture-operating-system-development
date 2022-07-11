#include "user/PreemptiveLoopThread.h"

void PreemptiveLoopThread::run() {

    /* Hier muss Code eingefuegt werden */

    int cnt = 0;
    while (true) {
        // Basic synchronization by semaphore
        sem->p();

        kout.setpos(55, this->id);
        kout << this->id << ": " << dec << cnt++ << endl;

        sem->v();
    }
}
