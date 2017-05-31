#ifndef ALGORITHM_LIB_CPP14_INT_HEAP_H
#define ALGORITHM_LIB_CPP14_INT_HEAP_H

#include <vector>
#include <algorithm>

#include "../../exceptions/index_out_of_bounds_exception.h"
#include "../../exceptions/empty_queue_exception.h"

template <class Comparator = std::less<int>>
class Heap {
private:
    Comparator comparator;
    std::vector<int> data;
    std::vector<int> at;
    int _size;

    void sift_up(int index) {
        if (index < 0 || index >= _size) {
            throw IndexOutOfBoundsException(_size, index);
        }
        int value = data[index];
        while (index != 0) {
            int parent = (index - 1) >> 1;
            int parentValue = data[parent];
            if (comparator.compare(parentValue, value)) {
                data[index] = value;
                at[value] = index;
                return;
            }
            data[index] = parentValue;
            at[parentValue] = index;
            index = parent;
        }
        data[0] = value;
        at[value] = 0;
    }

    void sift_down(int index) {
        if (index < 0 || index >= _size) {
            throw IndexOutOfBoundsException(_size, index);
        }
        while (true) {
            int child = (index << 1) + 1;
            if (child >= _size) {
                return;
            }
            if (child + 1 < _size && !comparator(data[child], data[child + 1])) {
                child++;
            }
            if (comparator(data[index], data[child])) {
                return;
            }
            std::swap(data[index], data[child]);
            index = child;
        }
    }

public:
    explicit Heap(int max_element) {
        _size = 0;
        data.resize(10);
        at.assign((unsigned int) (max_element + 1), -1);
    }

    explicit Heap(int capacity, int max_element) {
        _size = 0;
        data.resize((unsigned int) capacity);
        at.assign((unsigned int) (max_element + 1), -1);
    }

    int size() const {
        return _size;
    }

    bool empty() const {
        return _size == 0;
    }

    void push(int element) {
        if (data.size() < _size + 1) {
            data.resize(data.size() << 1);
        }
        data[_size] = element;
        at[element] = _size;
        sift_up(_size++);
    }

    int get_index(int element) {
        return at[element];
    }

    int top() {
        if (empty()) {
            throw EmptyQueueException();
        }
        return data[0];
    }

    int pop() {
        if (empty()) {
            throw EmptyQueueException();
        }
        int result = data[0];
        at[result] = -1;
        if (_size == 1) {
            _size = 0;
            return result;
        }
        data[0] = data[--_size];
        at[data[0]] = 0;
        sift_down(0);
        return result;
    }

    bool remove(int element) {
        int index = get_index(element);
        if (index == -1) {
            return false;
        }
        int result = data[index];
        at[result] = -1;
        if (index == _size - 1) {
            _size--;
            return true;
        }
        data[index] = data[--_size];
        at[data[index]] = index;
        sift_down(index);
        sift_up(index);
        return true;
    }

    void clear() {
        _size = 0;
        std::fill(at.begin(), at.end(), -1);
    }
};

#endif //ALGORITHM_LIB_CPP14_INT_HEAP_H
