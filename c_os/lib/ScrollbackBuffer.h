#ifndef __SCROLLBACKBUFFER_INCLUDE_H_
#define __SCROLLBACKBUFFER_INCLUDE_H_

#include "devices/CGA.h"
#include "lib/MyStdLib.h"
#include <stddef.h>

// NOTE: I added this file

// The buffer dimensions must match with the destination dimensions
class ScrollbackBuffer {
private:
    CGA::cga_page_t* buffer;      // Circular buffer to store lines that left the screen
    CGA::cga_page_t* pagebuffer;  // Contains the current page separately from the scrollback.
                                  // I thought this was easier since it also captures the little output
                                  // generated before the scrollback buffer is initialized by accident
                                  // (but only if it's less than a page)
    unsigned int pos;

public:
    const unsigned int pages;  // Number of pages in buffer
    const unsigned int rows;   // Number of lines in buffer

    ScrollbackBuffer(unsigned char rows, unsigned char pages)
      : pos(0), pages(pages), rows(rows * pages) {
        this->buffer = new CGA::cga_page_t[pages];
        this->pagebuffer = new CGA::cga_page_t;
        this->clear();  // Null out the buffer so no crap gets displayed
    }
    ~ScrollbackBuffer() {
        delete[] this->buffer;
        delete this->pagebuffer;
    }

    void put(CGA::cga_line_t* line);
    void get(CGA::cga_line_t* destination, unsigned char page) const;
    void copy_from_pagebuffer(CGA::cga_page_t* destination) const;
    void copy_to_pagebuffer(CGA::cga_page_t* source);
    void clear();
};

#endif
