#pragma once

/*
 * This is a simple implementation of the algorithmic idea
 * proposed by Alvaro Fernandez de Fuente (https://github.com/Alvaro-Fernandez-1/).
 *
 * To find a k-th order statistic:
 * - Subsample the array
 * - Determine the location of k-th order statistic in this array
 * - Find neighboring statistics (here, using std::nth_element)
 * - Filter the part of the array which is between these statistics
 *   - There is a hope that the sought k-th order statistic will be in this part,
 *     and that this part will be sufficiently smaller than the original array
 * - Count also the number of smaller/larger elements to validate where the sought statistic is
 * - If OK, continue with the smaller array (here, using std::nth_element)
 * - If not OK, failover to std::nth_element on the main array
 */

#include <algorithm>
#include <cassert>
#include <limits>

#include "kth_statistic.h"

struct sample_sizes {
    virtual bool is_size_acceptable(size_t n) = 0;
    virtual size_t n_phase_1_samples(size_t n) = 0;
    virtual size_t n_phase_2_samples(size_t n, size_t phase_1) = 0;
    virtual ~sample_sizes() {}
};

struct fixed_ratio_sample_sizes : sample_sizes {
    fixed_ratio_sample_sizes(size_t phase_1_divisor, size_t phase_2_divisor);
    bool is_size_acceptable(size_t n);
    size_t n_phase_1_samples(size_t n);
    size_t n_phase_2_samples(size_t n, size_t phase_1);

private:
    size_t _phase_1_divisor, _phase_2_divisor;
};

struct tuned_ratio_sample_sizes : sample_sizes {
    tuned_ratio_sample_sizes();
    bool is_size_acceptable(size_t n);
    size_t n_phase_1_samples(size_t n);
    size_t n_phase_2_samples(size_t n, size_t phase_1);
};

template<typename element_t>
struct predicting_kth_statistic : kth_statistic<element_t> {
private:
    size_t _size;
    element_t *_mem;
    size_t _hits, _misses;
    size_t _phase_1_samples, _phase_2_samples;
    size_t _n_below, _n_mid, _n_above;
    sample_sizes &_sample_sizes;

public:
    char const *name() const { return "simple predicting kth"; }
    bool is_inplace() const { return false; }
    bool is_destructive() const { return false; } // actually false as we use the aux array
    size_t size() { return _size; }

    predicting_kth_statistic(sample_sizes &sample_sizes,
                             size_t initial_size = 1)
    : _size(initial_size),
      _hits(0), _misses(0), _phase_1_samples(0), _phase_2_samples(0),
      _n_below(0), _n_mid(0), _n_above(0),
      _sample_sizes(sample_sizes) {
        _mem = new element_t[_size];
    }

    void resize(size_t new_size) {
        if (_size != new_size) {
            delete[] _mem;
            _size = new_size;
            _mem = new element_t[_size];
        }
    }

    ~predicting_kth_statistic() {
        delete[] _mem;
    }

    void display_and_reset_statistics(std::ostream &out) {
        out << "    [Hits: " << _hits
            << ", misses: " << _misses
            << ", phase 1 samples avg: " << double(_phase_1_samples) / (_hits + _misses)
            << ", phase 2 samples avg: " << double(_phase_2_samples) / _hits
            << ", below: " << _n_below << ", mid: " << _n_mid << ", above: " << _n_above
            << "]" << std::endl;
        _hits = 0;
        _misses = 0;
        _phase_1_samples = 0;
        _phase_2_samples = 0;
        _n_below = 0;
        _n_mid = 0;
        _n_above = 0;
    }

