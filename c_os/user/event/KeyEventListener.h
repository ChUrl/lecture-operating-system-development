#ifndef KeyEventListener_Include_H_
#define KeyEventListener_Include_H_

#include "kernel/threads/Thread.h"

class KeyEventListener {
private:
    char lastChar = '\0';

    friend class KeyEventManager;
    unsigned int tid;  // Thread which contains this listener, so the listener can block the thread

public:
    KeyEventListener(const KeyEventListener& copy) = delete;

    KeyEventListener(unsigned int tid) : tid(tid) {}

    char waitForKeyEvent() const;  // Blocks the thread until woken up by manager
    void trigger(char c);          // Gets called from KeyEventManager
};

#endif
