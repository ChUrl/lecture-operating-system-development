#ifndef __LinkedListDemo_include__
#define __LinkedListDemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "user/lib/LinkedList.h"

class LinkedListDemo : public Thread {
private:
    LinkedListDemo(const LinkedListDemo& copy) = delete;

    LinkedList<int> list;

public:
    LinkedListDemo() {
        kout << "Initialized LinkedListDemo" << endl;
    }

    void run() override;
};

#endif
