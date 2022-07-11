#include "user/KeyEventListener.h"
#include "kernel/Globals.h"

void KeyEventListener::trigger(char c) {
    this->lastChar = c;
}

char KeyEventListener::waitForKeyEvent() const {
    scheduler.block();
    return this->lastChar;
}
