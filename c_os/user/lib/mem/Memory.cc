#include "Memory.h"

void bse::memset(char* destination, const char value, std::size_t bytes) {
    for (std::size_t byte = 0; byte < bytes; ++byte) {
        *(destination + byte) = value;
    }
}
