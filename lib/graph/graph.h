#ifndef ALGORITHM_LIB_CPP14_GRAPH_H
#define ALGORITHM_LIB_CPP14_GRAPH_H

#include <vector>
#include <algorithm>
#include <functional>

#include "edge.h"
#include "../exceptions/illegal_argument_exception.h"

template <class T>
class Graph {
protected:
    int _vertex_count;
    int _edge_count;

    Edge<T> create_edge(int id) {
        Edge<T> result;
        result._id = id;
        result._source = source(id);
        result._destination = destination(id);
        result._weight = weight(id);
        result._capacity = capacity(id);
        result._flow = flow(id);
        result._transposed_id = transposed(id);
        result._reverse_id = reverse(id);
        return result;
    }

    virtual int entries_per_edge() {
        return 1;
    }

    int edge_capacity() {
        return _from.size();
    }

    void init_edges() {
        if (!edges_initialized) {
            edges_initialized = true;
            _edges.resize(_from.size());
            for (int i = 0; i < _edge_count; i++) {
                _edges[i] = create_edge(i);
            }
        }
    }

    virtual void ensure_edge_capacity(int size) {
        if (_from.size() < static_cast<std::size_t>(size)) {
            std::size_t new_size = std::max(static_cast<std::size_t>(size), 2 * _from.size());
            if (edges_initialized) {
                _edges.resize(new_size);
            }
            _from.resize(new_size);
            _to.resize(new_size);
            _next_outbound.resize(new_size);
            if (inbound_initialized) {
                _next_inbound.resize(new_size);
            }
            if (weight_initialized) {
                _weight.resize(new_size);
            }
            if (capacity_initialized) {
                _capacity.resize(new_size);
            }
            if (reverse_edge_initialized) {
                _reverse_edge.resize(new_size);
            }
            _flags.resize(new_size);
        }
    }

private:
    bool capacity_initialized;
    std::vector<int> _first_outbound;
    bool inbound_initialized;
    std::vector<int> _first_inbound;
    std::vector<Edge<T>> _edges;
    bool edges_initialized;
    std::vector<int> _next_inbound;
    std::vector<int> _next_outbound;
    std::vector<int> _from;
    std::vector<int> _to;
    std::vector<T> _weight;
    bool weight_initialized;
    std::vector<int> _reverse_edge;
    bool reverse_edge_initialized;
    std::vector<int> _flags;

    void init_graph(int vertex_count, int edge_capacity) {
        _edge_count = 0;
        this->_vertex_count = vertex_count;
        _first_outbound.resize(static_cast<std::size_t>(vertex_count), -1);
        _from.resize(static_cast<std::size_t>(edge_capacity));
        _to.resize(static_cast<std::size_t>(edge_capacity));
        _next_outbound.resize(static_cast<std::size_t>(edge_capacity));
        _flags.resize(static_cast<std::size_t>(edge_capacity));

        edges_initialized = false;
        inbound_initialized = false;
        capacity_initialized = false;
        weight_initialized = false;
        reverse_edge_initialized = false;
    }

    void init_inbound() {
        if (!inbound_initialized) {
            inbound_initialized = true;
            _first_inbound.resize(_first_outbound.size());
            std::fill(_first_inbound.begin(), _first_inbound.begin() + _vertex_count, -1);
            _next_inbound.resize(_from.size());
            for (int i = 0; i < _edge_count; i++) {
                _next_inbound[i] = _first_inbound[_to[i]];
                _first_inbound[_to[i]] = i;
            }
        }
    }

    void ensure_vertex_capacity(int size) {
        if (_first_outbound.size() < size) {
            std::size_t new_size = std::max(static_cast<std::size_t>(size), 2 * _from.size());
            _first_outbound.resize(new_size);
            if (inbound_initialized) {
                _first_inbound.resize(new_size);
            }
        }
    }

public:
    static const int removed_bit = 0;
    std::vector<T> _capacity;

