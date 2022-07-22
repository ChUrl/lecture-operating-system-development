#ifndef __SmartPointerDemo_include__
#define __SmartPointerDemo_include__

#include "kernel/Globals.h"

class SmartPointerDemo : public Thread {
private:
    SmartPointerDemo(const SmartPointerDemo& copy) = delete;

public:
    SmartPointerDemo() : Thread("SmartPointerDemo") {}

    void run() override;
};

#endif
