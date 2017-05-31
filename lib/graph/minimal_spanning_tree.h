#ifndef ALGORITHM_LIB_CPP14_MINIMAL_SPANNING_TREE_H
#define ALGORITHM_LIB_CPP14_MINIMAL_SPANNING_TREE_H

#include <set>
#include <vector>

#include "bidirectional_graph.h"
#include "../collections/iss/recursive_independent_set_system.h"
#include "../misc/sorter.h"

class MinimalSpanningTree {
public:
    template <class T>
    static std::set<int> minimal_tree(const BidirectionalGraph<T> &graph) {
        std::set<int> result;
        std::vector<int> edge_order(static_cast<std::size_t>(graph.edge_count()) >> 1);
        int order_count = static_cast<int>(edge_order.size());
        for (int i = 0; i < order_count; i++) {
            edge_order[i] = i << 1;
        }
        Sorter::sort(edge_order, [&](int first, int second) -> bool {
            return graph.weight(first) < graph.weight(second);
        });
        RecursiveIndependentSetSystem<> set_system(graph.vertex_count());
        for (int i : edge_order) {
            if (set_system.join(graph.source(i), graph.destination(i))) {
                result.insert(i);
            }
        }
        if (set_system.get_set_count() == 1) {
            return result;
        } else {
            return std::set<int>();
        }
    }
};

#endif //ALGORITHM_LIB_CPP14_MINIMAL_SPANNING_TREE_H
