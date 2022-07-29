#include "user/devices/SerialOut.h"

const IOport SerialOut::com1(0x3f8);

SerialOut::SerialOut() {
    // NOTE: I could add different ports for every register but this was easier as it's that way on OSDev
    com1.outb(1, 0x00);  // Disable all interrupts
    com1.outb(3, 0x80);  // Enable DLAB (set baud rate divisor)
    com1.outb(0x03);     // Set divisor to 3 (lo byte) 38400 baud
    com1.outb(1, 0x00);  //                  (hi byte)
    com1.outb(3, 0x03);  // 8 bits, no parity, one stop bit
    com1.outb(2, 0xC7);  // Enable FIFO, clear them, with 14-byte threshold
    com1.outb(4, 0x0B);  // IRQs enabled, RTS/DSR set
    com1.outb(4, 0x1E);  // Set in loopback mode, test the serial chip
    com1.outb(0xAE);     // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (com1.inb() == 0xAE) {
        // If serial is not faulty set it in normal operation mode
        // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
                com1.outb(4, 0x0F);
    }
}

int SerialOut::serial_received() {
    return com1.inb(5) & 1;
}

int SerialOut::is_transmit_empty() {
    return com1.inb(5) & 0x20;
}

char SerialOut::read() {
    while (serial_received() == 0) {}
    return com1.inb();
}

void SerialOut::write(const char a) {
    while (is_transmit_empty() == 0) {}
    com1.outb(a);
}

void SerialOut::write(const bse::string_view a) {
    for (char current : a) {
        write(current);
    }
}
