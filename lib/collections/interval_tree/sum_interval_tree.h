#ifndef ALGORITHM_LIB_CPP14_SUM_INTERVAL_TREE_H
#define ALGORITHM_LIB_CPP14_SUM_INTERVAL_TREE_H

#include <vector>

#include <lib/numbers/zero_element.h>

#include "interval_tree.h"

template <class T>
struct SumJoinValue {
    T operator ()(T left, T right) {
        return left + right;
    }
};

template <class T>
struct SumJoinDelta {
    T operator ()(T was, T delta) {
        return was + delta;
    }
};

template <class T>
struct SumAccumulate {
    T operator ()(T value, T delta, int length) {
        return value + delta * length;
    }
};

template <class T>
struct SumNeutralValue {
    T operator ()() {
        return Zero<T>::value();
    }
};

template <class T>
struct SumNeutralDelta {
    T operator ()() {
        return Zero<T>::value();
    }
};

template <class T>
class SumIntervalTree : public IntervalTree<T, SumJoinValue<T>, SumJoinDelta<T>,
        SumAccumulate<T>, SumNeutralValue<T>, SumNeutralDelta<T>> {
private:
    bool array_initialized;
    std::vector<T> array;

public:
    explicit SumIntervalTree(int size) : IntervalTree<T, SumJoinValue<T>, SumJoinDelta<T>,
            SumAccumulate<T>, SumNeutralValue<T>, SumNeutralDelta<T>>(size) {
        array_initialized = false;
        this->Init();
    }

    explicit SumIntervalTree(const std::vector<T> &array) : IntervalTree<T, SumJoinValue<T>, SumJoinDelta<T>,
            SumAccumulate<T>, SumNeutralValue<T>, SumNeutralDelta<T>>(array.size()) {
        array_initialized = true;
        this->array = array;
        this->Init();
    }

protected:
    T InitValue(int index) override {
        if (!array_initialized) {
            return Zero<T>::value();
        }
        return array[index];
    }
};

#endif //ALGORITHM_LIB_CPP14_SUM_INTERVAL_TREE_H
