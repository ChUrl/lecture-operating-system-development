#include "user/demo/ArrayDemo.h"

void ArrayDemo::run() {
    Array<int, 10> arr1 {};
    Array<int, 10> arr2 {};
    Array<Thread*, 10> arr3 {};

    kout.lock();
    kout.clear();

    kout << "Adding..." << endl;
    for (int i = 0; i < 10; ++i) {
        arr1[i] = i;
    }

    kout << "Iterator printing arr1:" << endl;
    for (int i : arr1) {
        kout << i << " ";
    }
    kout << endl;

    kout << "Swapping arr1 and arr2..." << endl;
    arr1.swap(arr2);

    kout << "Iterator printing arr1:" << endl;
    for (int i : arr1) {
        kout << i << " ";
    }
    kout << endl;

    kout << "Iterator printing arr2:" << endl;
    for (int i : arr2) {
        kout << i << " ";
    }
    kout << endl;

    // arr1.swap(arr3);  // Not possible as type/size has to match

    kout.unlock();
    scheduler.exit();
}
