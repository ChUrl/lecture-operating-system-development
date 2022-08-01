#ifndef String_Include_H_
#define String_Include_H_

#include "user/lib/Array.h"
#include "user/lib/Iterator.h"

// A heap dynamically heap-allocated string (mutable)

namespace bse {

    unsigned int strlen(const char* str);
    void strncpy(char* destination, unsigned int n, const char* source);
    int strcmp(const char* a, const char* b);

    class string {
    private:
        std::size_t len = 0;
        char* buf = nullptr;

    public:
        using iterator = ContinuousIterator<char>;

        string() = default;

        string(const char* str) : len(strlen(str)), buf(new char[len + 1]) {
            strncpy(buf, len + 1, str);
        }

        // Convert char array to string
        template<std::size_t N>
        explicit string(const array<char, N>& arr) : len(N), buf(new char[len + 1]) {
            for (std::size_t i = 0; i < N; ++i) {
                buf[i] = arr[i];
            }
            buf[N] = '\0';
        }

        string(const string& copy) : len(copy.len), buf(new char[len + 1]) {
            strncpy(buf, len + 1, copy.buf);
        }

        string& operator=(const string& copy) {
            if (&copy != this) {
                len = copy.len;
                buf = new char[len + 1];
                strncpy(buf, len + 1, copy.buf);
            }

            return *this;
        }

        string(string&& move) noexcept : len(move.len), buf(move.buf) {
            move.len = 0;
            move.buf = nullptr;
        }

        string& operator=(string&& move) noexcept {
            if (&move != this) {
                len = move.len;
                buf = move.buf;

                move.len = 0;
                move.buf = nullptr;
            }

            return *this;
        }

        ~string() {
            delete[] buf;
        }

        iterator begin() { return iterator(buf); }
        iterator begin() const { return iterator(buf); }
        iterator end() { return iterator(&buf[len]); }
        iterator end() const { return iterator(&buf[len]); }

        explicit operator char*() { return buf; }
        explicit operator char*() const { return buf; }

        char operator[](std::size_t pos) { return buf[pos]; }
        char operator[](std::size_t pos) const { return buf[pos]; }

        string operator+(const string& other) const {
            string new_str;
            new_str.len = len + other.len;
            new_str.buf = new char[new_str.len + 1];

            strncpy(new_str.buf, len, buf);                        // Copy this content
            strncpy(&new_str.buf[len], other.len + 1, other.buf);  // Copy other content

            return new_str;
        }

        string operator+(const char* other) const {
            std::size_t other_len = strlen(other);

            string new_str;
            new_str.len = len + other_len;
            new_str.buf = new char[new_str.len + 1];

            strncpy(new_str.buf, len, buf);
            strncpy(&new_str.buf[len], other_len + 1, other);

            return new_str;
        }

        string& operator+=(const string& other) {
            unsigned int new_len = len + other.size();
            char* new_buf = new char[new_len + 1];

            strncpy(new_buf, len, buf);
            strncpy(&new_buf[len], other.size() + 1, other.buf);

            delete[] buf;
            buf = new_buf;
            len = new_len;

            return *this;
        }

        string& operator+=(const char* other) {
            unsigned int other_len = strlen(other);
            unsigned int new_len = len + other_len;
            char* new_buf = new char[new_len + 1];

            strncpy(new_buf, len, buf);
            strncpy(&new_buf[len], other_len + 1, other);

            delete[] buf;
            buf = new_buf;
            len = new_len;

            return *this;
        }

        string operator*(unsigned int n) const {
            string new_str;
            new_str.len = len * n;
            new_str.buf = new char[new_str.len];

            for (unsigned int i = 0; i < n; ++i) {
                strncpy(&new_str.buf[i * len], len, buf);
            }

            return new_str;
        }

        string& operator*=(unsigned int n) {
            unsigned int new_len = len * n;
            char* new_buf = new char[new_len];

            for (unsigned int i = 0; i < n; ++i) {
                strncpy(&new_buf[i * len], len, buf);
            }

            delete[] buf;
            buf = new_buf;
            len = new_len;
            return *this;
        }

        bool operator==(const string& other) const {
            return strcmp(buf, other.buf) == 0;
        }

        bool operator!=(const string& other) const {
            return strcmp(buf, other.buf) != 0;
        }

        std::size_t size() const {
            return len;
        }
    };

}  // namespace bse

#endif
