#ifndef ALGORITHM_LIB_CPP14_NO_SUCH_ELEMENT_EXCEPTION_H
#define ALGORITHM_LIB_CPP14_NO_SUCH_ELEMENT_EXCEPTION_H

#include <string>

class NoSuchElementException {
public:
    std::string message;

    NoSuchElementException() {
        message = "No such element.";
    }
};

#endif //ALGORITHM_LIB_CPP14_NO_SUCH_ELEMENT_EXCEPTION_H
