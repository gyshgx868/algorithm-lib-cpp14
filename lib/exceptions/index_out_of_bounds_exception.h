//
// Created by GYSHGX868 on 2017/5/29.
//

#ifndef ALGORITHM_LIB_CPP14_INDEX_OUT_OF_BOUNDS_EXCEPTION_H
#define ALGORITHM_LIB_CPP14_INDEX_OUT_OF_BOUNDS_EXCEPTION_H

#include <string>

class IndexOutOfBoundsException {
public:
    std::string message;

    explicit IndexOutOfBoundsException(int size, int index) {
        message = "Index out of range, size = " + std::to_string(size) + ", index = " + std::to_string(index) + ".";
    }
};

#endif //ALGORITHM_LIB_CPP14_INDEX_OUT_OF_BOUNDS_EXCEPTION_H
