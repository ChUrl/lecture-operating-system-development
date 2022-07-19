#include "user/demo/SmartPointerDemo.h"
#include "kernel/threads/IdleThread.h"

void SmartPointerDemo::run() {
    log << INFO << "Initializing unique_ptr<int>:" << endl;
    std::unique_ptr<int> int1 = std::make_unique<int>(1);
    log << INFO << "Uninitialize..." << endl;

    log << INFO << "Initializing unique_ptr<Thread>:" << endl;
    std::unique_ptr<Thread> thread1 = std::make_unique<IdleThread>();
    log << INFO << "Should be unitialized now..." << endl;

    scheduler.exit();
}
