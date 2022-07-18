#ifndef __Iterator_Include_H_
#define __Iterator_Include_H_

// This iterator works for structures where the elements are adjacent in memory.
// For things like LinkedList, the operator++ has to be overriden to implement the traversal.
template<typename T>
class Iterator {
public:
    using Type = T;

protected:
    Type* ptr;

public:
    Iterator(Type* ptr) : ptr(ptr) {}

    // I only implement the least necessary operators
    virtual Iterator& operator++() {
        this->ptr = this->ptr + 1;
        return *this;
    }

    Type* operator->() {
        return this->ptr;
    }

    Type& operator*() {
        return *this->ptr;
    }

    bool operator==(const Iterator& other) const {
        return this->ptr == other.ptr;
    }

    bool operator!=(const Iterator& other) const {
        return !(*this == other);  // Use our == implementation
    }
};

#endif
