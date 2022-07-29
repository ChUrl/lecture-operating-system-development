#ifndef MYSTDLIB_INCLUDE_H_
#define MYSTDLIB_INCLUDE_H_

#include <utility>

namespace bse {

    // add using byte or sth to replace char

    template<typename T>
    void memcpy(T* destination, const T* source, std::size_t count = 1) {
        for (unsigned int i = 0; i < count; ++i) {
            *(destination + i) = *(source + i);
        }
    }

    void memset(char* destination, char value, std::size_t bytes);

    template<typename T>
    void zero(T* destination) {
        memset(reinterpret_cast<char*>(destination), '\0', sizeof(T));
    }

}  // namespace bse

#endif
