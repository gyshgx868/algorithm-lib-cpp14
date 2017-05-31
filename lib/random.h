#ifndef ALGORITHM_LIB_CPP14_RANDOM_H
#define ALGORITHM_LIB_CPP14_RANDOM_H

#include <ctime>
#include <random>
#include <climits>

class Random {
private:
    std::default_random_engine generator;

public:
    Random() {
        generator = std::default_random_engine((unsigned int) time(nullptr));
    }

    explicit Random(unsigned int seed) {
        generator = std::default_random_engine(seed);
    }

    int next_int() {
        return next_int(0, INT_MAX);
    }

    int next_int(int bound) {
        return next_int(0, bound);
    }

    int next_int(int from, int to) {
        std::uniform_int_distribution<int> range(from, to);
        return range(generator);
    }

    long long next_long() {
        return next_long(0, LONG_LONG_MAX);
    }

    long long next_long(long long bound) {
        return next_long(0, bound);
    }

    long long next_long(long long from, long long to) {
        std::uniform_int_distribution<long long> range(from, to);
        return range(generator);
    }

    double next_double(double from, double to) {
        std::uniform_real_distribution<double> range(from, to);
        return range(generator);
    }
};

#endif //ALGORITHM_LIB_CPP14_RANDOM_H
