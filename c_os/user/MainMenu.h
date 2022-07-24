#ifndef MainMenu_Inlucde_H_
#define MainMenu_Inlucde_H_

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "user/event/KeyEventListener.h"

class MainMenu : public Thread {
private:
    KeyEventListener listener;

public:
    MainMenu(const MainMenu& copy) = delete;

    MainMenu() : Thread("MainMenu"), listener(tid) {
        kevman.subscribe(listener);
    }

    ~MainMenu() override {
        kevman.unsubscribe(listener);
    }

    void run() override;
};

#endif
