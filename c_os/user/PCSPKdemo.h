#ifndef __PCSPKdemo_INCLUDE_H_
#define __PCSPKdemo_INCLUDE_H_

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class PCSPKdemo : public Thread {
private:
    PCSPKdemo(const PCSPKdemo& copy) = delete;

public:
    PCSPKdemo() {
        kout << "Initialized PCSPKdemo" << endl;
    }

    void run() override;
};

#endif
