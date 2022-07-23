#ifndef __String_Include_H_
#define __String_Include_H_

#include "user/lib/Array.h"
#include "user/lib/Iterator.h"

namespace bse {

    unsigned int strlen(const char* str);
    void strncpy(char* destination, unsigned int n, const char* source);
    int strcmp(const char* a, const char* b);

    class string {
    private:
        unsigned int len = 0;
        char* buf = nullptr;

    public:
        using iterator = ContinuousIterator<char>;

        string() = default;

        string(const char* str) : len(strlen(str)), buf(new char[len + 1]) {
            strncpy(buf, len + 1, str);
        }

        template<unsigned int N>
        string(const array<char, N>& arr) : len(N), buf(new char[len + 1]) {
            for (unsigned int i = 0; i < N; ++i) {
                buf[i] = arr[i];
            }
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
            delete[] move.buf;
            move.len = 0;
            move.buf = nullptr;
        }

        string& operator=(string&& move) noexcept {
            if (&move != this) {
                len = move.len;
                buf = move.buf;

                delete[] move.buf;
                move.len = 0;
                move.buf = nullptr;
            }

            return *this;
        }

        ~string() {
            delete[] buf;
        }

        iterator begin() { return iterator(&buf[0]); }
        iterator begin() const { return iterator(&buf[0]); }
        iterator end() { return iterator(&buf[len]); }
        iterator end() const { return iterator(&buf[len]); }

        explicit operator char*() { return buf; }
        explicit operator char*() const { return buf; }

        char operator[](std::size_t pos) { return buf[pos]; }
        char operator[](std::size_t pos) const { return buf[pos]; }

        string operator+(const string& other) {
            string new_str;
            new_str.len = len + other.len;
            new_str.buf = new char[new_str.len + 1];

            strncpy(new_str.buf, len, buf);                        // Copy this content
            strncpy(&new_str.buf[len], other.len + 1, other.buf);  // Copy other content

            return new_str;
        }

        bool operator==(const string& other) {
            return strcmp(buf, other.buf) == 0;
        }

        bool operator!=(const string& other) {
            return strcmp(buf, other.buf) != 0;
        }

        unsigned int size() const {
            return len;
        }
    };

}  // namespace bse

#endif
