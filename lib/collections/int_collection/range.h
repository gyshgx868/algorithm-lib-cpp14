#ifndef ALGORITHM_LIB_CPP14_RANGE_H
#define ALGORITHM_LIB_CPP14_RANGE_H

#include <vector>
#include <cstdlib>

class Range {
public:
    static std::vector<int> range(int from, int to) {
        std::vector<int> result((unsigned int) abs(from - to));
        int current = from;
        if (from <= to) {
            for (int i = 0; i < result.size(); i++) {
                result[i] = current++;
            }
        } else {
            for (int i = 0; i < result.size(); i++) {
                result[i] = current--;
            }
        }
        return result;
    }

    static std::vector<int> range(int to) {
        return range(0, to);
    }
};

#endif //ALGORITHM_LIB_CPP14_RANGE_H
