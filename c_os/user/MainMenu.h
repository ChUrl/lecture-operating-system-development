#ifndef __MainMenu_Inlucde_H_
#define __MainMenu_Inlucde_H_

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"
#include "user/event/KeyEventListener.h"

class MainMenu : public Thread {
private:
    MainMenu(const MainMenu& copy) = delete;

    KeyEventListener listener;

public:
    MainMenu() : listener(*this) {
        log << INFO << "MainMenu initialized with ID: " << dec << this->tid << endl;
        kevman.subscribe(this->listener);
    }

    ~MainMenu() override {
        log << INFO << "Unitialized MainMenu" << endl;
        kevman.unsubscribe(this->listener);
    }

    void run() override;
};

#endif
