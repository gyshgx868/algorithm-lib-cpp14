#ifndef ALGORITHM_LIB_CPP14_ENABLE_IF_T_H
#define ALGORITHM_LIB_CPP14_ENABLE_IF_T_H

#include <type_traits>

template <bool Bool, class T = void>
using enable_if_t = class std::enable_if<Bool, T>::type;

#endif //ALGORITHM_LIB_CPP14_ENABLE_IF_T_H
