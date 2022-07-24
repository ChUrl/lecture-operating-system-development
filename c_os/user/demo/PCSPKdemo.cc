#include "user/demo/PCSPKdemo.h"

void PCSPKdemo::run() {
    kout.lock();
    kout.clear();
    kout << "Playing..." << endl;
    kout.unlock();

    (*melody)();  // This syntax is confusing as hell

    kout.lock();
    kout << "Finished" << endl;
    kout.unlock();

    scheduler.exit();
}
