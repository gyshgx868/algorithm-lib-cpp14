#ifndef ALGORITHM_LIB_CPP14_RECURSIVE_INDEPENDENT_SET_SYSTEM_H
#define ALGORITHM_LIB_CPP14_RECURSIVE_INDEPENDENT_SET_SYSTEM_H

#include <vector>

class DefaultListener {
public:
    void joined(int joint_root, int root) {
        return;
    }
};

template <class Listener = DefaultListener>
class RecursiveIndependentSetSystem {
private:
    std::vector<int> color;
    std::vector<int> rank;
    int set_count;
    Listener listener;

public :
    RecursiveIndependentSetSystem(int size) {
        color.resize(static_cast<std::size_t>(size));
        rank.resize(static_cast<std::size_t>(size));
        for (int i = 0; i < size; i++) {
            color[i] = i;
        }
        set_count = size;
    }

    RecursiveIndependentSetSystem(const RecursiveIndependentSetSystem<Listener> &other) {
        color = other.color;
        rank = other.rank;
        set_count = other.set_count;
        listener = other.listener;
    }

    bool join(int first, int second) {
        first = get(first);
        second = get(second);
        if (first == second) {
            return false;
        }
        if (rank[first] < rank[second]) {
            std::swap(first, second);
        } else if (rank[first] == rank[second]) {
            rank[first]++;
        }
        set_count--;
        color[second] = first;
        listener.joined(second, first);
        return true;
    }

    int get(int index) {
        int start = index;
        while (color[index] != index) {
            index = color[index];
        }
        while (start != index) {
            int next = color[start];
            color[start] = index;
            start = next;
        }
        return index;
    }

    int get_set_count() {
        return set_count;
    }
};

#endif //ALGORITHM_LIB_CPP14_RECURSIVE_INDEPENDENT_SET_SYSTEM_H
