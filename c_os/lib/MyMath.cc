#include "MyMath.h"

// NOTE: I added this file

int mymin(int a, int b) {
    if (a < b) { return a; }
    return b;
}

int mymax(int a, int b) {
    if (a < b) { return b; }
    return a;
}

int myabs(int a) {
    if (a < 0) { return -a; }
    return a;
}

int mypow(int a, unsigned int b) {
    int result = 1;
    for (unsigned int i = 0; i < b; ++i) {
        result = result * a;
    }
    return result;
}
