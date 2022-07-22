#ifndef __preemptive_thread_include__
#define __preemptive_thread_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "lib/Semaphore.h"

class PreemptiveLoopThread : public Thread {
private:
    int id;
    PreemptiveLoopThread(const PreemptiveLoopThread& copy) = delete;  // Verhindere Kopieren

public:
    // Gibt der Loop einen Stack und eine Id.
    PreemptiveLoopThread(int i) : id(i) {}

    // Zaehlt einen Zaehler hoch und gibt ihn auf dem Bildschirm aus.
    void run() override;
};

class PreemptiveThreadDemo : public Thread {
private:
    PreemptiveThreadDemo(const PreemptiveThreadDemo& copy) = delete;  // Verhindere Kopieren

    unsigned int number_of_threads;

public:
    PreemptiveThreadDemo(unsigned int n) : number_of_threads(n) {
        kout << "Initialized PreemptiveThreadDemo" << endl;
    }

    // Thread-Startmethode
    void run() override;
};

#endif
