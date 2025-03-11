#ifndef __KTH_STATISTIC__TESTING__TEST_ALL_01S_HPP__
#define __KTH_STATISTIC__TESTING__TEST_ALL_01S_HPP__

#include <iostream>
#include "infrastructure.hpp"
#include "safe_iterator.hpp"

template <
    template<typename, typename, typename> typename algorithm_t
> requires kth_statistic_algorithm <
    algorithm_t, int, std::less<int>, safe_iterator<int>
> void test_all_01s_exact_size(size_t size) {
    algorithm_t<int, std::less<int>, safe_iterator<int>> algo;
    algo.resize(size);
    FAIL_IF_FALSE(algo.max_size() >= size);

    std::vector<int> values(size);
    size_t mask_max = size_t(1) << size;
    for (size_t mask = 0; mask < mask_max; ++mask) {
        for (size_t k = 0; k < size; ++k) {
            size_t count_1 = 0;
            for (size_t i = 0; i < size; ++i) {
                values[i] = int((mask >> i) & 1);
                count_1 += values[i];
            }
            int expected = k + count_1 >= size ? 1 : 0;
            FAIL_IF_NOT_EQUAL(expected, algo(safe_iterator(values), size, k));
        }
    }

    std::cout << "OK: algorithm '" << algo.name() << "', all tests of size " << size << " made of 0s and 1s" << std::endl;
}

template <
    template<typename, typename, typename> typename algorithm_t
> requires kth_statistic_algorithm <
    algorithm_t, int, std::greater<int>, safe_iterator<int>
> void test_all_01s_exact_size_greater(size_t size) {
    algorithm_t<int, std::greater<int>, safe_iterator<int>> algo;
    algo.resize(size);
    FAIL_IF_FALSE(algo.max_size() >= size);

    std::vector<int> values(size);
    size_t mask_max = size_t(1) << size;
    for (size_t mask = 0; mask < mask_max; ++mask) {
        for (size_t k = 0; k < size; ++k) {
            size_t count_1 = 0;
            for (size_t i = 0; i < size; ++i) {
                values[i] = int((mask >> i) & 1);
                count_1 += values[i];
            }
            int expected = k < count_1 ? 1 : 0;
            FAIL_IF_NOT_EQUAL(expected, algo(safe_iterator(values), size, k));
        }
    }

    std::cout << "OK: algorithm '" << algo.name() << "', all tests of size " << size << " made of 0s and 1s, reverse comparator" << std::endl;
}

#endif // __KTH_STATISTIC__TESTING__TEST_ALL_01S_HPP__
