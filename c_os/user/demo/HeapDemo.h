/*****************************************************************************
 *                                                                           *
 *                            H E A P D E M O                                *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Demonstration der dynamischen Speicherverwaltung.        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 25.9.2016                       *
 *****************************************************************************/
#ifndef __HeapDemo_include__
#define __HeapDemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class MyObj {
public:
    constexpr MyObj() : value(5) {};
    constexpr MyObj(const unsigned int val) : value(val) {};
    const unsigned int value;
};

class HeapDemo : public Thread {
private:
    HeapDemo(const HeapDemo& copy) = delete;

public:
    HeapDemo() {
        kout << "Initialized HeapDemo" << endl;
    }

    void run() override;
};


#endif
