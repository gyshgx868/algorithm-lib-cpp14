#ifndef ALGORITHM_LIB_CPP14_INT_TREAP_ARRAY_H
#define ALGORITHM_LIB_CPP14_INT_TREAP_ARRAY_H

#include <vector>
#include "../../random.h"
#include "../../exceptions/index_out_of_bounds_exception.h"

class IntTreapArray {
private:
    std::vector<int> left;
    std::vector<int> right;
    std::vector<long long> priority;
    std::vector<int> value;
    std::vector<int> _size;
    int count;
    Random random;
    int root;
    int last;

    void create_arrays(int count) {
        left.resize((unsigned int) count);
        right.resize((unsigned int) count);
        priority.resize((unsigned int) count);
        value.resize((unsigned int) count);
        _size.resize((unsigned int) count);
    }

    int remove(int root, int at) {
        int left_size = size(left[root]);
        if (left_size == at) {
            return merge(left[root], right[root]);
        }
        if (left_size > at) {
            left[root] = remove(left[root], at);
        } else {
            right[root] = remove(right[root], at - left_size - 1);
        }
        update_size(root);
        return root;
    }

    void update_size(int root) {
        _size[root] = size(left[root]) + size(right[root]) + 1;
    }

    int merge(int c_left, int c_right) {
        if (c_left == -1) {
            return c_right;
        }
        if (c_right == -1) {
            return c_left;
        }
        if (priority[c_left] > priority[c_right]) {
            right[c_left] = merge(right[c_left], c_right);
            update_size(c_left);
            return c_left;
        } else {
            left[c_right] = merge(c_left, left[c_right]);
            update_size(c_right);
            return c_right;
        }
    }

    std::pair<int, int> split(int root, int key, int to_add) {
        if (root == -1) {
            return std::make_pair(-1, -1);
        }
        int current_key = to_add + size(left[root]);
        if (key <= current_key) {
            std::pair<int, int> result = split(left[root], key, to_add);
            left[root] = result.second;
            update_size(root);
            return std::make_pair(result.first, root);
        } else {
            std::pair<int, int> result = split(right[root], key, current_key + 1);
            right[root] = result.first;
            update_size(root);
            return std::make_pair(root, result.second);
        }
    }

    int size(int index) {
        if (index == -1) {
            return 0;
        }
        return _size[index];
    }

    void resize(int new_size) {
        if (left.size() < new_size) {
            new_size = std::max(new_size, 2 * count);
            left.resize((unsigned int) new_size);
            right.resize((unsigned int) new_size);
            value.resize((unsigned int) new_size);
            priority.resize((unsigned int) new_size);
            _size.resize((unsigned int) new_size);
        }
    }

public:
    IntTreapArray() {
        root = -1;
        last = 0;
        count = 0;
        random = Random(239);
        create_arrays(10);
    }

    explicit IntTreapArray(std::vector<int> array) {
        root = -1;
        last = 0;
        count = 0;
        random = Random(239);
        create_arrays(array.size());
        for (int i = 0; i < array.size(); i++) {
            add(array[i]);
        }
    }

    int get(int at) {
        if (at < 0 || at >= count) {
            throw IndexOutOfBoundsException(at, count);
        }
        int current = root;
        while (true) {
            int left_size = size(left[current]);
            if (left_size == at) {
                return value[current];
            }
            if (left_size > at) {
                current = left[current];
            } else {
                at -= left_size + 1;
                current = right[current];
            }
        }
    }

    void set(int at, int value) {
        if (at < 0 || at >= count) {
            throw IndexOutOfBoundsException(at, count);
        }
        int current = root;
        while (true) {
            int left_size = size(left[current]);
            if (left_size == at) {
                this->value[current] = value;
                return;
            }
            if (left_size > at) {
                current = left[current];
            } else {
                at -= left_size + 1;
                current = right[current];
            }
        }
    }

    void remove_at(int at) {
        root = remove(root, at);
        count--;
    }

    void push_back(int from, int to) {
        std::pair<int, int> first = split(root, to + 1, 0);
        std::pair<int, int> second = split(first.first, from, 0);
        int right = merge(second.first, first.second);
        root = merge(second.second, right);
    }

    void add(int value) {
        add_at(size(), value);
    }

    void add_at(int at, int value) {
        resize(last + 1);
        this->value[last] = value;
        priority[last] = random.next_long();
        left[last] = -1;
        right[last] = -1;
        _size[last] = 1;
        std::pair<int, int> result = split(root, at, 0);
        root = merge(result.first, last);
        root = merge(root, result.second);
        count++;
        last++;
    }

    int size() {
        return count;
    }
};

#endif //ALGORITHM_LIB_CPP14_INT_TREAP_ARRAY_H
