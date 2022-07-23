#ifndef __MATH_INCLUDE_H_
#define __MATH_INCLUDE_H_

namespace bse {
    template<typename T>
    T& min(const T& a, const T& b) { return a < b ? a : b; }

    template<typename T>
    T& max(const T& a, const T& b) { return a > b ? a : b; }
}  // namespace bse

#endif
