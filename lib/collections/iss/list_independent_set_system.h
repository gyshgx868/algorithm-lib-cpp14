#ifndef ALGORITHM_LIB_CPP14_LIST_INDEPENDENT_SET_SYSTEM_H
#define ALGORITHM_LIB_CPP14_LIST_INDEPENDENT_SET_SYSTEM_H

#include <vector>

#include "../../exceptions/illegal_argument_exception.h"

class DefaultListener {
public:
    void joined(int joint_root, int root) {
        return;
    }
};

template <class Listener = DefaultListener>
class ListIndependentSetSystem {
private:
    std::vector<std::vector<int>> lists;
    std::vector<int> parent;
    int setCount;
    Listener listener;

public:
    explicit ListIndependentSetSystem(int size) {
        lists.resize((unsigned int) size);
        parent.resize((unsigned int) size);
        for (int i = 0; i < size; i++) {
            parent[i] = i;
        }
        setCount = size;
    }

    bool join(int first, int second) {
        first = parent[first];
        second = parent[second];
        if (first == second) {
            return false;
        }
        int firstSize = lists[first].size() + 1;
        int secondSize = lists[second].size() + 1;
        if (firstSize < secondSize) {
            std::swap(first, second);
        }
        lists[first].push_back(second);
        parent[second] = first;
        if (lists[second].size() != 0) {
            for (auto iterator = lists[second].begin(); iterator != lists[second].end(); ++iterator) {
                lists[first].push_back(*iterator);
            }
            int finalFirst = first;
            for (auto iterator = lists[second].begin(); iterator != lists[second].end(); ++iterator) {
                parent[*iterator] = finalFirst;
            }
        }
        std::vector<int>().swap(lists[second]);
        listener.joined(second, first);
        setCount--;
        return true;
    }

    int get(int index) {
        return parent[index];
    }

    int getSetCount() {
        return setCount;
    }

    std::vector<int> getChildren(int vertex) {
        if (parent[vertex] != vertex) {
            throw IllegalArgumentException();
        }
        return lists[vertex];
    }

    int getSize(int vertex) {
        vertex = parent[vertex];
        return lists[vertex].size() + 1;
    }
};

#endif //ALGORITHM_LIB_CPP14_LIST_INDEPENDENT_SET_SYSTEM_H
