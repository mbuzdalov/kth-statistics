#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <utility>

/*
 * A k-th statistic of a sequence of elements with a total ordering
 * is the element which would be k-th if this sequence was to be sorted according to this ordering.
 *
 * This problem could be solved by sorting (in time O(n log n) where n is the number of elements),
 * but faster algorithms exist (in time O(n), on average or even in the worst case).
 *
 * An important special case is finding the median, which corresponds to k being equal to n/2.
 *
 * Classic algorithms are often defined as non-destructive ones,
 * that is, after execution they leave the sequence in a permuted state,
 * and they are often in-place.
 *
 * This project aims at exploring what speed-ups are possible if we lift
 * these two requirements: namely, we allow O(n) of additional memory to be pre-allocated and used,
 * and we also allow the original sequence to be completely destroyed.
 *
 * This is a header for EXPERIMENTS.
 * It is not a drop-in replacement for std::nth_element.
 * Use this at your own risk.
 */

template<typename element_t>
struct kth_statistic {
    virtual char const *name() const = 0;
    virtual bool is_inplace() const = 0;
    virtual bool is_destructive() const = 0;
    virtual size_t size() = 0;
    virtual void resize(size_t new_size) = 0;
    virtual element_t find(element_t *start, size_t size, size_t k) = 0;
    virtual ~kth_statistic() {}
};

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

template<typename element_t>
struct bidirectional_hoare_middle : kth_statistic<element_t> {
    char const *name() const { return "bidirectional Hoare scan with middle pivot"; }
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
