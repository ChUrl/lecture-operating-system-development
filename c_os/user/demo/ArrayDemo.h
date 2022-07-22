#ifndef __ArrayDemo_include__
#define __ArrayDemo_include__

#include "kernel/Globals.h"
#include "user/lib/Array.h"

class ArrayDemo : public Thread {
private:
    ArrayDemo(const ArrayDemo& copy) = delete;

public:
    ArrayDemo() : Thread("ArrayDemo") {}

    void run() override;
};

#endif
