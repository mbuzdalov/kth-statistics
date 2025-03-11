#ifndef __KTH_STATISTIC__HOARE_HPP__
#define __KTH_STATISTIC__HOARE_HPP__

#include <algorithm>
#include <limits>
#include <string>

template <typename T, typename C, typename I>
struct kth_statistic_algorithm_hoare {
    size_t max_size() const {
        return std::numeric_limits<size_t>::max();
    }

    void resize(size_t ignored) {}

    std::string name() const {
        return "Hoare's quickselect";
    }

    T operator()(I iterator, size_t size, size_t k) const {
        C comp;
        if (size == 2) {
            if (comp(iterator[1], iterator[0])) {
                std::swap(iterator[0], iterator[1]);
            }
        } else if (size > 2) {
            I from = iterator, to = iterator + size - 1;
            I arr_k = iterator + k;
            while (to > from) {
                T pivot = from[(to - from) >> 1];
                I l = from, r = to;
                do {
                    while (comp(*l, pivot)) ++l;
                    while (comp(pivot, *r)) --r;

                    // The correctness of the statement below and subsequent comparisons
                    // requires an iterator to be still valid when equivalent to an index -1,
                    // i.e. pointing before the beginning of the sorted sequence.
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
        return iterator[k];
    }
};

#endif // __KTH_STATISTIC__HOARE_HPP__
