#include "user/event/KeyEventManager.h"
#include "kernel/Globals.h"

void KeyEventManager::subscribe(KeyEventListener& sub) {
    log.debug() << "Subscribe, Thread ID: " << dec << sub.tid << endl;
    listeners.push_back(&sub);
}

void KeyEventManager::unsubscribe(KeyEventListener& unsub) {
    log.debug() << "Unsubscribe, Thread ID: " << dec << unsub.tid << endl;
    for (bse::vector<KeyEventListener*>::iterator it = listeners.begin(); it != listeners.end(); ++it) {
        if ((*it)->tid == unsub.tid) {
            listeners.erase(it);
            return;
        }
    }
}

void KeyEventManager::broadcast(char c) {
    log.trace() << "Beginning Broadcast" << endl;
    for (KeyEventListener* listener : listeners) {
        log.trace() << "Broadcasting " << c << " to Thread ID: " << dec << listener->tid << endl;
        listener->trigger(c);
        scheduler.deblock(listener->tid);
    }
}
