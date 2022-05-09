#ifndef __SCROLLBACKBUFFER_INCLUDE_H_
#define __SCROLLBACKBUFFER_INCLUDE_H_

#include "devices/CGA.h"
#include "lib/MyStdLib.h"
#include <stddef.h>

// NOTE: I added this file

// The buffer dimensions must match with the destination dimensions
class ScrollbackBuffer {
private:
    CGA::cga_line_t* buffer;      // Circular buffer to store lines that left the screen
    CGA::cga_page_t* pagebuffer;  // Contains the current page separately from the scrollback
    unsigned int current_linenumber;

    CGA::cga_line_t* get_current_line() const;  // Translate linenumber to memory location
    void buffer_next_line();                    // Moves the current_line one line forward

public:
    const unsigned int pageheight,  // Number of lines per page
      pages,                        // Number of pages in buffer
      lines;                        // Total number of lines in buffer

    ScrollbackBuffer(unsigned char width, unsigned char height, unsigned char pages)
      : current_linenumber(0), pageheight(height), pages(pages), lines(height * pages) {
        this->buffer = new CGA::cga_line_t[lines];
        this->pagebuffer = new CGA::cga_page_t;
        this->clear();  // Null out the buffer so no crap gets displayed
    }

    void line_to_buffer(CGA::cga_line_t* line);
    void copy_page_from_buffer(CGA::cga_line_t* destination, unsigned char page) const;
    void copy_page_from_pagebuffer(CGA::cga_page_t* destination) const;
    void copy_page_to_pagebuffer(CGA::cga_page_t* source);
    void clear();
};

#endif
