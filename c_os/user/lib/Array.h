#ifndef __ARRAY_INCLUDE_H
#define __ARRAY_INCLUDE_H

#include "user/lib/Iterator.h"
#include <cstddef>
#include <utility>

template<typename T, const std::size_t N, typename I = Iterator<T>>
class Array {
public:
    using Type = T;
    using Iterator = I;

private:
    Type buf[N];

public:
    Iterator begin() { return Iterator(&buf[0]); }
    Iterator end() { return Iterator(&buf[N]); }
    constexpr Iterator begin() const { return this->begin(); }
    constexpr Iterator end() const { return this->end(); }

    constexpr unsigned int size() const {
        return N;
    }

    T& operator[](std::size_t i) {
        return this->buf[i];
    }

    constexpr const T& operator[](std::size_t i) const {
        return this->buf[i];
    }

    void swap(Array<Type, N>& other) {
        for (std::size_t i = 0; i < N; ++i) {
            std::swap(this->buf[i], other[i]);
        }
    }
};

#endif
