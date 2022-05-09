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

    void displaypage();  // Write the current_page to CGA memory

public:
    BufferedCGA() : CGA(), initialized(false), current_page(0) {};

    void init();                          // Scrollback needs to be initialized after memorymanagement
    unsigned char current_page;           // The page that is displayed
    unsigned int scroll_page_backward();  // Scroll up the page history
    unsigned int scroll_page_forward();   // Scroll down the page history (to the current page)

    void print(char* string, int n, unsigned char attrib = STD_ATTR) override;
    void scrollup() override;
    void clear() override;
};

#endif
