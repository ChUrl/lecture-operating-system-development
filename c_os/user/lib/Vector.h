#ifndef __VECTOR_INCLUDE_H_
#define __VECTOR_INCLUDE_H_

// NOTE: I decided to implement this because I wanted some sort of dynamic array (for example for the keyeventmanager).
//       Also I wanted to template the Queue (for the scheduler) but with this I can just replace the Queue and use the
//       ArrayList instead

#include "Iterator.h"
#include "Logger.h"
#include <utility>

// https://en.cppreference.com/w/cpp/container/vector
namespace bse {

    template<typename T>
    class vector {
    public:
        using Iterator = ContinuousIterator<T>;

    private:
        static constexpr const std::size_t default_cap = 10;  // Arbitrary but very small because this isn't a real OS :(
        static constexpr const std::size_t min_cap = 5;       // Slots to allocate extra when array full

        T* buf = nullptr;  // Heap allocated as size needs to change during runtime
                           // Can't use Array for the same reason so we use a C Style array
        std::size_t buf_pos = 0;
        std::size_t buf_cap = 0;

        void init() {
            buf = new T[vector::default_cap];
            buf_cap = vector::default_cap;
        }

        std::size_t get_rem_cap() const {
            return buf_cap - size();
        }

        // Enlarges the buffer if we run out of space
        void min_expand() {
            // Init if necessary
            if (buf == nullptr) {
                init();
                return;  // Dont have to realloc after init
            }

            // Since we only ever add single elements this should never get below zero
            if (get_rem_cap() < min_cap) {
                switch_buf(buf_cap + min_cap);
            }
        }

        // 1. Allocates new buffer
        // 2. Moves stuff to new buffer
        // 3. Deletes old buffer
        // 4. Sets new pos/cap
        void switch_buf(std::size_t cap) {
            // Alloc new array
            T* new_buf = new T[cap];

            // Swap current elements to new array
            for (std::size_t i = 0; i < size(); ++i) {
                new_buf[i] = std::move(buf[i]);
                buf[i].~T();  // TODO: I think delete[] buf calls these, verify that
            }

            // Move new array to buf, deleting the old array
            delete[] buf;
            buf = new_buf;
            buf_cap = cap;
        }

        // Index is location where space should be made
        void copy_right(std::size_t i) {
            if (i >= size()) {
                // We don't need to copy anything as space is already there
                return;
            }

            for (std::size_t idx = size(); idx > i; --idx) {
                buf[idx].~T();                       // Delete previously contained element that will be overridden
                buf[idx] = std::move(buf[idx - 1]);  // This leaves a "shell" of the old object that has to be deleted
                buf[idx - 1].~T();                   // Delete element in moved-out state
            }
        }

        // Index is the location that will be removed
        void copy_left(std::size_t i) {
            if (i >= size()) {
                // We don't need to copy anything as nothing will be overridden
                return;
            }

            for (std::size_t idx = i; idx < size(); ++idx) {
                buf[idx].~T();  // Delete the element that will be overwritten
                buf[idx] = std::move(buf[idx + 1]);
                buf[idx + 1].~T();
            }
        }

    public:
        ~vector() {
            for (std::size_t i; i < size(); ++i) {
                buf[i].~T();  // TODO: I think delete[] buf calls these, verify that
            }
            delete[] buf;
        }

        // Iterator
        Iterator begin() {
            if (buf == nullptr) {
                init();
            }
            return Iterator(&buf[0]);
        }
        Iterator begin() const {
            if (buf == nullptr) {
                init();
            }
            return Iterator(&buf[0]);
        }
        Iterator end() {
            if (buf == nullptr) {
                init();
            }
            return Iterator(&buf[size()]);
        }
        Iterator end() const {
            if (buf == nullptr) {
                init();
            }
            return Iterator(&buf[size()]);
        }

        // Add elements
        // https://en.cppreference.com/w/cpp/container/vector/push_back
        void push_back(const T& copy) {
            if (buf == nullptr) {
                init();
            }

            buf[size()] = copy;
            ++buf_pos;
            min_expand();
        }

        void push_back(T&& move) {
            if (buf == nullptr) {
                init();
            }

            buf[size()] = std::move(move);
            ++buf_pos;
            min_expand();
        }

        // https://en.cppreference.com/w/cpp/container/vector/insert
        // The element will be inserted before the pos iterator, pos can be the end() iterator
        Iterator insert(Iterator pos, const T& copy) {
            std::size_t idx = distance(begin(), pos);  // begin() does init if necessary
            copy_right(idx);                           // nothing will be done if pos == end()
            buf[idx] = copy;
            ++buf_pos;
            min_expand();
            return Iterator(&buf[idx]);
        }

        Iterator insert(Iterator pos, T&& move) {
            std::size_t idx = distance(begin(), pos);  // begin() does init if necessary
            copy_right(idx);
            buf[idx] = std::move(move);
            ++buf_pos;
            min_expand();
            return Iterator(&buf[idx]);
        }

        // Remove elements
        // https://en.cppreference.com/w/cpp/container/vector/erase
        // Returns the iterator after the removed element, pos can't be end() iterator
        Iterator erase(Iterator pos) {
            std::size_t idx = distance(begin(), pos);
            copy_left(idx);
            --buf_pos;
            // shrink();
            return Iterator(&buf[idx]);
        }

        // Access
        T& front() {
            return buf[0];
        }

        const T& front() const {
            return buf[0];
        }

        T& back() {
            return buf[size() - 1];
        }

        const T& back() const {
            return buf[size() - 1];
        }

        T& operator[](std::size_t pos) {
            return buf[pos];
        }

        const T& operator[](std::size_t pos) const {
            return buf[pos];
        }

        // Misc
        bool empty() const {
            return !size();
        }

        std::size_t size() const {
            return buf_pos;
        }

        void clear() {
            while (buf_pos > 0) {
                --buf_pos;
                buf[buf_pos].~T();
            }
        }

        void reserve(std::size_t cap) {
            if (buf == nullptr) {
                init();
            }
            switch_buf(cap);
        }
    };

    // Erase all elements that match a predicate
    // NOTE: pred is no real predicate as one would need closures for this, but we don't have <functional> available
    //       This means the result has to be passed separately and the function differs from the c++20 std::erase_if
    template<typename T, typename arg>
    std::size_t erase_if(vector<T>& vec, arg (*pred)(const T&), arg result) {
        std::size_t erased_els = 0;
        for (typename vector<T>::Iterator it = vec.begin(); it != vec.end(); /*Do nothing*/) {
            if (pred(*it) == result) {
                it = vec.erase(it);  // erase returns the iterator to the next element
                ++erased_els;
            } else {
                ++it;  // move forward when nothing was deleted
            }
        }
        return erased_els;
    }

}  // namespace bse

#endif
