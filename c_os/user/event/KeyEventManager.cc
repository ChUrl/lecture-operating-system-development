#include "user/event/KeyEventManager.h"
#include "kernel/Globals.h"

void KeyEventManager::subscribe(KeyEventListener& listener) {
    this->listeners.insert(&listener);
}

void KeyEventManager::broadcast(char c) {
    for (KeyEventListener* listener : this->listeners) {
        listener->trigger(c);
        scheduler.deblock(&listener->thread);
    }
}
