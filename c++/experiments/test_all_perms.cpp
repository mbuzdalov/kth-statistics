#include "tests.h"
#include "util.h"

#include <cstdlib>
#include <iostream>

class wrapper {
    kth_statistic<int> *algorithm;
    size_t size;
    int *reference_array;
    int *working_array;

public:
    wrapper(kth_statistic<int> *algorithm, size_t size): algorithm(algorithm), size(size) {
        reference_array = new int[size];
        working_array = new int[size];
    }

    void go(size_t depth, size_t used_mask) {
        if (depth == size) {
            for (size_t k = 0; k < size; ++k) {
                array_copy(reference_array, size, working_array);
                int result = algorithm->find(working_array, size, k);
                if (result != k) {
                    std::cerr << "[test_all_perms, " << algorithm->name()
                              << "] Expected " << k << ", found " << result
                              << " on test: k = " << k << ", array [";
                    for (size_t i = 0; i < size; ++i) {
                        std::cerr << reference_array[i];
                        if (i + 1 == size) {
                            std::cerr << "]" << std::endl;
                        } else {
                            std::cerr << ", ";
                        }
                    }
                    std::exit(1);
                }
            }
        } else {
            for (size_t v = 0; v < size; ++v) {
                if (!(used_mask & (1 << v))) {
                    reference_array[depth] = v;
                    go(depth + 1, used_mask | (1 << v));
                }
            }
        }
    }

    ~wrapper() {
        delete reference_array;
        delete working_array;
    }
};

void test_all_perms(kth_statistic<int> *algorithm, size_t size) {
    test_common(algorithm, size, "test_all_perms", 10);
    wrapper w(algorithm, size);
    w.go(0, 0);
}
