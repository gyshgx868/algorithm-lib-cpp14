#ifndef ALGORITHM_LIB_CPP14_IS_CONTAINER_H
#define ALGORITHM_LIB_CPP14_IS_CONTAINER_H

#include <utility>
#include <iterator>

template <class T>
constexpr auto has_begin(int) -> decltype(std::begin(std::declval<T>()), true) {
    return true;
}

template <class T>
constexpr bool has_begin(...) {
    return false;
}

template <class T>
using is_container = std::integral_constant<bool, has_begin<T>(0)>;

#endif //ALGORITHM_LIB_CPP14_IS_CONTAINER_H
