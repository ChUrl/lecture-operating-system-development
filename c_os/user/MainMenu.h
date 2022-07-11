#ifndef __MainMenu_Inlucde_H_
#define __MainMenu_Inlucde_H_

#include "kernel/threads/Thread.h"
#include "kernel/Globals.h"

class MainMenu : public Thread {
private:
    MainMenu(const MainMenu& copy) = delete;

public:
    MainMenu() {
        kout << "MainMenu initialized" << endl;
    }

    void run() override;
};

#endif
