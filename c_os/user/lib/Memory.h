#ifndef __MYSTDLIB_INCLUDE_H_
#define __MYSTDLIB_INCLUDE_H_

namespace bse {

    template<typename T>
    void memcpy(T* destination, T* source, unsigned int count = 1) {
        for (unsigned int i = 0; i < count; ++i) {
            *(destination + i) = *(source + i);
        }
    }

    void memset(char* destination, char value, unsigned int bytes);

    template<typename T>
    void zero(T* destination) {
        memset((char*)destination, '\0', sizeof(T));
    }

    // void strcpy(char* destination, char* source);
    // unsigned int strlen(char* string);
}  // namespace bse

#endif
