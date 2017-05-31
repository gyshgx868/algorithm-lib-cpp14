#ifndef ALGORITHM_LIB_CPP14_INT_HASH_MAP_H
#define ALGORITHM_LIB_CPP14_INT_HASH_MAP_H

#include <cmath>
#include <vector>
#include <algorithm>

#include "../../random.h"
#include "../../exceptions/no_such_element_exception.h"

class IntHashMap {
    Random random;
    int shifts[4];
    char present_mask = 1;
    char removed_mask = 2;
    int _size;
    int real_size;
    std::vector<int> keys;
    std::vector<int> values;
    std::vector<char> present;
    int step;
    int ratio;

    void init_step(int capacity) {
        step = random.next_int(capacity - 2) + 1;
        while (std::__gcd(step, capacity) != 1) {
            step++;
        }
    }

    int get_hash(int key) {
        int result = key;
        for (int i : shifts) {
            result ^= key >> i;
        }
        result %= keys.size();
        if (result < 0) {
            result += keys.size();
        }
        return result;
    }

    void resize(int capacity) {
        if (keys.size() < capacity) {
            capacity = std::max(capacity * 2, (int) keys.size());
            rebuild(capacity);
        }
    }

    void squish() {
        if (keys.size() > _size * ratio * 2 + 10) {
            rebuild(_size * ratio + 3);
        }
    }

    void rebuild(int capacity) {
        init_step(capacity);
        keys.resize((unsigned int) capacity);
        present.resize((unsigned int) capacity);
        values.resize((unsigned int) capacity);
    }

public:
    IntHashMap(int capacity = 3) {
        for (int i = 0; i < 4; i++) {
            shifts[i] = random.next_int(31) + 1;
        }
        _size = 0;
        real_size = 0;
        capacity = std::max(capacity, 1);
        keys.resize((unsigned int) capacity);
        present.resize((unsigned int) capacity);
        values.resize((unsigned int) capacity);
        ratio = 2;
        init_step(capacity);
    }

    int size() const {
        return _size;
    }

    void put(int key, int value) {
        resize((real_size + 1) * ratio + 2);
        int current = get_hash(key);
        while (present[current] != 0) {
            if ((present[current] & present_mask) != 0 && keys[current] == key) {
                values[current] = value;
                return;
            }
            current += step;
            if (current >= values.size()) {
                current -= values.size();
            }
        }
        while ((present[current] & present_mask) != 0) {
            current += step;
            if (current >= keys.size()) {
                current -= keys.size();
            }
        }
        if (present[current] == 0) {
            real_size++;
        }
        present[current] = present_mask;
        keys[current] = key;
        values[current] = value;
        _size++;
    }

    void remove(int value) {
        int current = get_hash(value);
        while (present[current] != 0) {
            if (keys[current] == value && (present[current] & present_mask) != 0) {
                present[current] = removed_mask;
                _size--;
                squish();
                return;
            }
            current += step;
            if (current >= keys.size()) {
                current -= keys.size();
            }
        }
    }

    bool contains(int key) {
        int current = get_hash(key);
        while (present[current] != 0) {
            if (keys[current] == key && (present[current] & present_mask) != 0) {
                return true;
            }
            current += step;
            if (current >= keys.size()) {
                current -= keys.size();
            }
        }
        return false;
    }

    int get(int key) {
        int current = get_hash(key);
        while (present[current] != 0) {
            if (keys[current] == key && (present[current] & present_mask) != 0) {
                return values[current];
            }
            current += step;
            if (current >= keys.size()) {
                current -= keys.size();
            }
        }
        throw NoSuchElementException();
    }
};

#endif //ALGORITHM_LIB_CPP14_INT_HASH_MAP_H
