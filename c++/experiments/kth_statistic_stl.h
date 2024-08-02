#pragma once

#include <algorithm>
#include <limits>

#include "kth_statistic.h"

template<typename element_t>
struct stl_kth_statistic : kth_statistic<element_t> {
    char const *name() const { return "std::nth_element"; }
    bool is_inplace() const { return true; }
    bool is_destructive() const { return false; }
    size_t size() { return std::numeric_limits<size_t>::max(); }
    void resize(size_t new_size) {}

    element_t find(element_t *start, size_t size, size_t k) {
        element_t *kth = start + k;
        element_t *end = start + size;
        std::nth_element(start, kth, end);
        return *kth;
    }
};
