#include "user/demo/VectorDemo.h"

void print(bse::vector<int> list) {
    kout << "Printing List: ";
    for (int i : list) {
        kout << i << " ";
    }
    kout << endl;
}

void VectorDemo::run() {
    bse::vector<int> list;

    kout.lock();
    kout.clear();
    kout << "Initial list size: " << dec << list.size() << endl;

    kout << "Adding elements in order" << endl;
    for (unsigned int i = 0; i < 5; ++i) {
        list.push_back(i);
    }
    print(list);

    kout << "Removing all elements from the front" << endl;
    for (unsigned int i = 0; i < 5; ++i) {
        list.erase(list.begin());
    }
    print(list);

    // ============================================================

    kout << "Adding elements in order with realloc" << endl;
    for (unsigned int i = 0; i < 10; ++i) {
        kout << "Add " << dec << i << endl;
        list.push_back(i);
    }
    print(list);

    kout << "Removing all elements from the back" << endl;
    for (unsigned int i = 0; i < 10; ++i) {
        list.erase(list.end() - 1);
    }
    print(list);

    // ============================================================

    for (unsigned int i = 0; i < 5; ++i) {
        list.push_back(i);
    }
    print(list);

    kout << "Adding inside the list (at idx 0, 2, 5)" << endl;
    list.insert(list.begin() + 0, 10);
    list.insert(list.begin() + 2, 10);
    list.insert(list.begin() + 5, 10);
    print(list);

    kout << "Removing inside the list (at idx 0, 2, 5)" << endl;
    list.erase(list.begin() + 0);
    list.erase(list.begin() + 2);
    list.erase(list.begin() + 5);
    print(list);

    for (unsigned int i = 0; i < 5; ++i) {
        list.erase(list.begin());
    }
    print(list);

    // ============================================================

    kout << "Mirror scheduling behavior" << endl;

    // These are the threads
    int active = 0;  // Idle thread
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    print(list);

    kout << "Starting..." << endl;
    for (unsigned int n = 0; n < 10000; ++n) {
        list.push_back(active);
        active = list[0];
        list.erase(list.begin());

        if (list.size() != 3 || active == -1) {
            kout << "ERROR: Thread went missing" << endl;
            break;
        }

        if (n < 5) {
            print(list);
        }
    }
    kout << "Finished." << endl;

    print(list);

    // ============================================================

    kout << "Range based for support" << endl;
    for (int i : list) {
        kout << "List contains element: " << dec << i << endl;
    }

    kout.unlock();
    scheduler.exit();
}
