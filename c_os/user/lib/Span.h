#ifndef SPAN_INCLUDE_H
#define SPAN_INCLUDE_H

#include "user/lib/Iterator.h"
#include <utility>

namespace bse {

    // 0 is unchecked
    template<typename T, const std::size_t N = 0>
    class span {
    public:
        using iterator = ContinuousIterator<T>;

    private:
        const T* ptr;
        std::size_t sz = N;

    public:
        span() = default;

        span(const T* first) : ptr(first) {}

        span(const T* first, const T* last) : ptr(first), sz(last - first) {}

        iterator begin() { return iterator(&ptr[0]); }
        iterator begin() const { return iterator(&ptr[0]); }
        // If size is unchecked end() is equal to begin()
        iterator end() { return iterator(&ptr[N]); }
        iterator end() const { return iterator(&ptr[N]); }

        constexpr T& operator[](std::size_t i) {
            if constexpr (N != 0 && i >= N) {
                return nullptr;
            }
            return ptr[i];
        }

        constexpr const T& operator[](std::size_t i) const {
            if constexpr (N != 0 && i >= N) {
                return nullptr;
            }
            return ptr[i];
        }

        T* data() { return &ptr[0]; }
        const T* data() const { return &ptr[0]; }

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
