#ifndef __LIST_INCLUDE_H_
#define __LIST_INCLUDE_H_

#include "lib/OutStream.h"
#include "user/lib/Iterator.h"

// Define the list interface for ArrayList/LinkedList implementations with support for Iterators/ranged based for loops

template<typename T, typename I = Iterator<T>>
class List {
public:
    using Type = T;      // We make the template argument accessible from the subclasses
    using Iterator = I;  // Needed for range based for loop

    // Iterator
    virtual Iterator begin() = 0;
    virtual Iterator end() = 0;
    constexpr Iterator begin() const { return this->begin(); }
    constexpr Iterator end() const { return this->end(); }

    // Insert
    virtual unsigned int insert_at(Type e, unsigned int i) = 0;
    virtual unsigned int insert_first(Type e) = 0;
    virtual unsigned int insert_last(Type e) = 0;

    // Remove
    virtual Type remove_at(unsigned int i) = 0;
    virtual Type remove_first() = 0;
    virtual Type remove_last() = 0;
    virtual bool remove(Type e) = 0;

    // Get
    virtual Type get(unsigned int i) const = 0;
    virtual Type first() const = 0;
    virtual Type last() const = 0;

    // Misc
    virtual bool empty() const = 0;
    virtual unsigned int size() const = 0;
    virtual void print(OutStream& out) const = 0;
};

#endif
