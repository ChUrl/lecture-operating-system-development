#include "MyStdLib.h"

void mymemset(char* destination, char value, unsigned int bytes) {
    for (unsigned int byte = 0; byte < bytes; ++byte) {
        *(destination + byte) = value;
    }
}
