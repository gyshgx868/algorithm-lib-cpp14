#ifndef ALGORITHM_LIB_CPP14_EMPTY_QUEUE_EXCEPTION_H
#define ALGORITHM_LIB_CPP14_EMPTY_QUEUE_EXCEPTION_H

#include <string>

class EmptyQueueException {
public:
    std::string message;

    EmptyQueueException() {
        message = "Queue is empty.";
    }
};

#endif //ALGORITHM_LIB_CPP14_EMPTY_QUEUE_EXCEPTION_H
