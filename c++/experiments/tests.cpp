#include <cstdlib>
#include <iostream>

#include "kth_statistic.h"
#include "tests.h"

#include "kth_statistic_stl.h"
#include "kth_statistic_hoare.h"
#include "kth_statistic_predictor_simple.h"

void test_all(kth_statistic<int> *algorithm) {
    for (size_t size = 1; size <= 16; ++size) {
        test_all_01s(algorithm, size);
    }
    std::cout << algorithm->name() << ": test_all_01s OK" << std::endl;

    for (size_t size = 1; size <= 9; ++size) {
        test_all_perms(algorithm, size);
    }
    std::cout << algorithm->name() << ": test_all_perms OK" << std::endl;

    size_t rnd_sizes[] = { 10, 100, 1000, 10000, 100000, 1000000 };

    for (size_t idx = 0; idx < 6; ++idx) {
        size_t size = rnd_sizes[idx];
        size_t count = 10000000 / size;
        size_t seed = 87512451357632 * (idx + 1);
        test_random(algorithm, size, count, seed);
        std::cout << algorithm->name() << ": test_random OK (size " << size << ")" << std::endl;
    }

    for (size_t idx = 0; idx < 6; ++idx) {
        size_t size = rnd_sizes[idx];
        size_t count = 10000000 / size;
        size_t seed = 87512451357631 * (idx + 1);
        test_random_repeated(algorithm, size, count, seed);
        std::cout << algorithm->name() << ": test_random_repeated OK (size " << size << ")" << std::endl;
    }
}

struct example_sample_sizes : sample_sizes {
    bool is_size_acceptable(size_t n) {
        return n >= 20;
    }
    size_t n_phase_1_samples(size_t n) {
        return n / 10;
    }
    size_t n_phase_2_samples(size_t n, size_t phase_1) {
        return 1 + phase_1 / 10;
    }
};


int main() {
    stl_kth_statistic<int> stl_int;
    test_all(&stl_int);

    bidirectional_hoare_middle<int> hoare_mid_int;
    test_all(&hoare_mid_int);

    example_sample_sizes ess;
    predicting_kth_statistic<int> predicting_int(&ess);
    test_all(&predicting_int);

    return 0;
}
