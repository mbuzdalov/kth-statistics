#pragma once

#include <limits>
#include <utility>

#include "kth_statistic.h"

template<typename element_t>
struct bidirectional_hoare_middle : kth_statistic<element_t> {
    char const *name() const { return "bidirectional Hoare"; }
    bool is_inplace() const { return true; }
    bool is_destructive() const { return false; }
    size_t size() { return std::numeric_limits<size_t>::max(); }
    void resize(size_t new_size) {}

    element_t find(element_t *arr, size_t size, size_t k) {
        if (size == 2) {
            if (arr[0] > arr[1]) {
                std::swap(arr[0], arr[1]);
            }
        } else if (size > 2) {
            element_t *from = arr, *to = arr + size - 1;
            element_t *arr_k = arr + k;
            while (to > from) {
                element_t pivot = from[(to - from) >> 1];
                element_t *l = from, *r = to;
                do {
                    while (*l < pivot) ++l;
                    while (*r > pivot) --r;
                    if (l <= r) {
                        std::swap(*l, *r);
                        ++l;
                        --r;
                    }
                } while (l <= r);
                if (arr_k <= r) {
                    to = r;
                } else if (l <= arr_k) {
                    from = l;
                } else break;
            }
        }
        return arr[k];
    }
};
