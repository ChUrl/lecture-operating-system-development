#ifndef __LIST_INCLUDE_H_
#define __LIST_INCLUDE_H_

#include "lib/OutStream.h"
#include "user/lib/Iterator.h"
#include <optional>

// Define the list interface for ArrayList/LinkedList implementations with support for Iterators/ranged based for loops

// NOTE: Can't be used with types that are not movable
//       Also if used with unique_ptr the get methods can't be used as it deletes the pointer inside the list

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
    // NOTE: Copies an element into the structure
    virtual std::size_t insert_at(Type e, unsigned int i) = 0;
    virtual std::size_t insert_first(Type e) = 0;
    virtual std::size_t insert_last(Type e) = 0;

    // Remove
    // NOTE: Moves an element out of the structure
    virtual std::optional<Type> remove_at(std::size_t i) = 0;
    virtual std::optional<Type> remove_first() = 0;
    virtual std::optional<Type> remove_last() = 0;
    virtual bool remove(Type e) = 0;

    // Get
    // TODO: Return c++20 optional references
    virtual std::optional<Type> get(std::size_t i) const = 0;
    virtual std::optional<Type> first() const = 0;
    virtual std::optional<Type> last() const = 0;

    // Misc
    virtual bool empty() const = 0;
    virtual std::size_t size() const = 0;
    virtual void print(OutStream& out) const = 0;
};

#endif
