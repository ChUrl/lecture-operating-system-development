#include "user/demo/SmartPointerDemo.h"
#include "kernel/threads/IdleThread.h"
#include "user/lib/Array.h"
#include "user/lib/mem/UniquePointer.h"

void SmartPointerDemo::run() {
    kout.lock();
    kout.clear();

    kout << "Output is written to log to be able to trace memory allocations/deallocations" << endl;

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

    // NOTE: This wasn't working because of a missing operator[] delete in the allocator
    log << INFO << "Allocating unique_ptr<int>*..." << endl;
    bse::unique_ptr<int>* ptrptr = new bse::unique_ptr<int>[10];
    delete[] ptrptr;
    log << INFO << "Should be deleted by now..." << endl;

    // =====================================================================

    {
        log << INFO << "Stackallocating Array<bse::unique_ptr<int>, 10>..." << endl;
        bse::Array<bse::unique_ptr<int>, 10> arr;
        log << INFO << "Populating slot 0..." << endl;
        arr[0] = bse::make_unique<int>(1);
        log << INFO << "Moving slot 0 to slot 1..." << endl;
        arr[1] = std::move(arr[0]);
        log << INFO << "Leaving scope" << endl;
    }
    log << INFO << "Should be deleted by now..." << endl;

    {
        log << INFO << "Heapallocating Array<bse::unique_ptr<int>, 10>..." << endl;
        bse::Array<bse::unique_ptr<int>, 10>* arr = new bse::Array<bse::unique_ptr<int>, 10>;
        log << INFO << "Populating slot 0..." << endl;
        (*arr)[0] = bse::make_unique<int>(1);
        log << INFO << "Moving slot 0 to slot 1..." << endl;
        (*arr)[1] = std::move((*arr)[0]);
        log << INFO << "Deleting" << endl;
        delete arr;
        log << INFO << "Leaving scope" << endl;
    }
    log << INFO << "Should be deleted by now..." << endl;

    {
        log << INFO << "ArrayList<bse::unique_ptr<int>>..." << endl;
        bse::Vector<bse::unique_ptr<int>> vec;
        log << INFO << "2x insertion" << endl;
        vec.push_back(bse::make_unique<int>(1));
        vec.push_back(bse::make_unique<int>(2));

        log << INFO << "Leaving scope" << endl;
    }
    log << INFO << "Should be deleted by now..." << endl;

    kout.unlock();
    scheduler.exit();
}
