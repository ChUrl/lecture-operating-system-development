#ifndef __Iterator_Include_H_
#define __Iterator_Include_H_

#include <cstddef>

namespace bse {

    template<typename T>
    class AbstractIterator {
    public:
        T* ptr;

        // *this is always <= other
        virtual std::size_t dist(const AbstractIterator& other) const = 0;

        AbstractIterator(T* ptr) : ptr(ptr) {}

        T* operator->() { return this->ptr; }
        T& operator*() { return *this->ptr; }
        bool operator==(const AbstractIterator& other) const { return this->ptr == other.ptr; }
        bool operator!=(const AbstractIterator& other) const { return !(*this == other); }

        friend std::size_t distance(const AbstractIterator& first, const AbstractIterator& last) {
            return first.dist(last);
        }
    };

    // This iterator works for structures where the elements are adjacent in memory.
    template<typename T>
    class ContinuousIterator : public AbstractIterator<T> {
    public:
        std::size_t dist(const AbstractIterator<T>& other) const override {
            return other.ptr - this->ptr;
        }

        ContinuousIterator(T* ptr) : AbstractIterator<T>(ptr) {}

        friend ContinuousIterator& operator++(ContinuousIterator& rhs) {
            ++rhs.ptr;
            return rhs;
        }

        friend ContinuousIterator& operator--(ContinuousIterator& rhs) {
            --rhs.ptr;
            return rhs;
        }

        friend ContinuousIterator operator+(ContinuousIterator lhs, std::size_t add) {
            lhs.ptr += add;
            return lhs;
        }

        friend ContinuousIterator operator-(ContinuousIterator lhs, std::size_t sub) {
            lhs.ptr -= sub;
            return lhs;
        }
    };

}  // namespace bse

#endif
