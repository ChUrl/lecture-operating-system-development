#ifndef __Iterator_Include_H_
#define __Iterator_Include_H_

#include <cstddef>

namespace bse {

    // This iterator works for structures where the elements are adjacent in memory.
    template<typename T>
    class ContinuousIterator {
    private:
        T* ptr;

    public:
        ContinuousIterator(T* ptr) : ptr(ptr) {}

        ContinuousIterator& operator++() {
            ++this->ptr;
            return *this;
        }

        ContinuousIterator& operator--() {
            --this->ptr;
            return *this;
        }

        ContinuousIterator operator+(std::size_t add) {
            return ContinuousIterator(this->ptr + add);
        }

        ContinuousIterator operator-(std::size_t sub) {
            return ContinuousIterator(this->ptr - sub);
        }

        // Convenience
        T* operator->() { return this->ptr; }
        const T* operator->() const { return this->ptr; }
        T& operator*() { return *this->ptr; }
        const T& operator*() const { return *this->ptr; }

        bool operator<(const ContinuousIterator& other) const { return this->ptr < other.ptr; }
        bool operator<=(const ContinuousIterator& other) const { return this->ptr <= other.ptr; }
        bool operator>(const ContinuousIterator& other) const { return this->ptr > other.ptr; }
        bool operator>=(const ContinuousIterator& other) const { return this->ptr >= other.ptr; }
        bool operator==(const ContinuousIterator& other) const { return this->ptr == other.ptr; }
        bool operator!=(const ContinuousIterator& other) const { return this->ptr != other.ptr; }

        template<typename t>
        friend std::size_t distance(const ContinuousIterator<t>& first, const ContinuousIterator<t>& last);
    };

    template<typename T>
    std::size_t distance(const ContinuousIterator<T>& first, const ContinuousIterator<T>& last) {
        return last.ptr - first.ptr;
    }

}  // namespace bse

#endif
