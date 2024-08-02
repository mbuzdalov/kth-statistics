#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <utility>

/*
 * A k-th order statistic of a sequence of elements with a total ordering
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
