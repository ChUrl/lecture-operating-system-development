#ifndef __BlueScreenDemo_include__
#define __BlueScreenDemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class BlueScreenDemo : public Thread {
private:
    BlueScreenDemo(const BlueScreenDemo& copy) = delete;

public:
    BlueScreenDemo() : Thread("BlueScreenDemo") {}

    void run() override;
};

#endif
