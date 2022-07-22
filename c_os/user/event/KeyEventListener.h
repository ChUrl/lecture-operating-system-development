#ifndef __KeyEventListener_Include_H_
#define __KeyEventListener_Include_H_

#include "kernel/threads/Thread.h"

class KeyEventListener {
private:
    KeyEventListener(const KeyEventListener& copy) = delete;

    static Logger log;

    char lastChar = '\0';

public:
    unsigned int tid;  // Thread which contains this listener, so the listener can block the thread

    KeyEventListener(unsigned int tid) : tid(tid) {}

    char waitForKeyEvent() const;  // Blocks the thread until woken up by manager
    void trigger(char c);          // Gets called from KeyEventManager
};

#endif
