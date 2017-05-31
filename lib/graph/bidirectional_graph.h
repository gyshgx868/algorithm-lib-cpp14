#ifndef ALGORITHM_LIB_CPP14_BIDIRECTIONAL_GRAPH_H
#define ALGORITHM_LIB_CPP14_BIDIRECTIONAL_GRAPH_H

#include <vector>

#include "graph.h"

template <class T>
class BidirectionalGraph : public Graph<T> {
public:
    std::vector<int> _transposed_edge;

    explicit BidirectionalGraph(int vertex_count) : Graph<T>(vertex_count) {
        _transposed_edge.resize(2 * static_cast<std::size_t>(vertex_count));
    }

    explicit BidirectionalGraph(int vertex_count, int edge_capacity) : Graph<T>(vertex_count, edge_capacity * 2) {
        _transposed_edge.resize(2 * static_cast<std::size_t>(edge_capacity));
    }

    static BidirectionalGraph create_graph(int vertex_count, const std::vector<int> &from, const std::vector<int> &to) {
        BidirectionalGraph graph(vertex_count, from.size());
        for (int i = 0; i < from.size(); i++) {
            graph.add_simple_edge(from[i], to[i]);
        }
        return graph;
    }

    static BidirectionalGraph create_weighted_graph(int vertex_count, const std::vector<int> &from,
                                                    const std::vector<int> &to, const std::vector<T> &weight) {
        BidirectionalGraph graph(vertex_count, from.size());
        for (int i = 0; i < from.size(); i++) {
            graph.add_weighted_edge(from[i], to[i], weight[i]);
        }
        return graph;
    }

    static BidirectionalGraph create_flow_graph(int vertex_count, const std::vector<int> &from,
                                                const std::vector<int> &to, const std::vector<T> &capacity) {
        BidirectionalGraph graph(vertex_count, from.size() * 2);
        for (int i = 0; i < from.size(); i++) {
            graph.add_flow_edge(from[i], to[i], capacity[i]);
        }
        return graph;
    }

    static BidirectionalGraph create_flow_weighted_graph(int vertex_count, const std::vector<int> &from,
                                                         const std::vector<int> &to, const std::vector<T> &weight,
                                                         const std::vector<T> &capacity) {
        BidirectionalGraph graph(vertex_count, from.size() * 2);
        for (int i = 0; i < from.size(); i++) {
            graph.add_flow_weighted_edge(from[i], to[i], weight[i], capacity[i]);
        }
        return graph;
    }

    int add_edge(int from_id, int to_id, T weight, T capacity, int reverse_edge) override {
        int lastEdgeCount = this->_edge_count;
        ensure_edge_capacity(this->_edge_count + 2);
        Graph<T>::add_edge(from_id, to_id, weight, capacity, reverse_edge);
        Graph<T>::add_edge(to_id, from_id, weight, capacity, reverse_edge == -1 ? -1 : reverse_edge + 1);
        _transposed_edge[lastEdgeCount] = lastEdgeCount + 1;
        _transposed_edge[lastEdgeCount + 1] = lastEdgeCount;
        return lastEdgeCount;
    }

    int transposed(int id) override {
        return _transposed_edge[id];
    }

protected:
    int entries_per_edge() override {
        return 2;
    }

    void ensure_edge_capacity(int size) override {
        if (size > this->edge_capacity()) {
            Graph<T>::ensure_edge_capacity(size);
            _transposed_edge.resize(static_cast<std::size_t>(this->edge_capacity()));
        }
    }
};

#endif //ALGORITHM_LIB_CPP14_BIDIRECTIONAL_GRAPH_H
