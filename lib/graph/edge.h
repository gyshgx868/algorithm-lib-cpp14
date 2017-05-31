#ifndef ALGORITHM_LIB_CPP14_EDGE_H
#define ALGORITHM_LIB_CPP14_EDGE_H

template <class T>
class Edge {
public:
    int _id;
    int _source;
    int _destination;
    T _weight;
    int _capacity;
    int _flow;
    int _transposed_id;
    int _reverse_id;

    Edge(int id = -1) {
        this->_id = id;
    }

    ~Edge() {
    }

    int source() {
        return _source;
    }

    int destination() {
        return _destination;
    }

    T weight() {
        return _weight;
    }

    T capacity() {
        return _capacity;
    }

    T flow() {
        return _flow;
    }

    int transposed_id() {
        return _transposed_id;
    }

    int reverse_id() {
        return _reverse_id;
    }

    int get_id() {
        return _id;
    }
};

#endif //ALGORITHM_LIB_CPP14_EDGE_H
