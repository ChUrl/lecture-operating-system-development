#include "user/PreemptiveLoopThread.h"

void PreemptiveLoopThread::run() {

    /* Hier muss Code eingefuegt werden */

    int cnt = 0;
    while (true) {
        // Basic synchronization by disabling PIT interrupts
        cpu.disable_int();

        kout.setpos(55, this->id);
        kout << this->id << ": " << dec << cnt++ << endl;

        cpu.enable_int();
    }
}
