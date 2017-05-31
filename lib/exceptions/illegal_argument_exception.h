#ifndef ALGORITHM_LIB_CPP14_ILLEGAL_ARGUMENT_EXCEPTION_H
#define ALGORITHM_LIB_CPP14_ILLEGAL_ARGUMENT_EXCEPTION_H

#include <string>

class IllegalArgumentException {
public:
    std::string message;

    IllegalArgumentException() {
        message = "Illegal argument.";
    }

    explicit IllegalArgumentException(std::string message) {
        this->message = message;
    }
};

#endif //ALGORITHM_LIB_CPP14_ILLEGAL_ARGUMENT_EXCEPTION_H
