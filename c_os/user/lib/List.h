#ifndef __LIST_INCLUDE_H_
#define __LIST_INCLUDE_H_

#include "lib/OutStream.h"

// Define the list interface for ArrayList/LinkedList implementations with support for Iterators/ranged based for loops

template<typename List>
class ListIterator {
private:
    using ValType = typename List::ValType;

    ValType* ptr;

public:
    ListIterator(ValType* ptr) : ptr(ptr) {}

    // I only implement the least necessary operators
    ListIterator& operator++() {
        this->ptr = this->ptr + 1;
        return *this;
    }

    ValType* operator->() {
        return this->ptr;
    }

    ValType& operator*() {
        return *this->ptr;
    }

    bool operator==(const ListIterator& other) const {
        return this->ptr == other.ptr;
    }

    bool operator!=(const ListIterator& other) const {
        return !(*this == other);  // Use our == implementation
    }
};

template<typename T>
class List {
public:
    using ValType = T;
    using Iterator = ListIterator<List<T>>;

    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;

    virtual unsigned int insert(T e) = 0;
    virtual unsigned int insert_at(T e, unsigned int i) = 0;

    virtual T remove_at(unsigned int i) = 0;
    virtual T remove_first() = 0;
    virtual T remove_last() = 0;
    virtual bool remove(T e) = 0;

    virtual T get(unsigned int i) const = 0;
    virtual T first() const = 0;
    virtual T last() const = 0;

    virtual bool empty() const = 0;
    virtual unsigned int size() const = 0;

    virtual void print(OutStream& out) const = 0;
};

#endif
