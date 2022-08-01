#ifndef C_OS_STRINGVIEW_H
#define C_OS_STRINGVIEW_H

#include "user/lib/String.h"
#include "user/lib/Iterator.h"
#include <utility>

namespace bse {

    class string_view {
    private:
        std::size_t len = 0;
        const char* buf = nullptr;

    public:
        using iterator = ContinuousIterator<char>;

        string_view() = default;

        // Important that char* and string& can be implicitly converted: Only have to provide one
        // implementation using stringview for everything (OutStream only uses string_view for example)
        string_view(const char* str) : len(strlen(str)), buf(str) {}
        string_view(const string& str) : len(str.size()), buf(static_cast<char*>(str)) {}

        iterator begin() const { return iterator(buf); }
        iterator end() const { return iterator(&buf[len]); }

        explicit operator const char*() const { return buf; }
        char operator[](std::size_t pos) const { return buf[pos]; }
        bool operator==(const string_view& other) const { return buf == other.buf; }
        bool operator!=(const string_view& other) const { return buf != other.buf; }

        std::size_t size() const { return len; }

        string_view substring(std::size_t first, std::size_t last) const;
    };

}  // namespace bse

#endif  //C_OS_STRINGVIEW_H
