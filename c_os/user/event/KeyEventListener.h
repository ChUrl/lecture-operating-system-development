#ifndef __KeyEventListener_Include_H_
#define __KeyEventListener_Include_H_

#include "kernel/threads/Thread.h"

class KeyEventListener {
private:
    KeyEventListener(const KeyEventListener& copy) = delete;

    char lastChar = '\0';

public:
    Thread& thread;  // Thread which contains this listener, so the listener can block the thread

    KeyEventListener(Thread& thread) : thread(thread) {}

    char waitForKeyEvent() const;  // Blocks the thread until woken up by manager
    void trigger(char c);          // Gets called from KeyEventManager
};

#endif
