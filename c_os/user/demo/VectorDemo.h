#ifndef VectorDemo_include__
#define VectorDemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "user/lib/Vector.h"

class VectorDemo : public Thread {
public:
    VectorDemo(const VectorDemo& copy) = delete;

    VectorDemo() : Thread("VectorDemo") {}

    void run() override;
};

#endif
