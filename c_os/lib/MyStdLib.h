#ifndef __MYSTDLIB_INCLUDE_H_
#define __MYSTDLIB_INCLUDE_H_

// NOTE: I added this file
// TODO: namespace this

template<typename T>
void mymemcpy(T* destination, T* source, unsigned int count = 1) {
    for (unsigned int i = 0; i < count; ++i) {
        *(destination + i) = *(source + i);
    }
}

void mymemset(char* destination, char value, unsigned int bytes);

template<typename T>
void myzero(T* destination) {
    mymemset((char*)destination, '\0', sizeof(T));
}

void mystrcpy(char* destination, char* source);
unsigned int mystrlen(char* string);

#endif
