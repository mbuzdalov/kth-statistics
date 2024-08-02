#include <cstdlib>
#include <iostream>

#include "kth_statistic.h"
#include "tests.h"

void test_all(kth_statistic<int> *algorithm) {
    for (size_t size = 1; size <= 16; ++size) {
        test_all_01s(algorithm, size);
    }
    std::cout << algorithm->name() << ": test_all_01s OK" << std::endl;

    for (size_t size = 1; size <= 9; ++size) {
        test_all_perms(algorithm, size);
    }
    std::cout << algorithm->name() << ": test_all_perms OK" << std::endl;
}

int main() {
    kth_statistic<int> *stl_int = new stl_kth_statistic<int>();
    test_all(stl_int);
    delete stl_int;
}
