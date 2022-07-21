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

        virtual AbstractIterator& operator++() = 0;

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

        ContinuousIterator& operator++() override {
            ++this->ptr;
            return *this;
        }

        ContinuousIterator operator+(std::size_t add) {
            this->ptr += add;
            return *this;
        }

        ContinuousIterator operator-(std::size_t sub) {
            this->ptr -= sub;
            return *this;
        }
    };

}  // namespace bse

#endif
