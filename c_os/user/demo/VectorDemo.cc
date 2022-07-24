#include "user/demo/VectorDemo.h"

void print(OutStream& os, const bse::vector<int>& list) {
    os << "Printing List: ";
    for (const int i : list) {
        os << i << " ";
    }
    os << endl;
}

void VectorDemo::run() {
    bse::vector<int> list;

    kout.lock();
    kout.clear();
    kout << "Logs are written to serial to see the memory interactions" << endl;

    log.info() << "Initial list size: " << dec << list.size() << endl;

    log.info() << "Adding elements in order" << endl;
    for (unsigned int i = 0; i < 5; ++i) {
        list.push_back(i);
    }
    print(log.info(), list);

    log.info() << "Removing all elements from the front" << endl;
    for (unsigned int i = 0; i < 5; ++i) {
        list.erase(list.begin());
    }
    print(log.info(), list);

    // ============================================================

    log.info() << "Adding elements in order with realloc" << endl;
    for (unsigned int i = 0; i < 10; ++i) {
        log.info() << "Add " << dec << i << endl;
        list.push_back(i);
    }
    print(log.info(), list);

    log.info() << "Removing all elements from the back" << endl;
    for (unsigned int i = 0; i < 10; ++i) {
        list.erase(list.end() - 1);
    }
    print(log.info(), list);

    // ============================================================

    for (unsigned int i = 0; i < 5; ++i) {
        list.push_back(i);
    }
    print(log.info(), list);

    log.info() << "Adding inside the list (at idx 0, 2, 5)" << endl;
    list.insert(list.begin() + 0, 10);
    list.insert(list.begin() + 2, 10);
    list.insert(list.begin() + 5, 10);
    print(log.info(), list);

    log.info() << "Removing inside the list (at idx 0, 2, 5)" << endl;
    list.erase(list.begin() + 0);
    list.erase(list.begin() + 2);
    list.erase(list.begin() + 5);
    print(log.info(), list);

    for (unsigned int i = 0; i < 5; ++i) {
        list.erase(list.begin());
    }
    print(log.info(), list);

    // ============================================================

    log.info() << "Mirror scheduling behavior" << endl;

    // These are the threads
    int active = 0;  // Idle thread
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    print(log.info(), list);

    log.info() << "Starting..." << endl;
    for (unsigned int n = 0; n < 10000; ++n) {
        list.push_back(active);
        active = list[0];
        list.erase(list.begin());

        if (list.size() != 3 || active == -1) {
            log.info() << "ERROR: Thread went missing" << endl;
            break;
        }

        if (n < 5) {
            print(log.info(), list);
        }
    }
    log.info() << "Finished." << endl;

    print(log.info(), list);

    // ============================================================

    log.info() << "Range based for support" << endl;
    for (int i : list) {
        log.info() << "List contains element: " << dec << i << endl;
    }

    kout.unlock();
    scheduler.exit();
}
