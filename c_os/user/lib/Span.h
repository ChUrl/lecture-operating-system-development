#ifndef SPAN_INCLUDE_H
#define SPAN_INCLUDE_H

#include "user/lib/Iterator.h"
#include <utility>

namespace bse {

    // 0 is unchecked
    template<typename T, std::size_t N = 0>
    class span {
    public:
        using iterator = ContinuousIterator<T>;

    private:
        T* ptr;
        std::size_t sz = N;

    public:
        span() = default;

        span(T* first) : ptr(first) {}

        span(T* first, T* last) : ptr(first), sz(last - first) {}

        iterator begin() { return iterator(ptr); }
        iterator begin() const { return iterator(ptr); }
        // If size is unchecked end() is equal to begin()
        iterator end() { return iterator(&ptr[N]); }
        iterator end() const { return iterator(&ptr[N]); }

        T* operator[](std::size_t i) {
            if constexpr (N != 0) {
                if (i >= N) { return nullptr; }
            }
            return &ptr[i];
        }

        const T* operator[](std::size_t i) const {
            if constexpr (N != 0) {
                if (i >= N) { return nullptr; }
            }
            return &ptr[i];
        }

        T* data() { return ptr; }
        const T* data() const { return ptr; }

        explicit operator T*() {
            return ptr;
        }

        // First is inclusive, last exclusive [first, last)
        span& subspan(std::size_t first, std::size_t last = N) {
            return span(ptr + first, ptr + last - 1);
        }

        std::size_t size() const {
            return sz;
        }
    };

}  // namespace bse

#endif
