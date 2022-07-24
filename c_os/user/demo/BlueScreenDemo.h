#ifndef BlueScreenDemo_include__
#define BlueScreenDemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class BlueScreenDemo : public Thread {
public:
    BlueScreenDemo(const BlueScreenDemo& copy) = delete;

    BlueScreenDemo() : Thread("BlueScreenDemo") {}

    void run() override;
};

#endif
