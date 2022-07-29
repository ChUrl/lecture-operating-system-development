#include "user/lib/String.h"
#include "user/lib/mem/Memory.h"

std::size_t bse::strlen(const char* str) {
    const char* current = str;
    while (*current != '\0') { ++current; }
    return current - str;
}

void bse::strncpy(char* destination, std::size_t n, const char* source) {
    memcpy<char>(destination, source, n);
}

// Only compares equal length strings
int bse::strcmp(const char* a, const char* b) {
    std::size_t a_len = strlen(a);
    std::size_t b_len = strlen(b);

    if (a_len < b_len) {
        return -1;
    }
    if (b_len < a_len) {
        return 1;
    }

    for (std::size_t i = 0; i < a_len; ++i) {
        if (a[i] < b[i]) {
            return -1;
        }
        if (b[i] < a[i]) {
            return 1;
        }
    }

    return 0;
}
