#include <algorithm>
#include <chrono>
#include <iostream>
#include <tuple>
#include <vector>

#include <kth_statistic/all.hpp>

#include "generators.hpp"

template<typename T, template<typename, typename, typename> typename algorithm_t>
requires kth_statistic_algorithm<algorithm_t, T, std::less<T>, typename std::vector<T>::iterator>
void benchmark_one(std::vector<T> /*deliberately no reference*/working_copy, std::vector<T> const &answers, size_t block_size, size_t query) {
    algorithm_t<T, std::less<T>, typename std::vector<T>::iterator> algo;
    size_t num_blocks = answers.size();
    std::vector<T> results(num_blocks);

    const auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < num_blocks; ++i) {
        results[i] = algo(working_copy.begin() + (i * block_size), block_size, query);
    }
    const auto finish = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < num_blocks; ++i) {
        if (answers[i] != results[i]) {
            std::cerr << "  " << algo.name() << ": Error! For block " << i << " expected " << answers[i] << ", found " << results[i] << std::endl;
            std::exit(1);
        }
    }

    const std::chrono::duration<double> elapsed_seconds(finish - start);
    const std::chrono::duration<double> normalized = elapsed_seconds / double(working_copy.size());

    std::cout << std::setw(25) << algo.name()
              << ": " << std::setprecision(4) << std::scientific << elapsed_seconds
              << ", " << std::setprecision(4) << std::scientific << normalized
              << " per element" << std::endl;
}

template<typename T>
std::vector<T> generate_answers(std::vector<T> /*deliberately no reference*/working_copy, size_t block_size, size_t query) {
    size_t num_blocks = working_copy.size() / block_size;
    std::vector<T> answers(num_blocks);
    for (size_t i = 0; i < num_blocks; ++i) {
        size_t offset = i * block_size;
        std::nth_element(working_copy.begin() + offset,
                         working_copy.begin() + (offset + query),
                         working_copy.begin() + (offset + block_size));
        answers[i] = working_copy[offset + query];
    }
    return answers;
}

template<typename T, sequence_changer<T>... G>
void benchmark(std::string const &benchmark_name, size_t block_size, size_t num_blocks, size_t query, size_t seed) {
    std::cout << benchmark_name << " (block size " << block_size << ", query " << query << ")" << std::flush;

    // 0. Generate the dataset
    std::mt19937_64 rng(seed);
    std::vector<T> reference = generate_with_chain<T, G...>(rng, block_size, num_blocks);
    std::cout << ":" << std::flush;

    // 1. Generate answers
    std::vector<T> answers = generate_answers(reference, block_size, query);
    std::cout << std::endl;

    // 2. Perform the benchmarks
    benchmark_one< T, kth_statistic_algorithm_stl   >(reference, answers, block_size, query);
    benchmark_one< T, kth_statistic_algorithm_hoare >(reference, answers, block_size, query);
}

int main() {
    std::vector<std::tuple<size_t, size_t, size_t>> configurations {
        std::make_tuple(10, 10000000, 5),
        std::make_tuple(100, 1000000, 50),
        std::make_tuple(1000, 100000, 500),
        std::make_tuple(10000, 10000, 5000),
        std::make_tuple(100000, 1000, 50000),
        std::make_tuple(1000000, 100, 500000),

        std::make_tuple(10, 10000000, 1),
        std::make_tuple(100, 1000000, 10),
        std::make_tuple(1000, 100000, 100),
        std::make_tuple(10000, 10000, 1000),
        std::make_tuple(100000, 1000, 10000),
        std::make_tuple(1000000, 100, 100000),

        std::make_tuple(10, 10000000, 3),
        std::make_tuple(100, 1000000, 3),
        std::make_tuple(1000, 100000, 3),
        std::make_tuple(10000, 10000, 3),
        std::make_tuple(100000, 1000, 3),
        std::make_tuple(1000000, 100, 3),
    };

    size_t common_seed = 12314342342342LL;

    for (auto conf : configurations) {
        benchmark<int,
            integer_generator<int, std::numeric_limits<int>::min(), std::numeric_limits<int>::max()>
        >("uniform_int[min, max]", std::get<0>(conf), std::get<1>(conf), std::get<2>(conf), ++common_seed);
    }

    for (auto conf : configurations) {
        benchmark<int,
            integer_generator<int, -10, 10>
        >("uniform_int[-10, +10]", std::get<0>(conf), std::get<1>(conf), std::get<2>(conf), ++common_seed);
    }

    for (auto conf : configurations) {
        benchmark<int,
            integer_generator<int, -10, 10>,
            sorter<int, std::less<int>>
        >("uniform_int[-10, +10] increasing", std::get<0>(conf), std::get<1>(conf), std::get<2>(conf), ++common_seed);
    }

    for (auto conf : configurations) {
        benchmark<int,
            integer_generator<int, -10, 10>,
            sorter<int, std::greater<int>>
        >("uniform_int[-10, +10] decreasing", std::get<0>(conf), std::get<1>(conf), std::get<2>(conf), ++common_seed);
    }

    for (auto conf : configurations) {
        benchmark<double,
            float_generator<double, -1.0, 1.0>
        >("uniform_double[-1.0, +1.0]", std::get<0>(conf), std::get<1>(conf), std::get<2>(conf), ++common_seed);
    }

    return 0;
}
