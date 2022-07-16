#ifndef __ARRAYLIST_INCLUDE_H_
#define __ARRAYLIST_INCLUDE_H_

// NOTE: I decided to implement this because I wanted some sort of dynamic array (for example for the keyeventmanager).
//       Also I wanted to template the Queue (for the scheduler) but with this I can just replace the Queue and use the
//       ArrayList instead, without additional effort.
//       It's also cool to use the allocator a bit more and introduce realloc because I coded that thing

#include "lib/OutStream.h"
#include <cstddef>

// I put the whole implementation in the header because the templating makes it cumbersome to split
template<typename T>
class ArrayList {
private:
    const unsigned int default_size = 10;  // Arbitrary but very small because this isn't a real OS :(
    const unsigned int expand_size = 5;    // Slots to allocate extra when array full

    unsigned int buffer_size = 10;
    unsigned int buffer_pos = 0;

    // TODO: Use user/lib/Array
    // I manage the size so I can use pointer arithmetic
    T* buffer;

    unsigned int get_free_space() const {
        return this->buffer_size - this->buffer_pos;
    }

    // Enlarges the buffer if we run out of space
    unsigned int expand() {
        // Since we only ever add single elements this should never get below zero
        // TODO: realloc
        if (this->get_free_space() < this->expand_size) {
            // We need to realloc the buffer
            const unsigned int new_size = this->buffer_size + this->expand_size;
            T* new_buffer = new T[new_size];
            // TODO: Use move semantics and managed pointers
            for (unsigned int idx = 0; idx < this->buffer_pos; ++idx) {
                new_buffer[idx] = this->buffer[idx];
            }
            delete[] this->buffer;
            this->buffer = new_buffer;
            this->buffer_size = new_size;
        }

        return this->buffer_size;
    }

    // unsigned int shrink {}

    // Returns new pos, both do element copying if necessary, -1 if failed
    // Index is location where space should be made/removed
    unsigned int copy_right(unsigned int i) {
        if (i > this->buffer_pos) {
            // Error: No elements here
            return -1;
        }

        this->expand();

        // Otherwise i == this->pos and we don't need to copy anything
        if (i < this->buffer_pos) {
            // Enough space to copy elements after pos i
            // Copy to the right to make space
            //
            // [0 1 2 3 _], expand(0) => [_ 0 1 2 3 _]
            //  ^       |                           |
            // [0 1 2 3 _], expand(1) => [0 _ 1 2 3 _]
            //    ^     |                           |
            //         pos = 4                     pos = 5
            for (unsigned int idx = this->buffer_pos; idx > i; --idx) {  // idx > i so idx - 1 is never < 0
                this->buffer[idx] = this->buffer[idx - 1];
            }

            // Only change pos if elements were copied
            this->buffer_pos = this->buffer_pos + 1;
        }

        return this->buffer_pos;
    }

    // Don't realloc here, we don't need to shring the buffer every time
    // One could introduce a limit of free space but I don't care for now
    // Would be bad if the scheduler triggers realloc everytime a thread is removed (if used as readyqueue)...
    unsigned int copy_left(unsigned int i) {
        if (i >= this->buffer_pos) {
            // Error: No elements here
            return -1;
        }

        // Decrement before loop because we overwrite 1 element (1 copy less than expand)
        this->buffer_pos = this->buffer_pos - 1;

        // [0 1 2 3 _], shrink(1) => [0 2 3 _]
        //    ^   |                       |
        //       pos = 3                 pos = 2
        for (unsigned int idx = i; idx < this->buffer_pos; ++idx) {  // idx < pos so idx + 1 is never outside of size limit
            this->buffer[idx] = this->buffer[idx + 1];
        }

        return this->buffer_pos;
    }

public:
    void init() {
        this->buffer = new T[this->default_size];
    }

    // Returns new pos
    unsigned int insert(T e) {
        this->expand();
        this->buffer[this->buffer_pos] = e;
        this->buffer_pos = this->buffer_pos + 1;

        return this->buffer_pos;
    }

    unsigned int insert_at(T e, unsigned int i) {
        if (i > this->buffer_pos) {
            // Error: Space between elements
            return -1;
        }

        if (i == this->buffer_pos) {
            // Insert at end
            this->insert(e);
            return this->buffer_pos;
        }

        this->copy_right(i);  // Changes pos
        this->buffer[i] = e;

        return this->buffer_pos;
    }

    // Returns removed element
    T remove_at(unsigned int i) {
        if (i >= this->buffer_pos) {
            // ERROR: No element here
            return NULL;
        }

        T e = this->buffer[i];
        this->copy_left(i);
        return e;
    }

    T remove_first() {
        return this->remove_at(0);
    }

    T remove_last() {
        // If index -1 unsigned int will overflow and remove_at will catch that
        return this->remove_at(this->buffer_pos - 1);
    }

    int remove(T e) {
        for (unsigned int i = 0; i < this->buffer_pos; ++i) {
            if (this->buffer[i] == e) {
                this->copy_left(i);
                return 0;
            }
        }

        return -1;
    }

    T get(unsigned int i) const {
        if (i >= this->buffer_pos) {
            // ERROR: No element there
            return NULL;
        }

        return this->buffer[i];
    }

    T first() const {
        return this->get(0);
    }

    bool empty() const {
        return this->buffer_pos == 0;
    }

    unsigned int size() const {
        return this->buffer_pos;
    }

    void print(OutStream& out) const;
};

#endif
