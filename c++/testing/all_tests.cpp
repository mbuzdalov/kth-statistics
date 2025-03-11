#include <iostream>
#include <vector>

#include <kth_statistic/all.hpp>

#include "test_all_01s.hpp"
#include "test_all_permutations.hpp"
#include "test_random.hpp"

template <
    template<typename, typename, typename> typename algorithm_t
> void run_all_tests() {
    for (size_t size = 1; size <= 16; ++size) {
        test_all_01s_exact_size<algorithm_t>(size);
        test_all_01s_exact_size_greater<algorithm_t>(size);
    }
    for (size_t size = 1; size <= 9; ++size) {
        test_all_permutations<algorithm_t>(size);
    }

    size_t rnd_sizes[] = { 10, 100, 1000, 10000, 100000, 1000000 };

    for (size_t idx = 0; idx < 6; ++idx) {
        size_t size = rnd_sizes[idx];
        size_t count = 10000000 / size;
        size_t seed = 87512451357632 * (idx + 1);
        test_random<algorithm_t>(size, count, seed);
    }

    for (size_t idx = 0; idx < 6; ++idx) {
        size_t size = rnd_sizes[idx];
        size_t count = 10000000 / size;
        size_t seed = 87512451357631 * (idx + 1);
        test_random_repeated<algorithm_t>(size, count, seed);
    }

    for (size_t idx = 0; idx < 6; ++idx) {
        size_t size = rnd_sizes[idx];
        size_t count = 10000000 / size;
        size_t seed = 87512451357630 * (idx + 1);
        test_random_double<algorithm_t>(size, count, seed);
    }
}

int main() {
    run_all_tests<kth_statistic_algorithm_stl>();
    return 0;
}
