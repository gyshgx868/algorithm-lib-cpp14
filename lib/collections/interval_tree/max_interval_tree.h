#ifndef ALGORITHM_LIB_CPP14_MAX_INTERVAL_TREE_H
#define ALGORITHM_LIB_CPP14_MAX_INTERVAL_TREE_H

#include <vector>
#include <climits>
#include <algorithm>

#include "interval_tree.h"

template <class T>
struct JoinValue {
    T operator ()(T left, T right) {
        return std::max(left, right);
    }
};

template <class T>
struct JoinDelta {
    T operator ()(T was, T delta) {
        return std::max(was, delta);
    }
};

template <class T>
struct Accumulate {
    T operator ()(T value, T delta, int length) {
        return std::max(value, delta);
    }
};

template <class T>
struct NeutralValue {
    T operator ()() {
        return std::numeric_limits<T>::min();
    }
};

template <class T>
struct NeutralDelta {
    T operator ()() {
        return std::numeric_limits<T>::min();
    }
};

template <class T>
class MaxIntervalTree : public IntervalTree<T, JoinValue<T>, JoinDelta<T>,
        Accumulate<T>, NeutralValue<T>, NeutralDelta<T>> {
private:
    bool array_initialized;
    std::vector<T> array;

public:
    explicit MaxIntervalTree(int size) : IntervalTree<T, JoinValue<T>, JoinDelta<T>,
            Accumulate<T>, NeutralValue<T>, NeutralDelta<T>>(size) {
        array_initialized = false;
        this->Init();
    }

    explicit MaxIntervalTree(const std::vector<T> &array) : IntervalTree<T, JoinValue<T>, JoinDelta<T>,
            Accumulate<T>, NeutralValue<T>, NeutralDelta<T>>(array.size()) {
        array_initialized = true;
        this->array = array;
        this->Init();
    }

protected:
    T InitValue(int index) override {
        if (!array_initialized) {
            return std::numeric_limits<T>::min();
        }
        return array[index];
    }
};

#endif //ALGORITHM_LIB_CPP14_MAX_INTERVAL_TREE_H
