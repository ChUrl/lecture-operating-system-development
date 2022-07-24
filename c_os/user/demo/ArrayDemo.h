#ifndef ArrayDemo_include__
#define ArrayDemo_include__

#include "kernel/Globals.h"
#include "user/lib/Array.h"

class ArrayDemo : public Thread {
public:
    ArrayDemo(const ArrayDemo& copy) = delete;

    ArrayDemo() : Thread("ArrayDemo") {}

    void run() override;
};

#endif
