#ifndef __KeyEventManager_Include_H_
#define __KeyEventManager_Include_H_

#include "user/event/KeyEventListener.h"
#include "user/lib/ArrayList.h"
#include "user/lib/Logger.h"

// NOTE: Could do this more generally but we only have key events

class KeyEventManager {
private:
    KeyEventManager(const KeyEventManager& copy) = delete;

    Logger log;

    ArrayList<KeyEventListener*> listeners;

public:
    KeyEventManager() : log("KEvMan") {}

    void subscribe(KeyEventListener& listener);
    void unsubscribe(KeyEventListener& listener);
    void broadcast(char c);  // Unblocks all input waiting threads, I don't have a method to direct input
};

#endif
