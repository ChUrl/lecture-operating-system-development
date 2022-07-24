#ifndef PCSPKdemo_INCLUDE_H_
#define PCSPKdemo_INCLUDE_H_

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class PCSPKdemo : public Thread {
private:
    void (PCSPK::*melody)();  // Allow to pass a melody to play when initializing the demo

public:
    PCSPKdemo(const PCSPKdemo& copy) = delete;

    explicit PCSPKdemo(void (PCSPK::*melody)()) : Thread("PCSPKdemo"), melody(melody) {}

    ~PCSPKdemo() override {
        PCSPK::off();
    }

    void run() override;
};

#endif
