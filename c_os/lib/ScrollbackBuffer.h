#ifndef __SCROLLBACKBUFFER_INCLUDE_H_
#define __SCROLLBACKBUFFER_INCLUDE_H_

#include "devices/CGA.h"
#include "lib/MyStdLib.h"
#include <memory>
#include <stddef.h>

// NOTE: I added this file

class ScrollbackBuffer {
private:
    std::unique_ptr<CGA::cga_page_t[]> buffer;  // Circular buffer to store lines that left the screen
    unsigned int pos;                           // Buffer write position

    ScrollbackBuffer(const ScrollbackBuffer&) = delete;

public:
    const unsigned int pages;  // Number of pages in buffer
    const unsigned int rows;   // Number of lines in buffer

    ScrollbackBuffer(unsigned char rows, unsigned char pages)
      : pos(0), pages(pages), rows(rows * pages) {
        this->buffer = std::make_unique<CGA::cga_page_t[]>(pages);  // Allocate with new because it's quite large (and I want to use the allocator)
        this->clear();                                              // Null out the buffer so no crap gets displayed
    }

    void put(CGA::cga_line_t* line);
    void get(CGA::cga_line_t* destination, unsigned char page) const;
    void clear();
};

#endif
