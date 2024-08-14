#include <cstdlib>
#include <iostream>

#include "kth_statistic.h"
#include "tests.h"

#include "kth_statistic_stl.h"
#include "kth_statistic_hoare.h"
#include "kth_statistic_predictor_simple.h"

void test_all(kth_statistic<int> *algorithm) {
    const char *name = algorithm->name();
    for (size_t size = 1; size <= 16; ++size) {
        test_all_01s(algorithm, size);
    }
    std::cout << name << ": test_all_01s OK" << std::endl;

    for (size_t size = 1; size <= 9; ++size) {
        test_all_perms(algorithm, size);
    }
    std::cout << name << ": test_all_perms OK" << std::endl;

    size_t rnd_sizes[] = { 10, 100, 1000, 10000, 100000, 1000000 };

    for (size_t idx = 0; idx < 6; ++idx) {
        size_t size = rnd_sizes[idx];
        size_t count = 10000000 / size;
        size_t seed = 87512451357632 * (idx + 1);
        test_random(algorithm, size, count, seed);
        std::cout << name << ": test_random OK (size " << size << ")" << std::endl;
    }

    for (size_t idx = 0; idx < 6; ++idx) {
        size_t size = rnd_sizes[idx];
        size_t count = 10000000 / size;
        size_t seed = 87512451357631 * (idx + 1);
        test_random_repeated(algorithm, size, count, seed);
        std::cout << name << ": test_random_repeated OK (size " << size << ")" << std::endl;
    }
}

int main() {
    stl_kth_statistic<int> stl_int;
    test_all(&stl_int);

    bidirectional_hoare_middle<int> hoare_mid_int;
    test_all(&hoare_mid_int);

    fixed_ratio_sample_sizes fss(10, 10);
    predicting_kth_statistic<int> predicting_int(fss, "simple predicting kth, fixed ratio");
    test_all(&predicting_int);

    tuned_ratio_sample_sizes tss;
    predicting_kth_statistic<int> predicting_int_tuned(tss, "simple predicting kth, tuned");
    test_all(&predicting_int_tuned);

    return 0;
}
