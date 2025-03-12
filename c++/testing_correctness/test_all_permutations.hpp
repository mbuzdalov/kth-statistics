#ifndef __KTH_STATISTIC__TESTING__TEST_ALL_PERMUTATIONS_HPP__
#define __KTH_STATISTIC__TESTING__TEST_ALL_PERMUTATIONS_HPP__

#include <algorithm>
#include <iostream>
#include <numeric>

#include "infrastructure.hpp"
#include "safe_iterator.hpp"

template <
    template<typename, typename, typename> typename algorithm_t
> requires kth_statistic_algorithm <
    algorithm_t, unsigned, std::less<unsigned>, safe_iterator<unsigned>
> void test_all_permutations(size_t size) {
    algorithm_t<unsigned, std::less<unsigned>, safe_iterator<unsigned>> algo;
    algo.resize(size);
    FAIL_IF_FALSE(algo.max_size() >= size);

    std::vector<unsigned> base(size);
    std::iota(base.begin(), base.end(), 0);

    do {
        for (unsigned k = 0; k < size; ++k) {
            std::vector<unsigned> working = base;
            FAIL_IF_NOT_EQUAL(k, algo(safe_iterator(working), size, k));
        }
    } while (std::next_permutation(base.begin(), base.end()));

    std::cout << "OK: algorithm '" << algo.name() << "', all permutatons of size " << size << std::endl;
}

#endif // __KTH_STATISTIC__TESTING__TEST_ALL_PERMUTATIONS_HPP__
