#ifndef __SmartPointerDemo_include__
#define __SmartPointerDemo_include__

#include "kernel/Globals.h"
#include <memory>

class SmartPointerDemo : public Thread {
private:
    SmartPointerDemo(const SmartPointerDemo& copy) = delete;

public:
    SmartPointerDemo() {
        kout << "Initialized SmartPointerDemo" << endl;
    }

    void run() override;
};

#endif
