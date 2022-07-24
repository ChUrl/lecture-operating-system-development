#include "user/event/KeyEventListener.h"
#include "kernel/Globals.h"

void KeyEventListener::trigger(char c) {
    lastChar = c;
}

char KeyEventListener::waitForKeyEvent() const {
    Logger::instance() << DEBUG << "KEvLis:: Thread with id: " << tid << " waiting for key event" << endl;
    scheduler.block();
    return lastChar;  // This is only executed after thread is woken up by manager
}
