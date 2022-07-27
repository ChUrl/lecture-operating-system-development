#ifndef BlueScreenDemo_include__
#define BlueScreenDemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "user/event/KeyEventListener.h"

class PagingDemo: public Thread {
private:
    void writeprotect_page();
    void free_page();
    void notpresent_page();

    KeyEventListener listener;

public:
    PagingDemo(const PagingDemo& copy) = delete;

    PagingDemo(): Thread("PagingDemo"), listener(tid) {
        kevman.subscribe(listener);
    }

    ~PagingDemo() override {
        kevman.unsubscribe(listener);
    }

    void run() override;
};

#endif
