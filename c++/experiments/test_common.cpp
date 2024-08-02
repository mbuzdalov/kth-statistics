#include "kth_statistic.h"
#include "tests.h"
#include <iostream>

void test_common(kth_statistic<int> *algorithm, size_t size, char const *test_name, size_t max_size) {
    if (algorithm == nullptr) {
        std::cerr << "[" << test_name << "] Error: algorithm is null"
                  << std::endl;
        std::exit(1);
    }
    if (size > max_size) {
        std::cerr << "[" << test_name << ", " << algorithm->name()
                  << "] Error: size is too large (" << size
                  << ", at most " << max_size << " is supported)"
                  << std::endl;
        std::exit(1);
    }

    algorithm->resize(size);
    if (algorithm->size() < size) {
        std::cerr << "[" << test_name << ", " << algorithm->name()
                  << "] After resize, the size is still too small ("
                  << algorithm->size() << " while expecting at least " << size << ")"
                  << std::endl;
        std::exit(1);
    }
}
