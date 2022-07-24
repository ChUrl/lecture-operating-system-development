#ifndef SerialOut_Include_H_
#define SerialOut_Include_H_

#include "kernel/IOport.h"
#include "user/lib/String.h"

// NOTE: I took this code from https://wiki.osdev.org/Serial_Ports

class SerialOut {
private:
    static const IOport com1;

    static int serial_received();
    static int is_transmit_empty();

public:
    SerialOut(const SerialOut& copy) = delete;
    SerialOut();

    static char read();
    static void write(char a);
    static void write(const char* a);
    static void write(const bse::string& a);
};

#endif
