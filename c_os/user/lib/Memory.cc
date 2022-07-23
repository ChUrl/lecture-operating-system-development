#include "user/lib/Memory.h"

void bse::memset(char* destination, const char value, const unsigned int bytes) {
    for (unsigned int byte = 0; byte < bytes; ++byte) {
        *(destination + byte) = value;
    }
}
