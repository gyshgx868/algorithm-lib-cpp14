#ifndef ALGORITHM_LIB_CPP14_INTEGER_UTILS_H
#define ALGORITHM_LIB_CPP14_INTEGER_UTILS_H

class IntegerUtils {
public:
    static int highest_one_bit(int value) {
        value |= (value >> 1);
        value |= (value >> 2);
        value |= (value >> 4);
        value |= (value >> 8);
        value |= (value >> 16);
        return value - (value >> 1);
    }

    static long long highest_one_bit(long long value) {
        value |= (value >> 1);
        value |= (value >> 2);
        value |= (value >> 4);
        value |= (value >> 8);
        value |= (value >> 16);
        value |= (value >> 32);
        return value - (value >> 1);
    }

    static int bit_count(int value) {
        value = value - ((value >> 1) & 0x55555555);
        value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
        value = (value + (value >> 4)) & 0x0f0f0f0f;
        value = value + (value >> 8);
        value = value + (value >> 16);
        return value & 0x3f;
    }
};

#endif //ALGORITHM_LIB_CPP14_INTEGER_UTILS_H
