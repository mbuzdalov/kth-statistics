#include "tests.h"

#include <cstdlib>
#include <iostream>

void test_all_01s(kth_statistic<int> *algorithm, size_t size) {
    test_common(algorithm, size, "test_all_01s", 30);

    int *values = new int[size];
    size_t mask_max = size_t(1) << size;
    for (size_t mask = 0; mask < mask_max; ++mask) {
        for (size_t k = 0; k < size; ++k) {
            size_t count_1 = 0;
            for (size_t i = 0; i < size; ++i) {
                values[i] = int((mask >> i) & 1);
                count_1 += values[i];
            }
            int result = algorithm->find(values, size, k);
            int expected = k + count_1 >= size ? 1 : 0;
            if (expected != result) {
                std::cerr << "[test_all_01s, " << algorithm->name()
                          << "] Expected " << expected << ", found " << result
                          << " on test: k = " << k << ", array [";
                for (size_t i = 0; i < size; ++i) {
                    std::cerr << ((mask >> i) & 1);
                    if (i + 1 == size) {
                        std::cerr << "]" << std::endl;
                    } else {
                        std::cerr << ", ";
                    }
                }
                std::exit(1);
            }
        }
    }
    delete values;
}
