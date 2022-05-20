#ifndef __SCROLLBACKBUFFER_INCLUDE_H_
#define __SCROLLBACKBUFFER_INCLUDE_H_

#include "devices/CGA.h"
#include "lib/MyStdLib.h"
#include <stddef.h>

// NOTE: I added this file

class ScrollbackBuffer {
private:
    CGA::cga_page_t* buffer;  // Circular buffer to store lines that left the screen
    unsigned int pos;         // Buffer write position

    ScrollbackBuffer(const ScrollbackBuffer&) = delete;

public:
    const unsigned int pages;  // Number of pages in buffer
    const unsigned int rows;   // Number of lines in buffer

    ScrollbackBuffer(unsigned char rows, unsigned char pages)
      : pos(0), pages(pages), rows(rows * pages) {
        this->buffer = new CGA::cga_page_t[pages];  // Allocate with new because it's quite large
        this->clear();                              // Null out the buffer so no crap gets displayed
    }
    ~ScrollbackBuffer() {
        delete[] this->buffer;
    }

    void put(CGA::cga_line_t* line);
    void get(CGA::cga_line_t* destination, unsigned char page) const;
    void clear();
};

#endif
