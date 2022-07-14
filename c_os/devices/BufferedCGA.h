#ifndef __BUFFEREDCGA_INCLUDE_H_
#define __BUFFEREDCGA_INCLUDE_H_

#include "devices/CGA.h"
#include "devices/Keyboard.h"
#include "user/ScrollbackBuffer.h"
#include <memory>

// NOTE: I added this file, I will probably replace this in the end by an application
class BufferedCGA : public CGA {
private:
    std::unique_ptr<ScrollbackBuffer> scrollback_buffer;  // Contains previous pages
    std::unique_ptr<CGA::cga_page_t> screen_buffer;       // Contains the current page separately from the scrollback.
    bool initialized;                                     // Don't do ScrollbackBuffer actions if not initialized

    BufferedCGA(const BufferedCGA&) = delete;

public:
    BufferedCGA() : initialized(false), scrollback(0) {}

    unsigned char scrollback;  // The page that is displayed, public to enable page display

    void init(unsigned int pages);  // Scrollback needs to be initialized after memorymanagement
    void display_scrollback();      // Write the current_page to CGA memory
    void scroll_page_backward();    // Scroll up the page history
    void scroll_page_forward();     // Scroll down the page history (to the current page)

    void print(char* string, int n, unsigned char attrib = STD_ATTR) override;
    void scrollup() override;
    void clear() override;
};

#endif
