#ifndef ALGORITHM_LIB_CPP14_SUM_INTERVAL_TREE_H
#define ALGORITHM_LIB_CPP14_SUM_INTERVAL_TREE_H

#include <vector>

#include "interval_tree.h"

template <class T>
class SumIntervalTree : public IntervalTree<T> {
private:
    bool array_initialized;
    std::vector<T> array;

public:
    explicit SumIntervalTree(int size) : IntervalTree<T>(size) {
        array_initialized = false;
    }

    explicit SumIntervalTree(std::vector<T> array) : IntervalTree<T>(array.size(), false) {
        array_initialized = true;
        this->array = array;
        this->init();
    }

protected:
    T init_value(int index) override {
        if (!array_initialized) {
            return 0;
        }
        return array[index];
    }

    T join_value(T left, T right) override {
        return left + right;
    }

    T join_delta(T was, T delta) override {
        return was + delta;
    }

    T accumulate(T value, T delta, int length) override {
        return value + delta * length;
    }

    T neutral_value() override {
        return 0;
    }

    T neutral_delta() override {
        return 0;
    }
};

#endif //ALGORITHM_LIB_CPP14_SUM_INTERVAL_TREE_H
