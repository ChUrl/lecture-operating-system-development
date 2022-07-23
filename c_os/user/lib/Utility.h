#ifndef __Utility_Include_H_
#define __Utility_Include_H_

#include <type_traits>

namespace bse {

    // All std::move does is cast to an rvalue reference

    template<typename T>
    typename std::remove_reference_t<T>&&
    move(T&& t) {
        return static_cast<typename std::remove_reference_t<T>&&>(t);
    }
}  // namespace bse

#endif
