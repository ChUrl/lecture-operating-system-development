#ifndef preemptive_thread_include__
#define preemptive_thread_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "lib/Semaphore.h"

class PreemptiveLoopThread : public Thread {
private:
    int id;

public:
    PreemptiveLoopThread(const PreemptiveLoopThread& copy) = delete;  // Verhindere Kopieren

    // Gibt der Loop einen Stack und eine Id.
    PreemptiveLoopThread(int i) : Thread("LoopThread"), id(i) {}

    // Zaehlt einen Zaehler hoch und gibt ihn auf dem Bildschirm aus.
    void run() override;
};

class PreemptiveThreadDemo : public Thread {
private:
    unsigned int number_of_threads;

public:
    PreemptiveThreadDemo(const PreemptiveThreadDemo& copy) = delete;  // Verhindere Kopieren

    PreemptiveThreadDemo(unsigned int n) : Thread("PreemptiveThreadDemo"), number_of_threads(n) {}

    // Thread-Startmethode
    void run() override;
};

#endif
