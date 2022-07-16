#ifndef __PCSPKdemo_INCLUDE_H_
#define __PCSPKdemo_INCLUDE_H_

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class PCSPKdemo : public Thread {
private:
    PCSPKdemo(const PCSPKdemo& copy) = delete;
    void (PCSPK::*melody)(void);  // Allow to pass a melody to play when initializing the demo

public:
    PCSPKdemo(void (PCSPK::*melody)(void)) : melody(melody) {
        kout << "Initialized PCSPKdemo" << endl;
    }

    ~PCSPKdemo() override {
        pcspk.off();
    }

    void run() override;
};

#endif
