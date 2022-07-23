#include "user/lib/Memory.h"

void bse::memset(char* destination, char value, unsigned int bytes) {
    for (unsigned int byte = 0; byte < bytes; ++byte) {
        *(destination + byte) = value;
    }
}
