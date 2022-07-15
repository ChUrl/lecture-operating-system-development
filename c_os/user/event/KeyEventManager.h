#ifndef __KeyEventManager_Include_H_
#define __KeyEventManager_Include_H_

#include "user/event/KeyEventListener.h"

// NOTE: Could do this more generally but we only have key events

class KeyEventManager {
private:
    KeyEventManager(const KeyEventManager& copy) = delete;

    // TODO: Implement some sort of set structure
    KeyEventListener* listeners[16];  // This is pretty limited but should be enough
    unsigned int num_subscribed = 0;

public:
    KeyEventManager() {}

    void subscribe(KeyEventListener& listener);
    void broadcast(char c);  // Unblocks all input waiting threads, I don't have a method to direct input
};

#endif
