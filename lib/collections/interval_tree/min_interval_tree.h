#ifndef ALGORITHM_LIB_CPP14_MIN_INTERVAL_TREE_H
#define ALGORITHM_LIB_CPP14_MIN_INTERVAL_TREE_H

#include <vector>
#include <climits>
#include <algorithm>

#include "interval_tree.h"

template <class T>
struct MinJoinValue {
    T operator ()(T left, T right) {
        return std::min(left, right);
    }
};

template <class T>
struct MinJoinDelta {
    T operator ()(T was, T delta) {
        return std::min(was, delta);
    }
};

template <class T>
struct MinAccumulate {
    T operator ()(T value, T delta, int length) {
        return std::min(value, delta);
    }
};

template <class T>
struct MinNeutralValue {
    T operator ()() {
        return std::numeric_limits<T>::max();
    }
};

template <class T>
struct MinNeutralDelta {
    T operator ()() {
        return std::numeric_limits<T>::max();
    }
};

template <class T>
class MinIntervalTree : public IntervalTree<T, MinJoinValue<T>, MinJoinDelta<T>,
        MinAccumulate<T>, MinNeutralValue<T>, MinNeutralDelta<T>> {
private:
    bool array_initialized;
    std::vector<T> array;

public:
    explicit MinIntervalTree(int size) : IntervalTree<T, MinJoinValue<T>, MinJoinDelta<T>,
            MinAccumulate<T>, MinNeutralValue<T>, MinNeutralDelta<T>>(size) {
        array_initialized = false;
        this->Init();
    }

    explicit MinIntervalTree(const std::vector<T> &array) : IntervalTree<T, MinJoinValue<T>, MinJoinDelta<T>,
            MinAccumulate<T>, MinNeutralValue<T>, MinNeutralDelta<T>>(array.size()) {
        array_initialized = true;
        this->array = array;
        this->Init();
    }

protected:
    T InitValue(int index) override {
        if (!array_initialized) {
            return std::numeric_limits<T>::max();
        }
        return array[index];
    }
};

#endif //ALGORITHM_LIB_CPP14_MIN_INTERVAL_TREE_H
