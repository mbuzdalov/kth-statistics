#include <cmath>
#include <utility>
#include <vector>
#include "kth_statistic_predictor_simple.h"

//////////////////////////////
// fixed_ratio_sample_sizes //
//////////////////////////////

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

//////////////////////////////
// tuned_ratio_sample_sizes //
//////////////////////////////

struct precalc_double_power {
    const double lo_power, lo_mult, hi_power, hi_mult;
    const size_t lo_max, hi_min;

    std::vector<double> precalc;
    precalc_double_power(double lo_power, double lo_mult, size_t lo_max,
                         double hi_power, double hi_mult, size_t hi_min)
      : lo_power(lo_power), lo_mult(lo_mult)
      , hi_power(hi_power), hi_mult(hi_mult)
      , lo_max(lo_max), hi_min(hi_min)
      , precalc(hi_min + 1) {
        for (size_t n = 1; n <= lo_max; ++n) {
            precalc[n] = pow(n, lo_power) * lo_mult;
        }
        precalc[hi_min] = pow(hi_min, hi_power) * hi_mult;
        double log_lo_max = log(lo_max);
        double log_hi_min = log(hi_min);
        double log_lo_val = log(precalc[lo_max]);
        double log_hi_val = log(precalc[hi_min]);
        for (size_t n = lo_max + 1; n < hi_min; ++n) {
            double x = (log(n) - log_lo_max) / (log_hi_min - log_lo_max);
            double y = log_lo_val + x * (log_hi_val - log_lo_val);
            precalc[n] = exp(y);
        }
    }

    double value(size_t n) const {
        if (n < precalc.size()) {
            return precalc[n];
        } else {
            return pow(n, hi_power) * hi_mult;
        }
    }
};

const precalc_double_power guess_x(0.9, 0.63, 100,  0.58, 1.29, 1000);
const precalc_double_power guess_y(0.8, 0.63, 100,  1.0 / 3, 1.3, 1000);

tuned_ratio_sample_sizes::tuned_ratio_sample_sizes() {}

bool tuned_ratio_sample_sizes::is_size_acceptable(size_t n) {
    return n >= 10;
}

size_t tuned_ratio_sample_sizes::n_phase_1_samples(size_t n) {
    return size_t(n / guess_y.value(n));
}

size_t tuned_ratio_sample_sizes::n_phase_2_samples(size_t n, size_t phase_1) {
    size_t guess = 1 + size_t (n / guess_x.value(n));
    return std::min(guess, phase_1);
}
