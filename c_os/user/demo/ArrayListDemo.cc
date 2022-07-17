#include "user/demo/ArrayListDemo.h"

void ArrayListDemo::run() {
    kout.lock();
    kout.clear();
    kout << "Initial list size: " << dec << this->list.size() << endl;

    kout << "Adding elements in order" << endl;
    for (unsigned int i = 0; i < 5; ++i) {
        this->list.insert_last(i);
    }
    this->list.print(kout);

    kout << "Removing all elements from the front" << endl;
    for (unsigned int i = 0; i < 5; ++i) {
        this->list.remove_first();
    }
    this->list.print(kout);

    // ============================================================

    kout << "Adding elements in order with realloc" << endl;
    for (unsigned int i = 0; i < 10; ++i) {
        kout << "Add " << dec << i << endl;
        this->list.insert_last(i);
    }
    this->list.print(kout);

    kout << "Removing all elements from the back" << endl;
    for (unsigned int i = 0; i < 10; ++i) {
        this->list.remove_last();
    }
    this->list.print(kout);

    // ============================================================

    for (unsigned int i = 0; i < 5; ++i) {
        this->list.insert_last(i);
    }
    this->list.print(kout);

    kout << "Adding inside the list (at idx 0, 2, 5)" << endl;
    this->list.insert_at(10, 0);
    this->list.insert_at(10, 2);
    this->list.insert_at(10, 5);
    this->list.print(kout);

    kout << "Removing inside the list (at idx 0, 2, 5)" << endl;
    this->list.remove_at(0);
    this->list.remove_at(2);
    this->list.remove_at(5);
    this->list.print(kout);

    for (unsigned int i = 0; i < 5; ++i) {
        this->list.remove_first();
    }
    this->list.print(kout);

    // ============================================================

    kout << "Mirror scheduling behavior" << endl;

    // These are the threads
    int active = 0;  // Idle thread
    this->list.insert_last(1);
    this->list.insert_last(2);
    this->list.insert_last(3);
    this->list.print(kout);

    kout << "Starting..." << endl;
    for (unsigned int n = 0; n < 1000; ++n) {
        this->list.insert_last(active);
        active = list.remove_first();

        if (this->list.size() != 3) {
            kout << "ERROR: Thread went missing" << endl;
            break;
        }

        if (n < 5) {
            this->list.print(kout);
        }
    }
    kout << "Finished." << endl;

    this->list.print(kout);

    // ============================================================

    kout << "Range based for support" << endl;
    for (int i : this->list) {
        kout << "List contains element: " << dec << i << endl;
    }

    kout.unlock();
    scheduler.exit();
}
