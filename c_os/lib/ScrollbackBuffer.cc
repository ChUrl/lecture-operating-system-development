#include "ScrollbackBuffer.h"

// NOTE: I added this file

void ScrollbackBuffer::put(CGA::cga_line_t* line) {
    CGA::cga_line_t* destination = (CGA::cga_line_t*)this->buffer + this->pos;
    mmem::memcpy<CGA::cga_line_t>(destination, line);

    this->pos = (this->pos + 1) % this->rows;
}

void ScrollbackBuffer::get(CGA::cga_line_t* destination, unsigned char page) const {
    if (page < 0 || page >= this->pages) {
        return;
    }

    // We reverse the pagenumber so page 0 is always the newest page
    unsigned char rpage = this->pages - page - 1;

    // Copy linewise because page may wrap around buffer borders
    unsigned int wrapline;
    for (unsigned int line = 0; line < (this->rows / this->pages); ++line) {
        wrapline = (this->pos + rpage * (this->rows / this->pages) + line) % this->rows;
        mmem::memcpy<CGA::cga_line_t>(destination + line, (CGA::cga_line_t*)this->buffer + wrapline);
    }
}

void ScrollbackBuffer::clear() {
    for (unsigned char page = 0; page < this->pages; ++page) {
        mmem::zero<CGA::cga_page_t>(this->buffer + page);
    }
    this->pos = 0;
}
