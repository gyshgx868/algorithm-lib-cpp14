#ifndef ALGORITHM_LIB_CPP14_INTERVAL_TREE_BASE_H
#define ALGORITHM_LIB_CPP14_INTERVAL_TREE_BASE_H

#include <algorithm>

#include <lib/misc/integer_utils.h>

template <class T>
class IntervalTreeBase {
protected:
    int size_;

    virtual void InitLeaf(int root, int index)= 0;

    virtual void InitBefore(int root, int left, int right, int middle)= 0;

    virtual void InitAfter(int root, int left, int right, int middle)= 0;

    virtual void UpdateFull(int root, int left, int right, int from, int to, T delta)= 0;

    virtual void UpdatePreProcess(int root, int left, int right, int from, int to, T delta, int middle)= 0;

    virtual void UpdatePostProcess(int root, int left, int right, int from, int to, T delta, int middle)= 0;

    virtual T EmptySegmentResult()= 0;

    virtual T QueryFull(int root, int left, int right, int from, int to)= 0;

    virtual void QueryPreProcess(int root, int left, int right, int from, int to, int middle)= 0;

    virtual T
    QueryPostProcess(int root, int left, int right, int from, int to, int middle, T left_result, T right_result)= 0;

    void Update(int root, int left, int right, int from, int to, T delta) {
        if (left > to || right < from) {
            return;
        }
        if (left >= from && right <= to) {
            UpdateFull(root, left, right, from, to, delta);
            return;
        }
        int middle = (left + right) >> 1;
        UpdatePreProcess(root, left, right, from, to, delta, middle);
        Update(2 * root + 1, left, middle, from, to, delta);
        Update(2 * root + 2, middle + 1, right, from, to, delta);
        UpdatePostProcess(root, left, right, from, to, delta, middle);
    }

    T Query(int root, int left, int right, int from, int to) {
        if (left > to || right < from) {
            return EmptySegmentResult();
        }
        if (left >= from && right <= to) {
            return QueryFull(root, left, right, from, to);
        }
        int middle = (left + right) >> 1;
        QueryPreProcess(root, left, right, from, to, middle);
        T left_result = Query(2 * root + 1, left, middle, from, to);
        T right_result = Query(2 * root + 2, middle + 1, right, from, to);
        return QueryPostProcess(root, left, right, from, to, middle, left_result, right_result);
    }

private:
    void Init(int root, int left, int right) {
        if (left == right) {
            InitLeaf(root, left);
        } else {
            int middle = (left + right) >> 1;
            InitBefore(root, left, right, middle);
            Init(2 * root + 1, left, middle);
            Init(2 * root + 2, middle + 1, right);
            InitAfter(root, left, right, middle);
        }
    }

public:
    explicit IntervalTreeBase(int size) {
        this->size_ = size;
    }

    void Init() {
        if (size_ == 0) {
            return;
        }
        Init(0, 0, size_ - 1);
    }

    void Update(int from, int to, T delta) {
        Update(0, 0, size_ - 1, from, to, delta);
    }

    T Query(int from, int to) {
        return Query(0, 0, size_ - 1, from, to);
    }
};

#endif //ALGORITHM_LIB_CPP14_INTERVAL_TREE_BASE_H
