#include "user/demo/PCSPKdemo.h"

void PCSPKdemo::run() {
    (pcspk.*this->melody)();  // This syntax is confusing as hell

    pcspk.off();
    scheduler.exit();
}
