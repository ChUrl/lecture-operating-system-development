#include "user/demo/SmartPointerDemo.h"
#include "user/lib/mem/UniquePointer.h"
// #include <memory>

void SmartPointerDemo::run() {
    kout.clear();
    kout << "Output is written to log to be able to trace memory allocations/deallocations" << endl;

    // <memory> legit smartpointer
    // {
    //     log << INFO << "Allocating new unique_ptr<int>..." << endl;
    //     std::unique_ptr<int> ptr1 = std::make_unique<int>(1);
    //     std::unique_ptr<int> ptr2 = std::make_unique<int>(2);

    //     log << INFO << "Moving..." << endl;
    //     ptr2 = std::move(ptr1);
    //     log << INFO << "Leaving scope..." << endl;
    // }
    // log << INFO << "Should be deleted by now..." << endl;

    {
        log << INFO << "Allocating new unique_ptr<int>..." << endl;
        bse::unique_ptr<int> ptr = bse::make_unique<int>(1);
        log << INFO << "Leaving scope..." << endl;
    }
    log << INFO << "Should be deleted by now..." << endl;

    {
        log << INFO << "Allocating new unique_ptr<int>..." << endl;
        bse::unique_ptr<int> ptr1 = bse::make_unique<int>(1);
        bse::unique_ptr<int> ptr2;

        log << INFO << "*ptr1 == " << *ptr1 << ", (bool)ptr2 == " << (bool)ptr2 << endl;
        log << INFO << "Moving ptr1 => ptr2 (no allocations should happen)..." << endl;
        // ptr2 = ptr1;  // Doesn't work, copy assignment is deleted
        ptr2 = std::move(ptr1);
        log << INFO << "(bool)ptr1 == " << (bool)ptr1 << ", *ptr2 == " << *ptr2 << endl;

        log << INFO << "Leaving scope..." << endl;
    }
    log << INFO << "Should be deleted by now..." << endl;

    {
        log << INFO << "Allocating (2) new unique_ptr<int>..." << endl;
        bse::unique_ptr<int> ptr1 = bse::make_unique<int>(1);
        bse::unique_ptr<int> ptr2 = bse::make_unique<int>(1);

        log << INFO << "Moving ptr1 => ptr2 (ptr1 should be freed)..." << endl;
        ptr2 = std::move(ptr1);

        log << INFO << "Leaving scope..." << endl;
    }
    log << INFO << "Should be deleted by now..." << endl;

    // =====================================================================

    {
        log << INFO << "Allocating new unique_ptr<int[]>..." << endl;
        bse::unique_ptr<int[]> ptr = bse::make_unique<int[]>(10);
        ptr[0] = 1;
        log << INFO << "ptr[0] == " << ptr[0] << endl;
    }
    log << INFO << "Should be deleted by now..." << endl;

    {
        log << INFO << "Allocating new unique_ptr<int[10]>..." << endl;
        bse::unique_ptr<int[]> ptr1 = bse::make_unique<int[]>(10);
        bse::unique_ptr<int[]> ptr2;

        log << INFO << "Moving ptr1 => ptr2 (no allocations should happen)..." << endl;
        ptr2 = std::move(ptr1);

        log << INFO << "Leaving scope..." << endl;
    }
    log << INFO << "Should be deleted by now..." << endl;

    {
        log << INFO << "Allocating (2) new unique_ptr<int[10]>..." << endl;
        bse::unique_ptr<int> ptr1 = bse::make_unique<int>(10);
        bse::unique_ptr<int> ptr2 = bse::make_unique<int>(10);

        log << INFO << "Moving ptr1 => ptr2 (ptr1 should be freed)..." << endl;
        ptr2 = std::move(ptr1);

        log << INFO << "Leaving scope..." << endl;
    }
    log << INFO << "Should be deleted by now..." << endl;

    scheduler.exit();
}
