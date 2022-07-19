#include "user/demo/SmartPointerDemo.h"
#include "kernel/threads/IdleThread.h"
#include "user/demo/ArrayDemo.h"
#include "user/demo/ArrayListDemo.h"
#include "user/demo/BlueScreenDemo.h"
#include "user/demo/HeapDemo.h"
#include "user/demo/KeyboardDemo.h"
#include "user/demo/LinkedListDemo.h"
#include "user/demo/PCSPKdemo.h"
#include "user/demo/PreemptiveThreadDemo.h"
#include "user/demo/TextDemo.h"
#include "user/demo/VBEdemo.h"
#include <memory>

// NOTE: I had weird linker errors (bc of smart pointers) I couldn't figure out,
//       so I tried to determine what caused them
// NOTE: Found the problem: Because we compile with -nostdlib the reference counting code of the
//       shared_ptr is not available. Can only use unique_ptr
// NOTE: Because using unique_ptr inside C-Style array is difficult (deletion) I can't use them

void SmartPointerDemo::run() {
    // Log from threaad to see status between alloc logs

    log << INFO << "Initializing unique_ptr<int>:" << endl;
    {
        std::unique_ptr<int> int1 = std::make_unique<int>(1);
        log << INFO << *int1 << endl;
    }
    log << INFO << "Should be uninitialized by now..." << endl;

    // ==================================================================

    log << INFO << "Testing unique_ptr<Thread>..." << endl;
    {
        std::unique_ptr<Thread> thread1 = std::make_unique<IdleThread>();
        std::unique_ptr<Thread> thread2 = std::make_unique<TextDemo>();
        std::unique_ptr<Thread> thread3 = std::make_unique<PCSPKdemo>(&PCSPK::aerodynamic);
        std::unique_ptr<Thread> thread4 = std::make_unique<KeyboardDemo>();
        std::unique_ptr<Thread> thread5 = std::make_unique<HeapDemo>();
        std::unique_ptr<Thread> thread6 = std::make_unique<VBEdemo>();
        std::unique_ptr<Thread> thread7 = std::make_unique<BlueScreenDemo>();
        std::unique_ptr<Thread> thread8 = std::make_unique<PreemptiveThreadDemo>(3);
        std::unique_ptr<Thread> thread9 = std::make_unique<ArrayListDemo>();
        std::unique_ptr<Thread> thread10 = std::make_unique<LinkedListDemo>();
        std::unique_ptr<Thread> thread11 = std::make_unique<ArrayDemo>();
    }
    log << INFO << "Finished." << endl;

    // log << INFO << "Testing shared_ptr<Thread>..." << endl;
    // {
    //     std::shared_ptr<Thread> thread1 = std::make_shared<IdleThread>();
    //     std::shared_ptr<Thread> thread2 = std::make_shared<TextDemo>();
    //     std::shared_ptr<Thread> thread3 = std::make_shared<PCSPKdemo>(&PCSPK::aerodynamic);
    //     std::shared_ptr<Thread> thread4 = std::make_shared<KeyboardDemo>();
    //     std::shared_ptr<Thread> thread5 = std::make_shared<HeapDemo>();
    //     std::shared_ptr<Thread> thread6 = std::make_shared<VBEdemo>();
    //     std::shared_ptr<Thread> thread7 = std::make_shared<BlueScreenDemo>();
    //     std::shared_ptr<Thread> thread8 = std::make_shared<PreemptiveThreadDemo>(3);
    //     std::shared_ptr<Thread> thread9 = std::make_shared<ArrayListDemo>();
    //     std::shared_ptr<Thread> thread10 = std::make_shared<LinkedListDemo>();
    //     std::shared_ptr<Thread> thread11 = std::make_shared<ArrayDemo>();
    // }
    // log << INFO << "Finished." << endl;

    // ==================================================================

    log << INFO << "Initializing unique_ptr<int>:" << endl;
    {
        std::unique_ptr<int> int1 = std::make_unique<int>(1);
        std::unique_ptr<int> int2;

        log << INFO << "Moving:" << endl;
        int2 = std::move(int1);
        log << INFO << *int2 << endl;
    }
    log << INFO << "Should be unitialized by now..." << endl;

    // ==================================================================

    log << INFO << "Testing Lists..." << endl;
    {
        // Doesn't work
        // std::unique_ptr<std::unique_ptr<Thread>[]> buf = std::make_unique<std::unique_ptr<Thread>[]>(10);

        // Something like this happens in the ArrayList
        std::unique_ptr<Thread>* buf = new std::unique_ptr<Thread>[10];
        std::unique_ptr<Thread>* new_buf = new std::unique_ptr<Thread>[10];

        log << INFO << "Allocating Thread in buf..." << endl;
        buf[0] = std::make_unique<IdleThread>();

        log << INFO << "Moving Thread to new_buf..." << endl;
        new_buf[0] = std::move(buf[0]);

        // BUG: Because buf[0] is basically the same as buf there are double deletes happening.
        //      Can't be sure if the block wasn't reallocated and this deletes live data...
        log << INFO << "Deleting buf..." << endl;
        delete buf;

        log << INFO << "Deleting new_buf..." << endl;
        delete new_buf;
    }
    log << INFO << "Finished." << endl;

    scheduler.exit();
}
