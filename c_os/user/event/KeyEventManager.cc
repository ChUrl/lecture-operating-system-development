#include "user/event/KeyEventManager.h"
#include "kernel/Globals.h"

void KeyEventManager::subscribe(KeyEventListener& listener) {
    this->listeners[this->num_subscribed] = &listener;
    this->num_subscribed = this->num_subscribed + 1;
}

void KeyEventManager::broadcast(char c) {
    for (unsigned int i = 0; i < this->num_subscribed; ++i) {
        this->listeners[i]->trigger(c);  // Sets lastChar, do this before wakeup
        scheduler.deblock(&this->listeners[i]->thread);
    }
}
