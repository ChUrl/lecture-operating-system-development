#include "ScrollbackBuffer.h"

// NOTE: I added this file

void ScrollbackBuffer::buffer_next_line() {
    this->current_linenumber = this->current_linenumber + 1;
    this->current_linenumber = this->current_linenumber % this->lines;
}

CGA::cga_line_t* ScrollbackBuffer::get_current_line() const {
    return (CGA::cga_line_t*)(this->buffer + this->current_linenumber);
}

void ScrollbackBuffer::line_to_buffer(CGA::cga_line_t* line) {
    mmem::memcpy<CGA::cga_line_t>(this->get_current_line(), line);
    this->buffer_next_line();
}

void ScrollbackBuffer::copy_page_from_buffer(CGA::cga_line_t* destination, unsigned char page) const {
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
    for (unsigned int line = 0; line < this->pageheight; ++line) {
        wrapline = (this->current_linenumber + rpage * this->pageheight + line) % this->lines;
        mmem::memcpy<CGA::cga_line_t>(destination + line, this->buffer + wrapline);
    }
}

void ScrollbackBuffer::copy_page_from_pagebuffer(CGA::cga_page_t* destination) const {
    mmem::memcpy<CGA::cga_page_t>(destination, this->pagebuffer);
}

void ScrollbackBuffer::copy_page_to_pagebuffer(CGA::cga_page_t* source) {
    mmem::memcpy<CGA::cga_page_t>(this->pagebuffer, source);
}

void ScrollbackBuffer::clear() {
    for (unsigned char page = 0; page < this->pages; ++page) {
        mmem::zero<CGA::cga_page_t>((CGA::cga_page_t*)this->buffer + page);
    }
    mmem::zero<CGA::cga_page_t>(this->pagebuffer);
    this->current_linenumber = 0;
}
