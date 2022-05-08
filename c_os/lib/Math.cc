#include "Math.h"

// NOTE: I added this file

int min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int max(int a, int b) {
    if (a < b) { return b; }
    return a;
}

int abs(int a) {
    if (a < 0) { return -a; }
    return a;
}

int pow(int a, unsigned int b) {
    int result = 1;
    for (unsigned int i = 0; i < b; ++i) {
        result = result * a;
    }
    return result;
}
