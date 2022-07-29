#ifndef Iterator_Include_H_
#define Iterator_Include_H_

namespace bse {

    // This iterator works for structures where the elements are adjacent in memory.
    template<typename T>
    class ContinuousIterator {
    private:
        T* ptr = nullptr;

    public:
        ContinuousIterator() = delete;

        // Use const_cast as the iterator has to increment the pointer
        ContinuousIterator(const T* ptr) : ptr(const_cast<T*>(ptr)) {}

        ContinuousIterator& operator++() {
            ++ptr;
            return *this;
        }

        ContinuousIterator& operator--() {
            --ptr;
            return *this;
        }

        ContinuousIterator operator+(unsigned int add) {
            return ContinuousIterator(ptr + add);
        }

        ContinuousIterator operator-(unsigned int sub) {
            return ContinuousIterator(ptr - sub);
        }

        // Convenience
        T* operator->() { return ptr; }
        const T* operator->() const { return ptr; }
        T& operator*() { return *ptr; }
        const T& operator*() const { return *ptr; }

        bool operator<(const ContinuousIterator& other) const { return ptr < other.ptr; }
        bool operator<=(const ContinuousIterator& other) const { return ptr <= other.ptr; }
        bool operator>(const ContinuousIterator& other) const { return ptr > other.ptr; }
        bool operator>=(const ContinuousIterator& other) const { return ptr >= other.ptr; }
        bool operator==(const ContinuousIterator& other) const { return ptr == other.ptr; }
        bool operator!=(const ContinuousIterator& other) const { return ptr != other.ptr; }

        template<typename t>
        friend unsigned int distance(const ContinuousIterator<t>& first, const ContinuousIterator<t>& last);
    };

    template<typename T>
    unsigned int distance(const ContinuousIterator<T>& first, const ContinuousIterator<T>& last) {
        return last.ptr - first.ptr;
    }

}  // namespace bse

#endif
