#include <utility>
#include "kth_statistic_predictor_simple.h"

fixed_ratio_sample_sizes::fixed_ratio_sample_sizes(size_t phase_1_divisor, size_t phase_2_divisor):
    _phase_1_divisor(phase_1_divisor), _phase_2_divisor(phase_2_divisor) {}

bool fixed_ratio_sample_sizes::is_size_acceptable(size_t n) {
    return n >= 2 * _phase_1_divisor;
}

size_t fixed_ratio_sample_sizes::n_phase_1_samples(size_t n) {
    return n / _phase_1_divisor;
}

size_t fixed_ratio_sample_sizes::n_phase_2_samples(size_t n, size_t phase_1) {
    return std::min(phase_1, 1 + phase_1 / _phase_2_divisor);
}
