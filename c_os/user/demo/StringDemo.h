#ifndef __StringDemo_include__
#define __StringDemo_include__

#include "kernel/Globals.h"

class StringDemo : public Thread {
private:
    StringDemo(const StringDemo& copy) = delete;

public:
    StringDemo() : Thread("StringDemo") {}

    void run() override;
};

#endif
