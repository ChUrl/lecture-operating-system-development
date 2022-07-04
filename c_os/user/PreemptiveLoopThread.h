#ifndef __pre_loopthread_include__
#define __pre_loopthread_include__

#include "kernel/threads/Thread.h"
#include "kernel/Globals.h"

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

#endif
