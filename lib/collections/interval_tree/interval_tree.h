#ifndef ALGORITHM_LIB_CPP14_INTERVAL_TREE_H
#define ALGORITHM_LIB_CPP14_INTERVAL_TREE_H

#include <algorithm>

#include "../../misc/integer_utils.h"

template <class T>
class IntervalTree {
protected:
    int _size;
    std::vector<T> _value;
    std::vector<T> _delta;

    virtual T join_value(T left, T right)= 0;

    virtual T join_delta(T was, T delta)= 0;

    virtual T accumulate(T value, T delta, int length)= 0;

    virtual T neutral_value()= 0;

    virtual T neutral_delta()= 0;

    virtual void init_data(int size, int nodeCount) {
        _value.resize(static_cast<unsigned int>(nodeCount));
        _delta.resize(static_cast<unsigned int>(nodeCount));
    }

    virtual void init_after(int root, int left, int right, int middle) {
        _value[root] = join_value(_value[2 * root + 1], _value[2 * root + 2]);
        _delta[root] = neutral_delta();
    }

    virtual void init_before(int root, int left, int right, int middle) {
        // TODO
        return;
    }

    virtual T init_value(int index) {
        return neutral_value();
    }

    virtual void init_leaf(int root, int index) {
        _value[root] = init_value(index);
        _delta[root] = neutral_delta();
    }

    virtual void update_post_process(int root, int left, int right, int from, int to, T delta, int middle) {
        _value[root] = join_value(_value[2 * root + 1], _value[2 * root + 2]);
    }

    void push_down(int root, int left, int middle, int right) {
        _value[2 * root + 1] = accumulate(_value[2 * root + 1], _delta[root], middle - left + 1);
        _value[2 * root + 2] = accumulate(_value[2 * root + 2], _delta[root], right - middle);
        _delta[2 * root + 1] = join_delta(_delta[2 * root + 1], _delta[root]);
        _delta[2 * root + 2] = join_delta(_delta[2 * root + 2], _delta[root]);
        _delta[root] = neutral_delta();
    }

    virtual void update_pre_process(int root, int left, int right, int from, int to, T delta, int middle) {
        push_down(root, left, middle, right);
    }

    virtual void update_full(int root, int left, int right, int from, int to, T delta) {
        _value[root] = accumulate(_value[root], delta, right - left + 1);
        this->_delta[root] = join_delta(this->_delta[root], delta);
    }

    virtual T query_post_process(int root, int left, int right, int from, int to, int middle,
                                 T left_result, T right_result) {
        return join_value(left_result, right_result);
    }

    virtual void query_pre_process(int root, int left, int right, int from, int to, int middle) {
        push_down(root, left, middle, right);
    }

    virtual T query_full(int root, int left, int right, int from, int to) {
        return _value[root];
    }

    virtual T empty_segment_result() {
        return neutral_value();
    }

    void update(int root, int left, int right, int from, int to, T delta) {
        if (left > to || right < from) {
            return;
        }
        if (left >= from && right <= to) {
            update_full(root, left, right, from, to, delta);
            return;
        }
        int middle = (left + right) >> 1;
        update_pre_process(root, left, right, from, to, delta, middle);
        update(2 * root + 1, left, middle, from, to, delta);
        update(2 * root + 2, middle + 1, right, from, to, delta);
        update_post_process(root, left, right, from, to, delta, middle);
    }

    T query(int root, int left, int right, int from, int to) {
        if (left > to || right < from) {
            return empty_segment_result();
        }
        if (left >= from && right <= to) {
            return query_full(root, left, right, from, to);
        }
        int middle = (left + right) >> 1;
        query_pre_process(root, left, right, from, to, middle);
        T left_result = query(2 * root + 1, left, middle, from, to);
        T right_result = query(2 * root + 2, middle + 1, right, from, to);
        return query_post_process(root, left, right, from, to, middle, left_result, right_result);
    }

private:
    void init(int root, int left, int right) {
        if (left == right) {
            init_leaf(root, left);
        } else {
            int middle = (left + right) >> 1;
            init_before(root, left, right, middle);
            init(2 * root + 1, left, middle);
            init(2 * root + 2, middle + 1, right);
            init_after(root, left, right, middle);
        }
    }

public:
    explicit IntervalTree(int size, bool shouldInit = true) {
        this->_size = size;
        int nodeCount = std::max(1, IntegerUtils::highest_one_bit(size) << 2);
        IntervalTree<T>::init_data(size, nodeCount);
        if (shouldInit) {
            init();
        }
    }

    virtual ~IntervalTree() = default;

    void init() {
        if (_size == 0) {
            return;
        }
        init(0, 0, _size - 1);
    }

    void update(int from, int to, T delta) {
        update(0, 0, _size - 1, from, to, delta);
    }

    T query(int from, int to) {
        return query(0, 0, _size - 1, from, to);
    }
};

#endif //ALGORITHM_LIB_CPP14_INTERVAL_TREE_H
