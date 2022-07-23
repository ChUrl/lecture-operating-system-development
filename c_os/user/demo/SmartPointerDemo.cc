#include "user/demo/SmartPointerDemo.h"
#include "kernel/threads/IdleThread.h"
#include "user/lib/Array.h"
#include "user/lib/mem/UniquePointer.h"

void SmartPointerDemo::run() {
    kout.lock();
    kout.clear();

    kout << "Output is written to log to be able to trace memory allocations/deallocations" << endl;

    {
        log.info() << "Allocating new unique_ptr<int>..." << endl;
        bse::unique_ptr<int> ptr = bse::make_unique<int>(1);
        log.info() << "Leaving scope..." << endl;
    }
    log.info() << "Should be deleted by now..." << endl;

    {
        log.info() << "Allocating new unique_ptr<int>..." << endl;
        bse::unique_ptr<int> ptr1 = bse::make_unique<int>(1);
        bse::unique_ptr<int> ptr2;

        log.info() << "*ptr1 == " << *ptr1 << ", (bool)ptr2 == " << (bool)ptr2 << endl;
        log.info() << "Moving ptr1 => ptr2 (no allocations should happen)..." << endl;
        ptr2 = std::move(ptr1);
        log.info() << "(bool)ptr1 == " << (bool)ptr1 << ", *ptr2 == " << *ptr2 << endl;

        log.info() << "Leaving scope..." << endl;
    }
    log.info() << "Should be deleted by now..." << endl;

    {
        log.info() << "Allocating (2) new unique_ptr<int>..." << endl;
        bse::unique_ptr<int> ptr1 = bse::make_unique<int>(1);
        bse::unique_ptr<int> ptr2 = bse::make_unique<int>(1);

        log.info() << "Moving ptr1 => ptr2 (ptr1 should be freed)..." << endl;
        ptr2 = std::move(ptr1);

        log.info() << "Leaving scope..." << endl;
    }
    log.info() << "Should be deleted by now..." << endl;

    // =====================================================================

    {
        log.info() << "Allocating new unique_ptr<int[]>..." << endl;
        bse::unique_ptr<int[]> ptr = bse::make_unique<int[]>(10);
        ptr[0] = 1;
        log.info() << "ptr[0] == " << ptr[0] << endl;
    }
    log.info() << "Should be deleted by now..." << endl;

    {
        log.info() << "Allocating new unique_ptr<int[10]>..." << endl;
        bse::unique_ptr<int[]> ptr1 = bse::make_unique<int[]>(10);
        bse::unique_ptr<int[]> ptr2;

        log.info() << "Moving ptr1 => ptr2 (no allocations should happen)..." << endl;
        ptr2 = std::move(ptr1);

        log.info() << "Leaving scope..." << endl;
    }
    log.info() << "Should be deleted by now..." << endl;

    {
        log.info() << "Allocating (2) new unique_ptr<int[10]>..." << endl;
        bse::unique_ptr<int> ptr1 = bse::make_unique<int>(10);
        bse::unique_ptr<int> ptr2 = bse::make_unique<int>(10);

        log.info() << "Moving ptr1 => ptr2 (ptr1 should be freed)..." << endl;
        ptr2 = std::move(ptr1);

        log.info() << "Leaving scope..." << endl;
    }
    log.info() << "Should be deleted by now..." << endl;

    // NOTE: This wasn't working because of a missing operator[] delete in the allocator
    log.info() << "Allocating unique_ptr<int>*..." << endl;
    bse::unique_ptr<int>* ptrptr = new bse::unique_ptr<int>[10];
    delete[] ptrptr;
    log.info() << "Should be deleted by now..." << endl;

    // =====================================================================

    {
        log.info() << "Stackallocating Array<bse::unique_ptr<int>, 10>..." << endl;
        bse::array<bse::unique_ptr<int>, 10> arr;
        log.info() << "Populating slot 0..." << endl;
        arr[0] = bse::make_unique<int>(1);
        log.info() << "Moving slot 0 to slot 1..." << endl;
        arr[1] = std::move(arr[0]);
        log.info() << "Leaving scope" << endl;
    }
    log.info() << "Should be deleted by now..." << endl;

    {
        log.info() << "Heapallocating Array<bse::unique_ptr<int>, 10>..." << endl;
        bse::array<bse::unique_ptr<int>, 10>* arr = new bse::array<bse::unique_ptr<int>, 10>;
        log.info() << "Populating slot 0..." << endl;
        (*arr)[0] = bse::make_unique<int>(1);
        log.info() << "Moving slot 0 to slot 1..." << endl;
        (*arr)[1] = std::move((*arr)[0]);
        log.info() << "Deleting" << endl;
        delete arr;
        log.info() << "Leaving scope" << endl;
    }
    log.info() << "Should be deleted by now..." << endl;

    {
        log.info() << "ArrayList<bse::unique_ptr<int>>..." << endl;
        bse::Vector<bse::unique_ptr<int>> vec;
        log.info() << "2x insertion" << endl;
        vec.push_back(bse::make_unique<int>(1));
        vec.push_back(bse::make_unique<int>(2));

        log.info() << "Leaving scope" << endl;
    }
    log.info() << "Should be deleted by now..." << endl;

    kout.unlock();
    scheduler.exit();
}
