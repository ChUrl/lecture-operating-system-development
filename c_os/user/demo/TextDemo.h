/*****************************************************************************
 *                                                                           *
 *                           T E X T D E M O                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Testausgaben für den CGA-Treiber.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 26.10.2018                       *
 *****************************************************************************/

#ifndef __TextDemo_include__
#define __TextDemo_include__

#include "kernel/threads/Thread.h"
#include "kernel/Globals.h"

class TextDemo : public Thread {
private:
    TextDemo(const TextDemo& copy) = delete;

public:
    TextDemo() {
        kout << "Initialized TextDemo" << endl;
    }

    void run() override;
};

#endif
