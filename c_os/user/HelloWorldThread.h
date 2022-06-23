/*****************************************************************************
 *                                                                           *
 *                   H E L L O W O R L D T H R E A D                         *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Ein einfacher Thread.                                    *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 16.12.2016                      *
 *****************************************************************************/
#ifndef __hello_world_thread_include__
#define __hello_world_thread_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class HelloWorldThread : public Thread {
private:
    HelloWorldThread(const HelloWorldThread& copy) = delete;  // Verhindere Kopieren

public:
    HelloWorldThread() : Thread() {
        kout << "Initialized HelloWorldThread" << endl;
    }

    // Thread-Startmethode
    void run() override;
};

#endif
