#ifndef __ArrayListDemo_include__
#define __ArrayListDemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "user/lib/ArrayList.h"

class ArrayListDemo : public Thread {
private:
    ArrayListDemo(const ArrayListDemo& copy) = delete;

    ArrayList<int> list;

public:
    ArrayListDemo() {
        kout << "Initialized ArrayListDemo" << endl;
    }

    void run() override;
};

#endif
