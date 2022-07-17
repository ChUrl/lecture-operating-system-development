#ifndef __LIST_INCLUDE_H_
#define __LIST_INCLUDE_H_

#include "lib/OutStream.h"

// Define the list interface for ArrayList/LinkedList implementations with support for Iterators/ranged based for loops

// TODO: Does it only work on lists?
// This iterator works for structures where the elements are adjacent in memory.
// For things like LinkedList, the operator++ has to be overriden to implement the traversal.
template<typename T>
class ListIterator {
public:
    using Type = T;

protected:
    Type* ptr;

public:
    ListIterator(Type* ptr) : ptr(ptr) {}

    // I only implement the least necessary operators
    virtual ListIterator& operator++() {
        this->ptr = this->ptr + 1;
        return *this;
    }

    Type* operator->() {
        return this->ptr;
    }

    Type& operator*() {
        return *this->ptr;
    }

    bool operator==(const ListIterator& other) const {
        return this->ptr == other.ptr;
    }

    bool operator!=(const ListIterator& other) const {
        return !(*this == other);  // Use our == implementation
    }
};

template<typename T, typename I = ListIterator<T>>
class List {
public:
    using Type = T;      // We make the template argument accessible from the subclasses
    using Iterator = I;  // Needed for range based for loop

protected:
    virtual typename Iterator::Type* begin_ptr() = 0;
    virtual typename Iterator::Type* end_ptr() = 0;

public:
    Iterator begin() { return Iterator(this->begin_ptr()); }
    Iterator end() { return Iterator(this->end_ptr()); }

    virtual unsigned int insert_at(Type e, unsigned int i) = 0;
    virtual unsigned int insert_first(Type e) = 0;
    virtual unsigned int insert_last(Type e) = 0;

    virtual Type remove_at(unsigned int i) = 0;
    virtual Type remove_first() = 0;
    virtual Type remove_last() = 0;
    virtual bool remove(Type e) = 0;

    virtual Type get(unsigned int i) const = 0;
    virtual Type first() const = 0;
    virtual Type last() const = 0;

    virtual bool empty() const = 0;
    virtual unsigned int size() const = 0;

    virtual void print(OutStream& out) const = 0;
};

#endif
