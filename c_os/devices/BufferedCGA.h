#ifndef __BUFFEREDCGA_INCLUDE_H_
#define __BUFFEREDCGA_INCLUDE_H_

#include "devices/CGA.h"
#include "devices/Keyboard.h"
#include "lib/ScrollbackBuffer.h"

// NOTE: I added this file

class BufferedCGA : public CGA {
private:
    ScrollbackBuffer* scrollback_buffer;
    bool initialized;  // Don't do ScrollbackBuffer actions if not initialized

    BufferedCGA(const CGA& copy);

    void display_scrollback();  // Write the current_page to CGA memory

public:
    BufferedCGA() : CGA(), initialized(false), scrollback(0) {};
    ~BufferedCGA() {
        if (this->initialized) { delete this->scrollback_buffer; }
    }

    unsigned char scrollback;  // The page that is displayed, public to enable page display

    void init(unsigned int pages);  // Scrollback needs to be initialized after memorymanagement
    void scroll_page_backward();    // Scroll up the page history
    void scroll_page_forward();     // Scroll down the page history (to the current page)

    void print(char* string, int n, unsigned char attrib = STD_ATTR) override;
    void scrollup() override;
    void clear() override;
};

#endif
