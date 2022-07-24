/*****************************************************************************
 *                                                                           *
 *                           T E X T D E M O                                 *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Testausgaben für den CGA-Treiber.                        *
 *                                                                           *
 * Autor:           Michael Schoettner, HHU, 26.10.2018                       *
 *****************************************************************************/

#ifndef TextDemo_include__
#define TextDemo_include__

#include "kernel/Globals.h"
#include "kernel/threads/Thread.h"

class TextDemo : public Thread {
public:
    TextDemo(const TextDemo& copy) = delete;

    TextDemo() : Thread("TextDemo") {}

    void run() override;
};

#endif