    explicit Graph(int vertex_count) {
        init_graph(vertex_count, vertex_count);
    }

    explicit Graph(int vertex_count, int edge_capacity) {
        init_graph(vertex_count, edge_capacity);
    }

    ~Graph() {
    }

    static Graph create_graph(int vertex_count, const std::vector<int> &from, const std::vector<int> &to) {
        Graph graph(vertex_count, from.size());
        for (int i = 0; i < from.size(); i++) {
            graph.add_simple_edge(from[i], to[i]);
        }
        return graph;
    }

    static Graph create_weighted_graph(int vertex_count, const std::vector<int> &from, const std::vector<int> &to,
                                       const std::vector<T> &weight) {
        Graph graph(vertex_count, from.size());
        for (int i = 0; i < from.size(); i++) {
            graph.add_weighted_edge(from[i], to[i], weight[i]);
        }
        return graph;
    }

    static Graph create_flow_graph(int vertex_count, const std::vector<int> &from, const std::vector<int> &to,
                                   const std::vector<T> &capacity) {
        Graph graph(vertex_count, from.size() * 2);
        for (int i = 0; i < from.size(); i++) {
            graph.add_flow_edge(from[i], to[i], capacity[i]);
        }
        return graph;
    }

    static Graph create_flow_weighted_graph(int vertex_count, const std::vector<int> &from, const std::vector<int> &to,
                                            const std::vector<T> &weight, const std::vector<T> &capacity) {
        Graph graph(vertex_count, from.size() * 2);
        for (int i = 0; i < from.size(); i++) {
            graph.add_flow_weighted_edge(from[i], to[i], weight[i], capacity[i]);
        }
        return graph;
    }

    static Graph create_tree(const std::vector<int> &parent) {
        Graph graph(parent.size() + 1, parent.size());
        for (int i = 0; i < parent.size(); i++) {
            graph.add_simple_edge(parent[i], i + 1);
        }
        return graph;
    }

    virtual int add_edge(int from_id, int to_id, T weight, T capacity, int reverse_edge) {
        ensure_edge_capacity(_edge_count + 1);
        if (_first_outbound[from_id] != -1) {
            _next_outbound[_edge_count] = _first_outbound[from_id];
        } else {
            _next_outbound[_edge_count] = -1;
        }
        _first_outbound[from_id] = _edge_count;
        if (inbound_initialized) {
            if (_first_inbound[to_id] != -1) {
                _next_inbound[_edge_count] = _first_inbound[to_id];
            } else {
                _next_inbound[_edge_count] = -1;
            }
            _first_inbound[to_id] = _edge_count;
        }
        this->_from[_edge_count] = from_id;
        this->_to[_edge_count] = to_id;
        if (capacity != 0) {
            if (!capacity_initialized) {
                capacity_initialized = true;
                this->_capacity.resize(_from.size());
            }
            this->_capacity[_edge_count] = capacity;
        }
        if (weight != 0) {
            if (!weight_initialized) {
                weight_initialized = true;
                this->_weight.resize(_from.size());
            }
            this->_weight[_edge_count] = weight;
        }
        if (reverse_edge != -1) {
            if (!reverse_edge_initialized) {
                reverse_edge_initialized = true;
                this->_reverse_edge.resize(_from.size());
                std::fill(_reverse_edge.begin(), _reverse_edge.begin() + _edge_count, -1);
            }
            this->_reverse_edge[_edge_count] = reverse_edge;
        }
        if (edges_initialized) {
            _edges[_edge_count] = create_edge(_edge_count);
        }
        return _edge_count++;
    }

    int add_flow_weighted_edge(int from, int to, T weight, T capacity) {
        if (capacity == 0) {
            return add_edge(from, to, weight, 0, -1);
        } else {
            int last_edge_count = _edge_count;
            add_edge(to, from, -weight, 0, last_edge_count + entries_per_edge());
            return add_edge(from, to, weight, capacity, last_edge_count);
        }
    }

