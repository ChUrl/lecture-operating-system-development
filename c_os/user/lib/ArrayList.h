#ifndef __ARRAYLIST_INCLUDE_H_
#define __ARRAYLIST_INCLUDE_H_

// NOTE: I decided to implement this because I wanted some sort of dynamic array (for example for the keyeventmanager).
//       Also I wanted to template the Queue (for the scheduler) but with this I can just replace the Queue and use the
//       ArrayList instead, without additional effort.

#include "user/lib/List.h"
#include "user/lib/mem/UniquePointer.h"
#include <cstddef>
#include <type_traits>
#include <utility>

// I put most of the implementation in the header because the templating makes it cumbersome to split

template<typename T>
class ArrayList : public List<T> {
public:
    using Type = typename List<T>::Type;  // Use this just in case T changes from the List type
    using Iterator = typename List<T>::Iterator;

private:
    static constexpr const std::size_t default_cap = 10;  // Arbitrary but very small because this isn't a real OS :(
    static constexpr const std::size_t min_cap = 5;       // Slots to allocate extra when array full

    bse::unique_ptr<Type[]> buf;  // Heap allocated as size needs to change during runtime
                                  // Can't use Array for the same reason so we use a C Style array
                                  // NOTE: I wouldn't normally use smart pointers for low level datastructures
                                  //       but here it doesn't hurt as the unique_ptr basically has no overhead
                                  //       (And it saved 2 deletes ¯\_(ツ)_/¯)
    std::size_t buf_pos = 0;
    std::size_t buf_cap = 0;

    void init() {
        buf = bse::make_unique<Type[]>(ArrayList::default_cap);
        buf_cap = ArrayList::default_cap;
    }

    std::size_t get_rem_cap() const {
        return buf_cap - size();
    }

    // Enlarges the buffer if we run out of space
    std::size_t expand() {
        // Init if necessary
        if (!buf) {
            init();
            return buf_cap;  // Dont have to realloc after init
        }

        // Since we only ever add single elements this should never get below zero
        if (get_rem_cap() < min_cap) {
            std::size_t new_cap = buf_cap + min_cap;

            // Alloc new array
            bse::unique_ptr<Type[]> new_buf = bse::make_unique<Type[]>(new_cap);

            // Swap current elements to new array
            for (std::size_t i = 0; i < size(); ++i) {
                new_buf[i] = std::move(buf[i]);  // Should I have just used a regular pointer?
            }

            // Move new array to buf, deleting the old array
            buf = std::move(new_buf);
            buf_cap = new_cap;
        }

        return buf_cap;
    }

    // unsigned int shrink {}

    // Returns new pos, both do element copying if necessary, -1 if failed
    // Index is location where space should be made/removed
    std::size_t copy_right(std::size_t i) {
        if (i > size()) {
            // Error: No elements here
            return -1;
        }

        expand();

        // Otherwise i == pos and we don't need to copy anything
        if (i < size()) {
            // Enough space to copy elements after pos i
            // Copy to the right to make space
            //
            // [0 1 2 3 _], expand(0) => [_ 0 1 2 3 _]
            //  ^       |                           |
            // [0 1 2 3 _], expand(1) => [0 _ 1 2 3 _]
            //    ^     |                           |
            //         pos = 4                     pos = 5
            for (std::size_t idx = size(); idx > i; --idx) {  // idx > i so idx - 1 is never < 0
                buf[idx] = std::move(buf[idx - 1]);
            }

            // Only change pos if elements were copied
            ++buf_pos;
        }

        return size();
    }

    // Don't realloc here, we don't need to shring the buffer every time
    // One could introduce a limit of free space but I don't care for now
    // Would be bad if the scheduler triggers realloc everytime a thread is removed (if used as readyqueue)...
    std::size_t copy_left(std::size_t i) {
        if (i >= size()) {
            // Error: No elements here
            return -1;
        }

        // Decrement before loop because we overwrite 1 element (1 copy less than expand)
        --buf_pos;

        // [0 1 2 3 _], shrink(1) => [0 2 3 _]
        //    ^   |                       |
        //       pos = 3                 pos = 2
        for (std::size_t idx = i; idx < size(); ++idx) {  // idx < pos so idx + 1 is never outside of size limit
            buf[idx] = std::move(buf[idx + 1]);
        }

        return size();
    }

public:
    ~ArrayList() = default;  // Buffer deletes itself (I hope)

    Iterator begin() override {
        return Iterator(&buf[0]);
    }

    Iterator end() override {
        return Iterator(&buf[size()]);
    }

    // Returns new pos
    std::size_t insert_at(Type e, std::size_t i) override {
        if (i > size()) {
            // Error: Space between elements
            return -1;
        }

        if (i == size()) {
            // Insert at end
            return insert_last(std::forward<Type>(e));
        }

        copy_right(i);  // Changes pos
        buf[i] = e;

        return size();
    }

    std::size_t insert_first(Type e) override {
        return insert_at(std::forward<Type>(e), 0);
    }

    std::size_t insert_last(Type e) override {
        expand();
        buf[size()] = e;
        ++buf_pos;

        return size();
    }

    // Returns removed element
    std::optional<Type> remove_at(std::size_t i) override {
        if (i >= size()) {
            // ERROR: No element here
            return std::nullopt;
        }

        Type e = std::move(buf[i]);
        copy_left(i);
        return e;
    }

    std::optional<Type> remove_first() override {
        return remove_at(0);
    }

    std::optional<Type> remove_last() override {
        // If index -1 unsigned int will overflow and remove_at will catch that
        return remove_at(size() - 1);
    }

    // Returns true on success
    bool remove(Type e) override {
        for (std::size_t i = 0; i < size(); ++i) {
            if (buf[i] == e) {
                copy_left(i);
                return true;
            }
        }

        return false;
    }

    // TODO: All gets should be optional references (c++20)
    std::optional<Type> get(std::size_t i) const override {
        if (i >= size()) {
            // ERROR: No element there
            return std::nullopt;
        }

        return buf[i];
    }

    std::optional<Type> first() const override {
        return get(0);
    }

    std::optional<Type> last() const override {
        return get(size() - 1);  // Underflow gets catched by get(unsigned int i)
    }

    bool empty() const override {
        return !size();
    }

    std::size_t size() const override {
        return buf_pos;
    }

    void print(OutStream& out) const override {
        // Our stream cannot print all types so enable this only for debugging purposes (only int)
        if constexpr (std::is_same<Type, int>::value) {
            if (empty()) {
                out << "Print List (0 elements)" << endl;
                return;
            }

            out << "Print List (" << dec << size() << " elements): ";
            for (std::size_t i = 0; i < size(); ++i) {
                out << dec << buf[i] << " ";
            }
            out << endl;
        }
    }
};

#endif
