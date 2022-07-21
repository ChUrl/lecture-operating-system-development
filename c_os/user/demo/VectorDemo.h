#ifndef __VectorDemo_include__
#define __VectorDemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "user/lib/Vector.h"

class VectorDemo : public Thread {
private:
    VectorDemo(const VectorDemo& copy) = delete;

public:
    VectorDemo() {
        kout << "Initialized VectorDemo" << endl;
    }

    void run() override;
};

#endif
