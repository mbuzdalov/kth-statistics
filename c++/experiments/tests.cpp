#include <cstdlib>
#include <iostream>

#include "kth_statistic.h"
#include "tests.h"

#include "kth_statistic_stl.h"
#include "kth_statistic_hoare.h"

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
    }
    std::cout << algorithm->name() << ": test_random OK" << std::endl;
}

int main() {
    kth_statistic<int> *stl_int = new stl_kth_statistic<int>();
    test_all(stl_int);
    delete stl_int;

    kth_statistic<int> *hoare_mid_int = new bidirectional_hoare_middle<int>();
    test_all(hoare_mid_int);
    delete hoare_mid_int;
}
