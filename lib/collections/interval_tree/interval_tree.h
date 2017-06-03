#ifndef ALGORITHM_LIB_CPP14_INTERVAL_TREE_H
#define ALGORITHM_LIB_CPP14_INTERVAL_TREE_H

#include <algorithm>

#include "interval_tree_base.h"

template <class T, class JoinValue, class JoinDelta, class Accumulate,
        class NeutralValue, class NeutralDelta>
class IntervalTree : public IntervalTreeBase<T> {
protected:
    std::vector<T> value;
    std::vector<T> delta;
    JoinValue join_value_;
    JoinDelta join_delta_;
    Accumulate accumulate_;
    NeutralValue neutral_value_;
    NeutralDelta neutral_delta_;

public:
    explicit IntervalTree(int size,
                          const JoinValue &join_value = JoinValue(),
                          const JoinDelta &join_delta = JoinDelta(),
                          const Accumulate &accumulate = Accumulate(),
                          const NeutralValue &neutral_value = NeutralValue(),
                          const NeutralDelta &neutral_delta = NeutralDelta()) : IntervalTreeBase<T>(size) {
        join_value_ = join_value;
        join_delta_ = join_delta;
        accumulate_ = accumulate;
        neutral_value_ = neutral_value;
        neutral_delta_ = neutral_delta;
        InitData(size);
    }

protected:
    void InitData(int size) {
        int node_count = IntegerUtils::highest_one_bit(size) << 2;
        value.resize(node_count);
        delta.resize(node_count);
    }

    // virtual T JoinValue(T left, T right)= 0;

    // virtual T JoinDelta(T was, T delta)= 0;

    // virtual T Accumulate(T value, T delta, int length)= 0;

    // virtual T NeutralValue()= 0;

    // virtual T NeutralDelta()= 0;

    virtual T InitValue(int index) {
        return neutral_value_();
    }

    void InitAfter(int root, int left, int right, int middle) override {
        value[root] = join_value_(value[2 * root + 1], value[2 * root + 2]);
        delta[root] = neutral_delta_();
    }

    void InitBefore(int root, int left, int right, int middle) override {
    }

    void InitLeaf(int root, int index) override {
        value[root] = InitValue(index);
        delta[root] = neutral_delta_();
    }

    void UpdatePostProcess(int root, int left, int right, int from, int to, T delta, int middle) override {
        value[root] = join_value_(value[2 * root + 1], value[2 * root + 2]);
    }

    void UpdatePreProcess(int root, int left, int right, int from, int to, T delta, int middle) override {
        PushDown(root, left, middle, right);
    }

    void PushDown(int root, int left, int middle, int right) {
        value[2 * root + 1] = accumulate_(value[2 * root + 1], delta[root], middle - left + 1);
        value[2 * root + 2] = accumulate_(value[2 * root + 2], delta[root], right - middle);
        delta[2 * root + 1] = join_delta_(delta[2 * root + 1], delta[root]);
        delta[2 * root + 2] = join_delta_(delta[2 * root + 2], delta[root]);
        delta[root] = neutral_delta_();
    }

    void UpdateFull(int root, int left, int right, int from, int to, T delta) override {
        value[root] = accumulate_(value[root], delta, right - left + 1);
        this->delta[root] = join_delta_(this->delta[root], delta);
    }

    T QueryPostProcess(int root, int left, int right, int from, int to, int middle, T left_result,
                       T right_result) override {
        return join_value_(left_result, right_result);
    }

    void QueryPreProcess(int root, int left, int right, int from, int to, int middle) override {
        PushDown(root, left, middle, right);
    }

    T QueryFull(int root, int left, int right, int from, int to) override {
        return value[root];
    }

    T EmptySegmentResult() override {
        return neutral_value_();
    }
};

#endif //ALGORITHM_LIB_CPP14_INTERVAL_TREE_H
