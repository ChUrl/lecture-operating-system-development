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

    // Array& other has to have size n:
    // arr1.swap_n<5>(arr2) => arr2 has size 5, arr1 has size >= 5
    template<std::size_t n>
    void swap_n(Array<Type, n>& other) {
        for (std::size_t i = 0; i < n; ++i) {
            std::swap(this->buf[i], other[i]);
        }
    }

    constexpr std::size_t size() const {
        return N;
    }
};

#endif
