#ifndef ALGORITHM_LIB_CPP14_SORTER_H
#define ALGORITHM_LIB_CPP14_SORTER_H

#include <vector>
#include <algorithm>

#include "integer_utils.h"
#include "../collections/comparator/comparator.h"

class Sorter {
public:
    template <class T, class Comparator>
    static void sort(std::vector<T> &list, Comparator comparator = Comparator()) {
        quick_sort<T, Comparator>(list, 0, list.size() - 1,
                                  (IntegerUtils::bit_count(
                                          IntegerUtils::highest_one_bit(static_cast<int>(list.size())) - 1) * 5) >> 1,
                                  comparator);
    }

private:
    static const int insertion_threshold = 16;

    template <class T, class Comparator>
    static void quick_sort(std::vector<T> &list, int from, int to, int remaining,
                           Comparator comparator = Comparator()) {
        if (to - from < insertion_threshold) {
            insertion_sort<T, Comparator>(list, from, to, comparator);
            return;
        }
        if (remaining == 0) {
            heap_sort<T, Comparator>(list, from, to, comparator);
            return;
        }
        remaining--;
        int pivot_index = (from + to) >> 1;
        T pivot = list[pivot_index];
        std::swap(list[pivot_index], list[to]);
        int store_index = from;
        int equal_index = to;
        for (int i = from; i < equal_index; i++) {
            T value = list[i];
            bool compare = comparator(value, pivot);
            bool reverse_compare = comparator(pivot, value);
            if (compare) {
                std::swap(list[store_index++], list[i]);
            } else if (!reverse_compare) {
                std::swap(list[--equal_index], list[i--]);
            }
        }
        quick_sort<T, Comparator>(list, from, store_index - 1, remaining, comparator);
        for (int i = equal_index; i <= to; i++) {
            std::swap(list[store_index++], list[i]);
        }
        quick_sort<T, Comparator>(list, store_index, to, remaining, comparator);
    }

    template <class T, class Comparator>
    static void heap_sort(std::vector<T> &list, int from, int to, Comparator comparator = Comparator()) {
        for (int i = (to + from - 1) >> 1; i >= from; i--) {
            sift_down<T, Comparator>(list, i, to, from, comparator);
        }
        for (int i = to; i > from; i--) {
            std::swap(list[from], list[i]);
            sift_down<T, Comparator>(list, from, i - 1, from, comparator);
        }
    }

    template <class T, class Comparator>
    static void sift_down(std::vector<T> &list, int start, int end, int delta, Comparator comparator = Comparator()) {
        T value = list[start];
        while (true) {
            int child = ((start - delta) << 1) + 1 + delta;
            if (child > end) {
                return;
            }
            T child_value = list[child];
            if (child + 1 <= end) {
                T other_value = list[child + 1];
                bool reverse_compare = comparator(child_value, other_value);
                if (reverse_compare) {
                    child++;
                    child_value = other_value;
                }
            }
            if (!comparator(value, child_value)) {
                return;
            }
            std::swap(list[start], list[child]);
            start = child;
        }
    }

    template <class T, class Comparator = std::less<T>>
    static void insertion_sort(std::vector<T> &list, int from, int to, Comparator comparator = Comparator()) {
        for (int i = from + 1; i <= to; i++) {
            T value = list[i];
            for (int j = i - 1; j >= from; j--) {
                bool compare = comparator(list[j], value);
                bool reverse_compare = comparator(value, list[j]);
                if (compare || !reverse_compare) {
                    break;
                }
                std::swap(list[j], list[j + 1]);
            }
        }
    }
};

#endif //ALGORITHM_LIB_CPP14_SORTER_H
