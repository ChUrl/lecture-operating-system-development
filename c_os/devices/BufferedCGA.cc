#include "BufferedCGA.h"
#include "kernel/Globals.h"

// Can't initialize in constructor as memory management already needs working CGA for output
// NOTE: This has to be called when memorymanagement is active
void BufferedCGA::init(unsigned int pages) {
    this->scrollback_buffer = std::make_unique<ScrollbackBuffer>(ROWS, pages);  // No delete since it's only off when shutting the os down
    this->screen_buffer = std::make_unique<CGA::cga_page_t>();

    if (this->scrollback_buffer == NULL || this->screen_buffer == NULL) {
        log << ERROR << "Error initializing scrollback buffer" << endl;
        return;
    }

    this->initialized = true;
    log << INFO << "Initialized scrollback buffer" << endl;
}

void BufferedCGA::display_scrollback() {
    if (this->initialized) {
        if (this->scrollback == 0) {
            // Use pagebuffer
            mmem::memcpy<CGA::cga_page_t>((CGA::cga_page_t*)CGA_START, this->screen_buffer.get());
        } else {
            // Use scrollback
            this->scrollback_buffer->get((CGA::cga_line_t*)CGA_START, this->scrollback - 1);
        }
    } else {
        log << DEBUG << "ScrollbackBuffer not initialized" << endl;
    }
}

void BufferedCGA::print(char* string, int n, unsigned char attrib) {
    if (this->scrollback != 0) {
        // Display newest content from buffer when new prints happen

        this->scrollback = 0;
        this->display_scrollback();
    }

    CGA::print(string, n, attrib);
}

void BufferedCGA::scrollup() {
    if (this->initialized) {
        this->scrollback_buffer->put((CGA::cga_line_t*)CGA_START);
    } else {
        log << DEBUG << "ScrollbackBuffer not initialized" << endl;
    }

    CGA::scrollup();
}

void BufferedCGA::clear() {
    CGA::clear();
    this->scrollback = 0;

    if (this->initialized) {
        this->scrollback_buffer->clear();
        mmem::zero<CGA::cga_page_t>(this->screen_buffer.get());
    } else {
        log << DEBUG << "ScrollbackBuffer not initialized" << endl;
    }
}

void BufferedCGA::scroll_page_backward() {
    if (this->initialized) {

        // If this is the first scrollback we have to save the current screen content
        if (this->scrollback == 0) {
            mmem::memcpy<CGA::cga_page_t>(this->screen_buffer.get(), (CGA::cga_page_t*)CGA_START);
        }

        // current_page can be equal to scrollback_buffer->pages
        // as we have a separate pagebuffer for the current screen content
        if (this->scrollback < this->scrollback_buffer->pages) {
            this->scrollback = this->scrollback + 1;
        }
        this->display_scrollback();
    } else {
        log << DEBUG << "ScrollbackBuffer not initialized" << endl;
    }
}

void BufferedCGA::scroll_page_forward() {
    if (this->initialized) {

        if (this->scrollback > 0) {
            this->scrollback = this->scrollback - 1;
        }
        this->display_scrollback();
    } else {
        log << DEBUG << "ScrollbackBuffer not initialized" << endl;
    }
}
