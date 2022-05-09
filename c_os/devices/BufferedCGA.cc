#include "BufferedCGA.h"

// Can't initialize in constructor as memory management already needs working CGA for output
// NOTE: This has to be called when memorymanagement is active
void BufferedCGA::init() {
    this->scrollback_buffer = new ScrollbackBuffer(COLUMNS, ROWS, 5);
    this->initialized = true;

    this->print("\nInitialized scrollback buffer with 5 pages\n\n", 45);
}

void BufferedCGA::displaypage() {
    if (this->current_page == 0) {
        // Use pagebuffer
        this->scrollback_buffer->copy_page_from_pagebuffer((cga_page_t*)CGA_START);
    } else {
        // Use scrollback
        this->scrollback_buffer->copy_page_from_buffer((cga_line_t*)CGA_START, this->current_page - 1);
    }
}

void BufferedCGA::print(char* string, int n, unsigned char attrib) {
    if (this->current_page != 0) {
        // Display newest content from buffer when new prints happen

        this->current_page = 0;
        this->displaypage();
    }

    CGA::print(string, n, attrib);
}

void BufferedCGA::scrollup() {
    if (this->initialized) {
        this->scrollback_buffer->line_to_buffer((cga_line_t*)CGA_START);
    } else {
        this->print("ScrollbackBuffer not initialized\n\n", 34);
    }

    CGA::scrollup();
}

void BufferedCGA::clear() {
    CGA::clear();
    this->current_page = 0;

    if (this->initialized) {
        this->scrollback_buffer->clear();
    } else {
        this->print("ScrollbackBuffer not initialized\n\n", 34);
    }
}

unsigned int BufferedCGA::scroll_page_backward() {
    // If this is the first scrollback we have to save the current screen content
    if (this->current_page == 0) {
        this->scrollback_buffer->copy_page_to_pagebuffer((cga_page_t*)CGA_START);
    }

    // current_page can be equal to scrollback_buffer->pages
    // as we have a separate pagebuffer for the current screen content
    if (this->current_page < this->scrollback_buffer->pages) {
        this->current_page = this->current_page + 1;
    }
    this->displaypage();
    return this->current_page;
}

unsigned int BufferedCGA::scroll_page_forward() {
    if (this->current_page > 0) {
        this->current_page = this->current_page - 1;
    }
    this->displaypage();
    return this->current_page;
}
