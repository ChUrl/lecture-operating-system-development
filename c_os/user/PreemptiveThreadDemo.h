#ifndef __preemptive_thread_include__
#define __preemptive_thread_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "user/PreemptiveLoopThread.h"

class PreemptiveThreadDemo : public Thread {
private:
    PreemptiveThreadDemo(const PreemptiveThreadDemo& copy) = delete;  // Verhindere Kopieren

public:
    PreemptiveThreadDemo() {
        kout << "Initialized PreemptiveThreadDemo" << endl;
    }

    // Thread-Startmethode
    void run() override;
};

#endif
