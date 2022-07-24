#ifndef SmartPointerDemo_include__
#define SmartPointerDemo_include__

#include "kernel/Globals.h"

class SmartPointerDemo : public Thread {
public:
    SmartPointerDemo(const SmartPointerDemo& copy) = delete;

    SmartPointerDemo() : Thread("SmartPointerDemo") {}

    void run() override;
};

#endif
