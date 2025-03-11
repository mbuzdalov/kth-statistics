#ifndef __KTH_STATISTIC__TESTING__TEST_RANDOM_HPP__
#define __KTH_STATISTIC__TESTING__TEST_RANDOM_HPP__

#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>

#include "infrastructure.hpp"
#include "safe_iterator.hpp"

template <
    template<typename, typename, typename> typename algorithm_t
> requires kth_statistic_algorithm <
    algorithm_t, int, std::less<int>, safe_iterator<int>
> void test_random_common(
    std::string const &config_name, size_t size, size_t count, size_t seed,
    int value_min = std::numeric_limits<int>::min(),
    int value_max = std::numeric_limits<int>::max()
) {
    algorithm_t<int, std::less<int>, safe_iterator<int>> algo;
    algo.resize(size);
    FAIL_IF_FALSE(algo.max_size() >= size);

    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<size_t> pos_gen(0, size - 1);
    std::uniform_int_distribution<int> val_gen(value_min, value_max);

    std::vector<int> reference(size), working(size);

    for (size_t attempt = 0; attempt < count; ++attempt) {
        size_t k = pos_gen(rng);
        for (size_t i = 0; i < size; ++i) {
            reference[i] = val_gen(rng);
            working[i] = reference[i];
        }
        std::nth_element(working.begin(), working.begin() + k, working.end());
        int expected = working[k];
        working = reference;
        FAIL_IF_NOT_EQUAL(expected, algo(safe_iterator(working), size, k));
    }

    std::cout << "OK: algorithm '" << algo.name() << "', " << config_name << " of size " << size << std::endl;
}

template <
    template<typename, typename, typename> typename algorithm_t
> requires kth_statistic_algorithm <
    algorithm_t, int, std::less<int>, safe_iterator<int>
> void test_random_repeated(size_t size, size_t count, size_t seed) {
    test_random_common<algorithm_t>("random tests with size/10 value range", size, count, seed, 0, size / 10);
}

template <
    template<typename, typename, typename> typename algorithm_t
> requires kth_statistic_algorithm <
    algorithm_t, int, std::less<int>, safe_iterator<int>
> void test_random(size_t size, size_t count, size_t seed) {
    test_random_common<algorithm_t>("random tests with all-int value range", size, count, seed);
}

#endif // __KTH_STATISTIC__TESTING__TEST_RANDOM_HPP__