    int add_flow_edge(int from, int to, T capacity) {
        return add_flow_weighted_edge(from, to, 0, capacity);
    }

    int add_weighted_edge(int from, int to, T weight) {
        return add_flow_weighted_edge(from, to, weight, 0);
    }

    int add_simple_edge(int from, int to) {
        return add_weighted_edge(from, to, 0);
    }

    void add_vertices(int count) {
        ensure_vertex_capacity(_vertex_count + count);
        std::fill(_first_outbound.begin() + _vertex_count, _first_outbound.begin() + _vertex_count + count, -1);
        if (inbound_initialized) {
            std::fill(_first_inbound.begin() + _vertex_count, _first_inbound.begin() + _vertex_count + count, -1);
        }
        _vertex_count += count;
    }

    void remove_vertex(int vertex) {
        int id = _first_outbound[vertex];
        while (id != -1) {
            remove_edge(id);
            id = _next_outbound[id];
        }
        init_inbound();
        id = _first_inbound[vertex];
        while (id != -1) {
            remove_edge(id);
            id = _next_inbound[id];
        }
    }

    int vertex_count()const {
        return _vertex_count;
    }

    int edge_count()const {
        return _edge_count;
    }

    Edge<T> edge(int id) {
        init_edges();
        return _edges[id];
    }

    int first_outbound(int vertex) const {
        int id = _first_outbound[vertex];
        while (id != -1 && is_removed(id)) {
            id = _next_outbound[id];
        }
        return id;
    }

    int next_outbound(int id) const {
        id = _next_outbound[id];
        while (id != -1 && is_removed(id)) {
            id = _next_outbound[id];
        }
        return id;
    }

    int first_inbound(int vertex) {
        init_inbound();
        int id = _first_inbound[vertex];
        while (id != -1 && is_removed(id)) {
            id = _next_inbound[id];
        }
        return id;
    }

    int next_inbound(int id) {
        init_inbound();
        id = _next_inbound[id];
        while (id != -1 && is_removed(id)) {
            id = _next_inbound[id];
        }
        return id;
    }

    int source(int id)const {
        return _from[id];
    }

    int destination(int id) const {
        return _to[id];
    }

    T weight(int id) const {
        if (!weight_initialized) {
            return 0;
        }
        return _weight[id];
    }

    T capacity(int id) const {
        if (!capacity_initialized) {
            return 0;
        }
        return _capacity[id];
    }

    T flow(int id) const {
        if (!reverse_edge_initialized) {
            return 0;
        }
        return _capacity[_reverse_edge[id]];
    }

    void push_flow(int id, T flow) {
        if (flow == 0) {
            return;
        }
        if (flow > 0) {
            if (capacity(id) < flow) {
                throw IllegalArgumentException("Not enough capacity");
            }
        } else {
            if (flow(id) < -flow) {
                throw IllegalArgumentException("Not enough capacity");
            }
        }
        _capacity[id] -= flow;
        _capacity[_reverse_edge[id]] += flow;
    }

    virtual int transposed(int id) {
        return -1;
    }

    int reverse(int id) {
        if (!reverse_edge_initialized) {
            return -1;
        }
        return _reverse_edge[id];
    }

    bool flag(int id, int bit) const {
        return (_flags[id] >> bit & 1) != 0;
    }

    void set_flag(int id, int bit) {
        _flags[id] |= 1 << bit;
    }

    void remove_flag(int id, int bit) {
        _flags[id] &= -1 - (1 << bit);
    }

    void remove_edge(int id) {
        set_flag(id, removed_bit);
    }

    void restore_edge(int id) {
        remove_flag(id, removed_bit);
    }

    bool is_removed(int id) const {
        return flag(id, removed_bit);
    }

    bool is_sparse() {
        return _vertex_count == 0 || _edge_count * 20 / _vertex_count <= _vertex_count;
    }

    // TODO: iterator for inbound and outbound
};

#endif //ALGORITHM_LIB_CPP14_GRAPH_H
