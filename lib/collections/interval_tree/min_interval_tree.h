#ifndef ALGORITHM_LIB_CPP14_MIN_INTERVAL_TREE_H
#define ALGORITHM_LIB_CPP14_MIN_INTERVAL_TREE_H

#include <vector>
#include <climits>
#include <algorithm>

#include "interval_tree.h"

template <class T>
class MinIntervalTree : public IntervalTree<T> {
private:
    bool array_initialized;
    std::vector<T> array;

public:
    explicit MinIntervalTree(int size) : IntervalTree<T>(size) {
        array_initialized = false;
    }

    explicit MinIntervalTree(std::vector<T> array) : IntervalTree<T>(array.size(), false) {
        array_initialized = true;
        this->array = array;
        this->init();
    }

protected:
    T init_value(int index) override {
        if (!array_initialized) {
            return std::numeric_limits<T>::max();
        }
        return array[index];
    }

    T join_value(T left, T right) override {
        return std::min(left, right);
    }

    T join_delta(T was, T delta) override {
        return std::min(was, delta);
    }

    T accumulate(T value, T delta, int length) override {
        return std::min(value, delta);
    }

    T neutral_value() override {
        return std::numeric_limits<T>::max();
    }

    T neutral_delta() override {
        return std::numeric_limits<T>::max();
    }
};

#endif //ALGORITHM_LIB_CPP14_MIN_INTERVAL_TREE_H
