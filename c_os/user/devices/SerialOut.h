#ifndef __SerialOut_Include_H_
#define __SerialOut_Include_H_

#include "kernel/IOport.h"
#include "user/lib/String.h"

// NOTE: I took this code from https://wiki.osdev.org/Serial_Ports

class SerialOut {
private:
    const IOport com1;

    SerialOut(const SerialOut& copy) = delete;

    int serial_received();
    int is_transmit_empty();

public:
    SerialOut() : com1(0x3f8) {
        this->init();
    }

    int init() const;

    char read();
    void write(char a);
    void write(const char* a);
    void write(const bse::string& a);
};

#endif
