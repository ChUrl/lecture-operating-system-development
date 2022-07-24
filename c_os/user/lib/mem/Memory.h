#ifndef MYSTDLIB_INCLUDE_H_
#define MYSTDLIB_INCLUDE_H_

namespace bse {

    // add using byte or sth to replace char

    template<typename T>
    void memcpy(T* destination, const T* source, const unsigned int count = 1) {
        for (unsigned int i = 0; i < count; ++i) {
            *(destination + i) = *(source + i);
        }
    }

    void memset(char* destination, char value, unsigned int bytes);

    template<typename T>
    void zero(T* destination) {
        memset(reinterpret_cast<char*>(destination), '\0', sizeof(T));
    }

    // void strcpy(char* destination, char* source);
    // unsigned int strlen(char* string);
}  // namespace bse

#endif
