#include "user/event/KeyEventListener.h"
#include "kernel/Globals.h"

Logger KeyEventListener::log("KEvLis");

void KeyEventListener::trigger(char c) {
    this->lastChar = c;
}

char KeyEventListener::waitForKeyEvent() const {
    log << DEBUG << "Thread with id: " << tid << " waiting for key event" << endl;
    scheduler.block();
    return this->lastChar;  // This is only executed after thread is woken up by manager
}
