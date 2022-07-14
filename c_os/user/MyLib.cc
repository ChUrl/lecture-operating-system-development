#include "user/MyLib.h"

void mmem::memset(char* destination, char value, unsigned int bytes) {
    for (unsigned int byte = 0; byte < bytes; ++byte) {
        *(destination + byte) = value;
    }
}
