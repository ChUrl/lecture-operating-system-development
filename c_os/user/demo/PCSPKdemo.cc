#include "user/demo/PCSPKdemo.h"

void PCSPKdemo::run() {
    kout << "Playing..." << endl;
    (pcspk.*this->melody)();  // This syntax is confusing as hell
    kout << "Finished" << endl;

    scheduler.exit();
}
