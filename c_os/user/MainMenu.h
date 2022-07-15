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
        kout << "MainMenu initialized" << endl;
        // kevman.subscribe(this->listener);
    }

    void run() override;
};

#endif
