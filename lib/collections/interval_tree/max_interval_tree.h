#ifndef ALGORITHM_LIB_CPP14_MAX_INTERVAL_TREE_H
#define ALGORITHM_LIB_CPP14_MAX_INTERVAL_TREE_H

#include <vector>
#include <climits>
#include <algorithm>

#include "interval_tree.h"

template <class T>
class MaxIntervalTree : public IntervalTree<T> {
private:
    bool array_initialized;
    std::vector<T> array;

public:
    explicit MaxIntervalTree(int size) : IntervalTree<T>(size) {
        array_initialized = false;
    }

    explicit MaxIntervalTree(std::vector<T> array) : IntervalTree<T>(array.size(), false) {
        array_initialized = true;
        this->array = array;
        this->init();
    }

protected:
    T init_value(int index) override {
        if (!array_initialized) {
            return std::numeric_limits<T>::min();
        }
        return array[index];
    }

    T join_value(T left, T right) override {
        return std::max(left, right);
    }

    T join_delta(T was, T delta) override {
        return std::max(was, delta);
    }

    T accumulate(T value, T delta, int length) override {
        return std::max(value, delta);
    }

    T neutralValue() {
        return std::numeric_limits<T>::min();
    }

    T neutralDelta() {
        return std::numeric_limits<T>::min();
    }
};

#endif //ALGORITHM_LIB_CPP14_MAX_INTERVAL_TREE_H
