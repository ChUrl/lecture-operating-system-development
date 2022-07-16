#ifndef __ARRAYLIST_INCLUDE_H_
#define __ARRAYLIST_INCLUDE_H_

// NOTE: I decided to implement this because I wanted some sort of dynamic array (for example for the keyeventmanager).
//       Also I wanted to template the Queue (for the scheduler) but with this I can just replace the Queue and use the
//       ArrayList instead, without additional effort.
//       It's also cool to use the allocator a bit more and introduce realloc because I coded that thing

#include "user/lib/List.h"
#include <cstddef>

// I put most of the implementation in the header because the templating makes it cumbersome to split
template<typename T>
class ArrayList : public List<T> {
private:
    const unsigned int default_size = 10;  // Arbitrary but very small because this isn't a real OS :(
    const unsigned int expand_size = 5;    // Slots to allocate extra when array full

    unsigned int buffer_size = 0;
    unsigned int buffer_pos = 0;

    // TODO: Use user/lib/Array
    // I manage the size so I can use pointer arithmetic
    T* buffer = NULL;

    void init() {
        this->buffer = new T[this->default_size];
        this->buffer_size = this->default_size;
    }

    unsigned int get_free_space() const {
        return this->buffer_size - this->buffer_pos;
    }

    // Enlarges the buffer if we run out of space
    unsigned int expand() {
        // Init if necessary
        if (this->buffer == NULL && this->buffer_size == 0) {
            this->init();
            return this->buffer_size;  // Dont have to realloc after init
        }

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
    typename List<T>::Iterator begin() override {
        return typename List<T>::Iterator(&this->buffer[0]);
    }

    typename List<T>::Iterator end() override {
        return typename List<T>::Iterator(&this->buffer[this->buffer_pos]);
    }

    // Returns new pos
    unsigned int insert(T e) override {
        this->expand();
        this->buffer[this->buffer_pos] = e;
        this->buffer_pos = this->buffer_pos + 1;

        return this->buffer_pos;
    }

    unsigned int insert_at(T e, unsigned int i) override {
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
    T remove_at(unsigned int i) override {
        if (i >= this->buffer_pos) {
            // ERROR: No element here
            return NULL;
        }

        T e = this->buffer[i];
        this->copy_left(i);
        return e;
    }

    T remove_first() override {
        return this->remove_at(0);
    }

    T remove_last() override {
        // If index -1 unsigned int will overflow and remove_at will catch that
        return this->remove_at(this->buffer_pos - 1);
    }

    // Returns true on success
    bool remove(T e) override {
        for (unsigned int i = 0; i < this->buffer_pos; ++i) {
            if (this->buffer[i] == e) {
                this->copy_left(i);
                return true;
            }
        }

        return false;
    }

    T get(unsigned int i) const override {
        if (i >= this->buffer_pos) {
            // ERROR: No element there
            return NULL;
        }

        return this->buffer[i];
    }

    T first() const override {
        return this->get(0);
    }

    T last() const override {
        return this->get(this->buffer_pos - 1);  // Underflow gets catched by get(unsigned int i)
    }

    bool empty() const override {
        return this->buffer_pos == 0;
    }

    unsigned int size() const override {
        return this->buffer_pos;
    }

    void print(OutStream& out) const override {
        if (this->buffer_pos == 0) {
            out << "Print List (0 elements)" << endl;
            return;
        }

        out << "Print List (" << dec << this->buffer_pos << " elements): ";
        for (unsigned int i = 0; i < this->buffer_pos; ++i) {
            out << dec << this->get(i) << " ";
        }
        out << endl;
    }
};

#endif
