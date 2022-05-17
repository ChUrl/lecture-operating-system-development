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

    // We reverse the pagenumber so page 0 is always the current page
    // (The last written line is always before curent_line
    //  so we need to take the last page if we want the newest lines)
    unsigned char rpage = this->pages - page - 1;

    // Copy linewise because page may wrap around buffer borders

    // Pageheight: 4, Pages: 2      |  page = 0 => rpage = 2 - 0 - 1 = 1
    // LINE 0                       |   - line = 0 => wrapline = (5 + 1 * 4 + 0) % 8 = 1
    // LINE 1                       |   - [...]
    // LINE 2                       |   - line = 3 => wrapline = (5 + 1 * 4 + 3) % 8 = 4
    // LINE 3                       |
    // LINE 4                       |  page = 1 => rpage = 2 - 1 - 1 = 0
    // LINE 5 - current_linenumber  |   - line = 0 => wrapline = (5 + 0 * 4 + 0) % 8 = 5
    // LINE 6                       |   - [...]
    // LINE 7                       |   - line = 3 => wrapline = (5 + 0 * 4 + 3) % 8 = 0
    unsigned int wrapline;
    for (unsigned int line = 0; line < (this->rows / this->pages); ++line) {
        wrapline = (this->pos + rpage * (this->rows / this->pages) + line) % this->rows;
        mmem::memcpy<CGA::cga_line_t>(destination + line, (CGA::cga_line_t*)this->buffer + wrapline);
    }
}

void ScrollbackBuffer::copy_from_pagebuffer(CGA::cga_page_t* destination) const {
    mmem::memcpy<CGA::cga_page_t>(destination, this->pagebuffer);
}

void ScrollbackBuffer::copy_to_pagebuffer(CGA::cga_page_t* source) {
    mmem::memcpy<CGA::cga_page_t>(this->pagebuffer, source);
}

void ScrollbackBuffer::clear() {
    for (unsigned char page = 0; page < this->pages; ++page) {
        mmem::zero<CGA::cga_page_t>(this->buffer + page);
    }
    mmem::zero<CGA::cga_page_t>(this->pagebuffer);
    this->pos = 0;
}
