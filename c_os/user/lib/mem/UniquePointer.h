#ifndef __UniquePointer_Include_H_
#define __UniquePointer_Include_H_

#include <utility>

// https://en.cppreference.com/w/cpp/memory/unique_ptr

// NOTE: Because of the way the scheduling works our functions are not executed completely.
//       This means that object destructors are not called if the objects live in a scope
//       that is left because of thread switching...

namespace bse {

    // T is the type make_unique is called with, meaning int or int[] for example
    // T_ is the bare type without extents (int in both cases), so we have a
    // int* pointer type for both unique_ptr<int> and unique_ptr<int[]>
    template<typename T>
    class unique_ptr {
    private:
        using T_ = std::remove_extent_t<T>;

        T_* ptr = nullptr;

        // Only use make_unique or reset for construction
        unique_ptr(T_* ptr) : ptr(std::move(ptr)) {}

        // I didn't want to introduce custom deleters for my small needs
        void del() {
            if constexpr (std::is_array_v<T>) {
                delete[] ptr;
            } else {
                delete ptr;
            }
            ptr = nullptr;
        }

        // Forbid copying
        unique_ptr(const unique_ptr& copy) = delete;
        unique_ptr& operator=(const unique_ptr& copy) = delete;

    public:
        // Construction
        unique_ptr() = default;  // Allow declaration without explicit definition

        template<typename t, typename... Args>
        friend typename std::enable_if_t<!std::is_array_v<t>, unique_ptr<t>>
        make_unique(Args&&... args);

        template<typename Tt>
        friend typename std::enable_if_t<std::is_array_v<Tt>, unique_ptr<Tt>>
        make_unique(std::size_t size);

        // Deletion
        ~unique_ptr() {
            del();
        }

        // Moving
        unique_ptr(unique_ptr&& move) noexcept { reset(move.release()); }
        unique_ptr& operator=(unique_ptr&& move) noexcept {
            reset(move.release());
            return *this;
        }

        // Resetting: Replaces managed object, deleting the old one
        void reset() { del(); }
        void reset(T_* ptr) {
            del();
            this->ptr = ptr;
        }

        // Release: Releases ownership without deletion
        T_* release() {
            // T* old = ptr;
            // ptr = nullptr;
            // return old;
            return std::exchange(ptr, nullptr);
        }

        // Nice to have operators
        T_* operator->() { return ptr; }
        const T_& operator*() const { return *ptr; }
        explicit operator void*() const { return ptr; }
        explicit operator bool() const { return (ptr != nullptr); }
        bool operator==(const unique_ptr& other) const { return ptr == other.ptr; }

        // These are only for array unique_ptr but I didn't want to define a full template specialization just for this
        T_& operator[](std::size_t i) { return ptr[i]; }
        const T_& operator[](std::size_t i) const { return ptr[i]; }
    };

    // make_unique implementation =======================================

    // Allow initialization of unique_ptr<int> with optional constructor arguments
    // and unique_ptr<int[]> without constructor arguments
    template<typename T, typename... Args>

    // We make the return type dependent on whether T is an array type or not
    typename std::enable_if_t<!std::is_array_v<T>, unique_ptr<T>>
    make_unique(Args&&... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<typename T>
    typename std::enable_if_t<std::is_array_v<T>, unique_ptr<T>>
    make_unique(std::size_t size) {
        using T_ = typename std::remove_extent_t<T>;
        return unique_ptr<T>(new T_[size]);
    }

}  // namespace bse

#endif