    element_t find(element_t *start, size_t size, size_t k) {
        if (!_sample_sizes.is_size_acceptable(size)) {
            std::nth_element(start, start + k, start + size);
            return start[k];
        }

        const size_t n_samples = _sample_sizes.n_phase_1_samples(size);
        const size_t proportion = size / n_samples;
        const size_t offset_from_below = (size - (n_samples - 1) * proportion + 1) / 2;

        for (size_t i = 0, j = offset_from_below; i < n_samples; ++i, j += proportion) {
            assert(j < size);
            _mem[i] = start[j];
        }
        _phase_1_samples += n_samples;

        const size_t n_samples_2 = _sample_sizes.n_phase_2_samples(size, n_samples);
        assert(n_samples_2 <= n_samples);

        element_t *last = start + size - 1;
        element_t *mem_end = _mem;
        element_t *subsampled_k;

        if (k < offset_from_below) {
            ++_n_below;
            // k-th order stat is likely <= the smallest element
            std::nth_element(_mem, _mem + n_samples_2 - 1, _mem + n_samples);
            element_t upper = _mem[n_samples_2 - 1];
            for (element_t *curr = start; curr <= last; ++curr) {
                *mem_end = *curr;
                mem_end += *curr <= upper;
            }
            subsampled_k = _mem + k;
        } else if (size - k < offset_from_below) {
            ++_n_above;
            // k-th order stat is likely >= the greatest element
            std::nth_element(_mem, _mem + n_samples - n_samples_2, _mem + n_samples);
            element_t lower = _mem[n_samples - n_samples_2];
            for (element_t *curr = start; curr <= last; ++curr) {
                *mem_end = *curr;
                mem_end += *curr >= lower;
            }
            subsampled_k = mem_end - (size - k);
        } else {
            ++_n_mid;
            element_t *expected_idx = _mem + (k - offset_from_below) / proportion;
            // k-th order stat is likely >= idx-th substat and <= (idx+1)-st substat
            element_t *lower_idx = expected_idx - n_samples_2 / 2;
            element_t *higher_idx = lower_idx + n_samples_2 - 1;
            if (lower_idx < _mem) {
                lower_idx = _mem;
                higher_idx = _mem + n_samples_2 - 1;
            }
            if (higher_idx >= _mem + n_samples) {
                higher_idx = _mem + n_samples - 1;
                lower_idx = higher_idx - n_samples_2 + 1;
            }
            assert(lower_idx >= _mem);
            std::nth_element(_mem, lower_idx, _mem + n_samples);
            std::nth_element(lower_idx + 1, higher_idx, _mem + n_samples);

            element_t lower = *lower_idx;
            element_t upper = *higher_idx;

            element_t *m_start = start;

            // drop elements which are definitely not in the range

            size_t k_change = 0;
            while (*m_start < lower) {
                ++m_start;
                ++k_change;
            }
            while (*m_start > upper) {
                ++m_start;
            }
            while (*last < lower) {
                --last;
                ++k_change;
            }
            while (*last > upper) {
                --last;
            }

            subsampled_k = _mem - 1; // only used if failed

            if (k >= k_change) {
                size_t k_mod = k - k_change;
                if (lower == upper) {
                    size_t count_less = 0;
                    size_t count_eq = 0;
                    for (element_t *curr = m_start; curr <= last; ++curr) {
                        count_less += *curr < lower;
                        count_eq += *curr == lower;
                    }
                    if (k_mod >= count_less && k_mod < count_less + count_eq) {
                        ++_hits;
                        return lower;
                    }
                } else {
                    subsampled_k = _mem + k_mod;
                    for (element_t *curr = m_start; curr <= last; ++curr) {
                        *mem_end = *curr;
                        bool is_lower = *curr < lower;
                        bool is_good = !is_lower && *curr <= upper;
                        subsampled_k -= is_lower;
                        mem_end += is_good;
                    }
                }
            }
        }

        if (subsampled_k >= _mem && subsampled_k < mem_end) {
            ++_hits;
            _phase_2_samples += mem_end - _mem;
            std::nth_element(_mem, subsampled_k, mem_end);
            return *subsampled_k;
        } else {
            ++_misses;
            std::nth_element(start, start + k, start + size);
            return start[k];
        }
    }
};
