#include "MyMath.h"

// NOTE: I added this file

int mmath::min(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int mmath::max(int a, int b) {
    if (a < b) { return b; }
    return a;
}

int mmath::abs(int a) {
    if (a < 0) { return -a; }
    return a;
}

int mmath::pow(int a, unsigned int b) {
    int result = 1;
    for (unsigned int i = 0; i < b; ++i) {
        result = result * a;
    }
    return result;
}
