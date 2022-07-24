#ifndef __KeyEventManager_Include_H_
#define __KeyEventManager_Include_H_

#include "user/event/KeyEventListener.h"
#include "user/lib/Logger.h"
#include "user/lib/Vector.h"

// NOTE: Could do this more generally but we only have key events
//       Also pretty limited: One thread can have one listener as identification is done over tid

class KeyEventManager {
private:
    KeyEventManager(const KeyEventManager& copy) = delete;

    NamedLogger log;

    bse::vector<KeyEventListener*> listeners;

public:
    KeyEventManager() : log("KEvMan"), listeners(true) {}

    void init() {
        listeners.reserve();
    }

    void subscribe(KeyEventListener& sub);
    void unsubscribe(KeyEventListener& unsub);
    void broadcast(char c);  // Unblocks all input waiting threads, I don't have a method to direct input
};

#endif
