#ifndef ALGORITHM_LIB_CPP14_INT_SPLAY_SET_H
#define ALGORITHM_LIB_CPP14_INT_SPLAY_SET_H

#include <vector>
#include <algorithm>

#include "../../exceptions/no_such_element_exception.h"
#include "../comparator/int_comparator.h"

template <class Comparator = DefaultIntComparator>
class IntSplaySet {
private:
    std::vector<int> _size;
    Comparator comparator;
    int nextFree;

    struct Split {
        int left;
        int right;

        Split() {
            left = right = -1;
        }
    };

    class SplayIterator {
    private:
        IntSplaySet *set;
        bool first;
        bool exhausted;

    public:
        SplayIterator(IntSplaySet *set) {
            this->set = set;
            first = true;
            exhausted = set->root == -1;
        }

        bool operator ==(const SplayIterator &other) {
            return set == other.set;
        }

        bool operator !=(const SplayIterator &other) {
            return set != other.set;
        }

        int value() {
            if (first) {
                if (set->root == -1) {
                    throw NoSuchElementException();
                }
                int leftmost = set->leftmost(set->root);
                set->splay(leftmost);
                first = false;
            }
            return set->value[set->root];
        }

        bool advance() {
            if (set->root == -1 || exhausted) {
                throw new NoSuchElementException();
            }
            int next = set->leftmost(set->right[set->root]);
            if (next == -1) {
                exhausted = true;
                return false;
            }
            set->splay(next);
            return true;
        }

        bool valid() {
            return !exhausted;
        }
    };

    void splay(int node) {
        if (node == -1) {
            return;
        }
        while (parent[node] != -1) {
            if (parent[parent[node]] == -1) {
                rotate(node);
            } else if ((left[parent[node]] == node) == (left[parent[parent[node]]] == parent[node])) {
                rotate(parent[node]);
                rotate(node);
            } else {
                rotate(node);
                rotate(node);
            }
        }
        root = node;
    }

    void merge(int left, int right) {
        if (left == -1) {
            root = right;
        } else if (right == -1) {
            root = left;
        } else {
            splay(rightmost(left));
            set_right(root, right);
            update(root);
        }
    }

    void set_left(int node, int child) {
        left[node] = child;
        if (child != -1) {
            parent[child] = node;
        }
    }

    void set_right(int node, int child) {
        right[node] = child;
        if (child != -1) {
            parent[child] = node;
        }
    }

protected:
    std::vector<int> value;
    std::vector<int> left;
    std::vector<int> right;
    std::vector<int> parent;
    int root;

    void rotate(int node) {
        int cParent = parent[node];
        if (node == left[cParent]) {
            set_left(cParent, right[node]);
            if (parent[cParent] != -1) {
                if (cParent == left[parent[cParent]]) {
                    set_left(parent[cParent], node);
                } else {
                    set_right(parent[cParent], node);
                }
            } else {
                parent[node] = -1;
            }
            set_right(node, cParent);
        } else {
            set_right(cParent, left[node]);
            if (parent[cParent] != -1) {
                if (cParent == left[parent[cParent]]) {
                    set_left(parent[cParent], node);
                } else {
                    set_right(parent[cParent], node);
                }
            } else {
                parent[node] = -1;
            }
            set_left(node, cParent);
        }
        update(cParent);
        update(node);
    }

    void update(int node) {
        _size[node] = size(left[node]) + size(right[node]) + 1;
    }

    int size(int node) {
        if (node != -1) {
            return _size[node];
        }
        return 0;
    }

    int leftmost(int root) {
        if (root == -1) {
            return -1;
        }
        while (left[root] != -1) {
            root = left[root];
        }
        return root;
    }

    int rightmost(int root) {
        if (root == -1) {
            return -1;
        }
        while (right[root] != -1) {
            root = right[root];
        }
        return root;
    }

    Split split(int root, int value) {
        if (root == -1) {
            return Split();
        }
        int compare = comparator(this->value[root], value);
        if (compare < 0) {
            Split result = split(right[root], value);
            set_right(root, result.left);
            result.left = root;
            update(root);
            behead(root);
            return result;
        } else if (compare > 0) {
            Split result = split(left[root], value);
            set_left(root, result.right);
            result.right = root;
            update(root);
            behead(root);
            return result;
        } else {
            Split result;
            result.left = left[root];
            result.right = right[root];
            behead(left[root]);
            behead(right[root]);
            return result;
        }
    }

    int resize(int capacity) {
        if (parent.size() < capacity) {
            capacity = std::max(2 * parent.size(), (unsigned int) capacity);
            left.resize((unsigned int) capacity);
            right.resize((unsigned int) capacity);
            parent.resize((unsigned int) capacity);
            _size.resize((unsigned int) capacity);
            value.resize((unsigned int) capacity);
            return capacity;
        }
        return parent.size();
    }

    void behead(int node) {
        if (node != -1) {
            parent[node] = -1;
        }
    }

public:
    typedef SplayIterator Iterator;

    IntSplaySet(int capacity = 5) {
        nextFree = 0;
        root = -1;
        value.resize((unsigned int) capacity);
        left.resize((unsigned int) capacity);
        right.resize((unsigned int) capacity);
        parent.resize((unsigned int) capacity);
        _size.resize((unsigned int) capacity);
    }

    IntSplaySet *iterator() {
        return this;
    }

    int size() {
        return size(root);
    }

    void add(int value) {
        if (contains(value)) {
            return;
        }
        resize(nextFree + 1);
        Split roots = split(root, value);
        set_left(nextFree, roots.left);
        set_right(nextFree, roots.right);
        parent[nextFree] = -1;
        this->value[nextFree] = value;
        update(nextFree);
        root = nextFree++;
    }

    bool remove(int value) {
        if (contains(value)) {
            behead(left[root]);
            behead(right[root]);
            merge(left[root], right[root]);
            return true;
        }
        return false;
    }

    bool contains(int value) {
        int node = root;
        int lastNode = -1;
        while (node != -1) {
            lastNode = node;
            int compare = comparator(value, this->value[node]);
            if (compare == 0) {
                splay(node);
                return true;
            }
            if (compare < 0) {
                node = left[node];
            } else {
                node = right[node];
            }
        }
        if (lastNode != -1) {
            splay(lastNode);
        }
        return false;
    }

    int index_of(int value) {
        if (!contains(value)) {
            return -1;
        }
        return size(left[root]);
    }

    int sub_set_size(int fromValue, bool includeFrom, int toValue, bool includeTo) {
        return head_set_size(toValue, includeTo) - head_set_size(fromValue, !includeFrom);
    }

    int head_set_size(int value, bool include) {
        if (contains(value)) {
            return size(left[root]) + (include ? 1 : 0);
        }
        int result = 0;
        int node = root;
        while (node != -1) {
            if (comparator(value, this->value[node]) > 0) {
                result += 1 + size(left[node]);
                node = right[node];
            } else {
                node = left[node];
            }
        }
        return result;
    }

    int tail_set_size(int value, bool include) {
        return size() - head_set_size(value, !include);
    }
};

#endif //ALGORITHM_LIB_CPP14_INT_SPLAY_SET_H
