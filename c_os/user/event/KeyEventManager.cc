#include "user/event/KeyEventManager.h"
#include "kernel/Globals.h"

void KeyEventManager::subscribe(KeyEventListener& listener) {
    log << DEBUG << "Subscribe, Thread ID: " << dec << listener.thread.tid << endl;
    this->listeners.insert_last(&listener);
}

void KeyEventManager::unsubscribe(KeyEventListener& listener) {
    log << DEBUG << "Unsubscribe, Thread ID: " << dec << listener.thread.tid << endl;
    this->listeners.remove(&listener);
}

void KeyEventManager::broadcast(char c) {
    log << TRACE << "Beginning Broadcast" << endl;
    for (KeyEventListener* listener : this->listeners) {
        log << TRACE << "Broadcasting " << c << " to Thread ID: " << dec << listener->thread.tid << endl;
        listener->trigger(c);
        scheduler.deblock(&listener->thread);
    }
}
