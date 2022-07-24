#ifndef ARRAY_INCLUDE_H
#define ARRAY_INCLUDE_H

#include "user/lib/Iterator.h"
#include <utility>

namespace bse {

    template<typename T, const std::size_t N>
    class array {
    public:
        using iterator = ContinuousIterator<T>;

    private:
        T buf[N];

    public:
        array() = default;;  // If i write default something like bse::array<int, 10> arr; is not initialized...

        array(std::initializer_list<T> list) {
            typename std::initializer_list<T>::iterator it = list.begin();
            for (unsigned int i = 0; i < N; ++i) {
                buf[i] = *it;
                ++it;
            }
        }

        iterator begin() { return iterator(&buf[0]); }
        iterator begin() const { return iterator(&buf[0]); }
        iterator end() { return iterator(&buf[N]); }
        iterator end() const { return iterator(&buf[N]); }

        T& operator[](std::size_t i) { return buf[i]; }
        constexpr const T& operator[](std::size_t i) const { return buf[i]; }

        T* data() { return &buf[0]; }  // Not standard, I don't know yet if this will turn out to be a bad idea
        const T* data() const { return &buf[0]; }

        void swap(array<T, N>& other) {
            for (std::size_t i = 0; i < N; ++i) {
                std::swap(buf[i], other[i]);
            }
        }

        // Array& other has to have size n:
        // arr1.swap_n<5>(arr2) => arr2 has size 5, arr1 has size >= 5
        template<std::size_t n>
        void swap_n(array<T, n>& other) {
            for (std::size_t i = 0; i < n; ++i) {
                std::swap(buf[i], other[i]);
            }
        }

        constexpr std::size_t size() const {
            return N;
        }
    };

}  // namespace bse

#endif
