#include "tests.h"
#include "util.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <limits>

void test_random(kth_statistic<int> *algorithm, size_t size, size_t count, size_t seed) {
    test_common(algorithm, size, "test_random", 10000000);

    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<size_t> pos_gen(0, size - 1);
    std::uniform_int_distribution<int> val_gen(std::numeric_limits<int>::min(),
                                               std::numeric_limits<int>::max());

    int *reference = new int[size];
    int *working = new int[size];

    for (size_t attempt = 0; attempt < count; ++attempt) {
        size_t k = pos_gen(rng);
        for (size_t i = 0; i < size; ++i) {
            reference[i] = val_gen(rng);
            working[i] = reference[i];
        }
        std::nth_element(working, working + k, working + size);
        int expected = working[k];
        array_copy(reference, size, working);
        int result = algorithm->find(working, size, k);
        if (expected != result) {
            std::cerr << "[test_random, " << algorithm->name()
                      << "] Expected " << expected << ", found " << result
                      << " on test with k = " << k << std::endl;
            if (size <= 100) {
                std::cerr << "    Array is [";
                for (size_t i = 0; i < size; ++i) {
                    std::cerr << reference[i];
                    if (i + 1 == size) {
                        std::cerr << "]" << std::endl;
                    } else {
                        std::cerr << ", ";
                    }
                }
                std::exit(1);
            } else {
                std::cerr << "    Seed was " << seed << ", attempt was " << attempt << std::endl;
            }
        }
    }

    delete[] reference;
    delete[] working;
}
