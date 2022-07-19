#ifndef __UniquePointer_Include_H_
#define __UniquePointer_Include_H_

#include <type_traits>
#include <utility>

// https://en.cppreference.com/w/cpp/memory/unique_ptr

namespace bse {

    template<typename T>
    class unique_ptr {
    private:
        T* ptr = nullptr;

        // Forbid copying
        unique_ptr(const T& copy) = delete;
        unique_ptr& operator=(const unique_ptr<T>& copy) = delete;

    public:
        // Construction
        unique_ptr() = default;
        unique_ptr(T* ptr) : ptr(std::move(ptr)) {}

        // Deletion
        ~unique_ptr<T>() {
            delete ptr;
        }

        // Moving
        unique_ptr(unique_ptr&& move) noexcept { reset(move.release()); }
        unique_ptr& operator=(unique_ptr&& move) noexcept {
            reset(move.release());
            return *this;
        }

        // Resetting: Replaces managed object, deleting the old one
        void reset(T* ptr) {
            delete this->ptr;
            this->ptr = ptr;
        }
        void reset() { reset(nullptr); }

        // Release: Releases ownership without deletion
        T* release() {
            // T* old = ptr;
            // ptr = nullptr;
            // return old;
            return std::exchange(ptr, nullptr);
        }

        // Nice to have operators
        T* operator->() { return ptr; }
        T& operator*() const { return *ptr; }
        explicit operator void*() const { return ptr; }
        explicit operator bool() const { return (ptr != nullptr); }
    };

    // Unique Pointer for Arrays
    template<typename T>
    class unique_ptr<T[]> {
    private:
        T* ptr = nullptr;

        // unique_ptr(const T& copy) = delete;
        // unique_ptr& operator=(const unique_ptr<T>& copy) = delete;

    public:
        unique_ptr() = default;
        unique_ptr(T* ptr) : ptr(std::move(ptr)) {}

        // Deletion
        ~unique_ptr<T[]>() {
            delete[] ptr;
        }

        // Moving
        unique_ptr(unique_ptr&& move) noexcept { reset(move.release()); }
        unique_ptr& operator=(unique_ptr&& move) noexcept {
            reset(move.release());
            return *this;
        }

        // Resetting: Replaces managed object, deleting the old one
        void reset(T* ptr) {
            delete this->ptr;
            this->ptr = ptr;
        }
        void reset() { reset(nullptr); }

        // Release: Releases ownership without deletion
        T* release() {
            // T* old = ptr;
            // ptr = nullptr;
            // return old;
            return std::exchange(ptr, nullptr);
        }

        // Nice to have operators
        T* operator->() = delete;
        T& operator*() const = delete;
        explicit operator void*() const { return ptr; }
        explicit operator bool() const { return (ptr != nullptr); }
        T& operator[](std::size_t i) { return ptr[i]; }
    };

    // Allow initialization of unique_ptr<int> with optional constructor arguments
    // and unique_ptr<int[]> without constructor arguments
    template<typename T, typename... Args>

    // We make the return type dependent on whether T is an array type or not
    typename std::enable_if<!std::is_array<T>::value, unique_ptr<T>>::type
    make_unique(Args&&... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T>
    typename std::enable_if<std::is_array<T>::value, unique_ptr<T>>::type
    make_unique(std::size_t size) {
        using bare_type = typename std::remove_extent<T>::type;
        return unique_ptr<T>(new bare_type[size]);
    }

}  // namespace bse

#endif
