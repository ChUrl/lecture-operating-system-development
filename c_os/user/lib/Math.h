#ifndef MATH_INCLUDE_H_
#define MATH_INCLUDE_H_

namespace bse {

    // I didn't add any numeric constraints, will do with c++20

    template<typename T>
    T& min(const T& a, const T& b) { return a < b ? a : b; }

    template<typename T>
    T& max(const T& a, const T& b) { return a > b ? a : b; }

    template<typename T>
    std::make_unsigned<T> abs(const T& a) { return a < 0 ? -a : a; }

}  // namespace bse

#endif
