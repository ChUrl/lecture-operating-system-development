#ifndef __Iterator_Include_H_
#define __Iterator_Include_H_

namespace bse {

    // This iterator works for structures where the elements are adjacent in memory.
    template<typename T>
    class ContinuousIterator {
    private:
        T* ptr = nullptr;

        ContinuousIterator() = delete;

    public:
        ContinuousIterator(T* ptr) : ptr(ptr) {}

        T* get() const {
            return ptr;
        }

        ContinuousIterator& operator++() {
            ++this->ptr;
            return *this;
        }

        ContinuousIterator& operator--() {
            --this->ptr;
            return *this;
        }

        ContinuousIterator operator+(unsigned int add) {
            return ContinuousIterator(this->ptr + add);
        }

        ContinuousIterator operator-(unsigned int sub) {
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
        friend unsigned int distance(const ContinuousIterator<t>& first, const ContinuousIterator<t>& last);
    };

    template<typename T>
    unsigned int distance(const ContinuousIterator<T>& first, const ContinuousIterator<T>& last) {
        return last.ptr - first.ptr;
    }

}  // namespace bse

#endif
