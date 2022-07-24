#include "user/lib/String.h"
#include "user/lib/mem/Memory.h"

unsigned int bse::strlen(const char* str) {
    const char* current = str;
    while (*current != '\0') { ++current; }
    return current - str;
}

void bse::strncpy(char* destination, unsigned int n, const char* source) {
    memcpy<char>(destination, source, n);
}

// Only compares equal length strings
int bse::strcmp(const char* a, const char* b) {
    const unsigned int a_len = strlen(a);
    const unsigned int b_len = strlen(b);

    if (a_len < b_len) {
        return -1;
    }
    if (b_len < a_len) {
        return 1;
    }

    for (unsigned int i = 0; i < a_len; ++i) {
        if (a[i] < b[i]) {
            return -1;
        }
        if (b[i] < a[i]) {
            return 1;
        }
    }

    return 0;
}
