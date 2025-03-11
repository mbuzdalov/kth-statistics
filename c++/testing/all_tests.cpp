#include <iostream>
#include <vector>

#include <kth_statistic/all.hpp>

#include "test_all_01s.hpp"
#include "test_all_permutations.hpp"

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
}

int main() {
    run_all_tests<kth_statistic_algorithm_stl>();
    return 0;
}
