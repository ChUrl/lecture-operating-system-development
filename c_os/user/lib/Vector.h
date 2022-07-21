#ifndef __VECTOR_INCLUDE_H_
#define __VECTOR_INCLUDE_H_

// NOTE: I decided to implement this because I wanted some sort of dynamic array (for example for the keyeventmanager).
//       Also I wanted to template the Queue (for the scheduler) but with this I can just replace the Queue and use the
//       ArrayList instead

#include "Iterator.h"
#include <cstddef>
#include <utility>

// https://en.cppreference.com/w/cpp/container/vector

namespace bse {

    template<typename T>
    class Vector {
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
            buf = new T[Vector::default_cap];
            buf_cap = Vector::default_cap;
        }

        std::size_t get_rem_cap() const {
            return buf_cap - size();
        }

        // Enlarges the buffer if we run out of space
        std::size_t expand() {
            // Init if necessary
            if (buf == nullptr) {
                init();
                return buf_cap;  // Dont have to realloc after init
            }

            // Since we only ever add single elements this should never get below zero
            if (get_rem_cap() < min_cap) {
                std::size_t new_cap = buf_cap + min_cap;

                // Alloc new array
                T* new_buf = new T[new_cap];

                // Swap current elements to new array
                for (std::size_t i = 0; i < size(); ++i) {
                    new_buf[i] = std::move(buf[i]);
                    buf[i].~T();
                }

                // Move new array to buf, deleting the old array
                delete[] buf;
                buf = new_buf;
                buf_cap = new_cap;
            }

            return buf_cap;
        }

        // Returns new pos, both do element copying if necessary, -1 if failed
        // Index is location where space should be made/removed
        std::size_t copy_right(std::size_t i) {
            if (i > size()) {
                // Error: No elements here
                return 0;
            }

            expand();

            // Otherwise i == pos and we don't need to copy anything
            if (i < size()) {
                // Enough space to copy elements after pos i
                for (std::size_t idx = size(); idx > i; --idx) {  // idx > i so idx - 1 is never < 0
                    buf[idx] = std::move(buf[idx - 1]);
                    buf[idx - 1].~T();
                }
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

            for (std::size_t idx = i; idx < size(); ++idx) {  // idx < pos so idx + 1 is never outside of size limit
                buf[idx] = std::move(buf[idx + 1]);
                buf[idx + 1].~T();
            }

            return size();
        }

    public:
        ~Vector() {
            for (std::size_t i; i < size(); ++i) {
                buf[i].~T();
            }
            delete[] buf;
        }

        // Iterator
        Iterator begin() { return Iterator(&buf[0]); }
        Iterator begin() const { return Iterator(&buf[0]); }
        Iterator end() { return Iterator(&buf[size()]); }
        Iterator end() const { return Iterator(&buf[size()]); }

        // Add elements
        void push_back(const T& copy) {
            if (buf == nullptr) {
                init();
            }

            buf[size()] = copy;
            ++buf_pos;
            expand();
        }

        void push_back(T&& move) {
            if (buf == nullptr) {
                init();
            }

            buf[size()] = std::move(move);
            ++buf_pos;
            expand();
        }

        Iterator insert(Iterator pos, const T& copy) {
            std::size_t idx = distance(begin(), pos);
            copy_right(idx);
            buf[idx] = copy;
            return Iterator(&buf[idx]);
        }

        Iterator insert(Iterator pos, T&& move) {
            std::size_t idx = distance(begin(), pos);
            copy_right(idx);
            buf[idx] = std::move(move);
            return Iterator(&buf[idx]);
        }

        // Remove elements
        Iterator erase(Iterator pos) {
            std::size_t idx = distance(begin(), pos);
            copy_left(idx);
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

        // Information
        bool empty() const {
            return !size();
        }

        std::size_t size() const {
            return buf_pos;
        }
    };

    // Erase all elements that match a predicate
    // NOTE: pred is no real predicate as one would need closures for this, but we don't have <functional> available
    //       This means the result has to be passed separately and the function differs from the c++20 std::erase_if
    template<typename T, typename arg>
    std::size_t erase_if(Vector<T>& vec, arg (*pred)(const T&), arg result) {
        std::size_t erased_els = 0;
        for (typename Vector<T>::Iterator it = vec.begin(); it != vec.end(); /*Do nothing*/) {
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
