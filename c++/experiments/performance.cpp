#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>

#include "kth_statistic.h"
#include "kth_statistic_stl.h"
#include "kth_statistic_hoare.h"
#include "kth_statistic_predictor_simple.h"
#include "util.h"

template<typename element_t>
struct sequence_changer {
    virtual void generate(element_t *array, size_t size) = 0;
    virtual ~sequence_changer() {}
};

template<typename element_t>
class performance_test {
    char const * const measurement_name;
    size_t const size, k, count;
    element_t **reference, **working;
    element_t *results;

public:
    performance_test(char const *measurement_name, size_t size, size_t k, size_t count,
                     std::vector< sequence_changer<element_t>* > const &seq_changers) :
                        measurement_name(measurement_name), size(size), k(k), count(count) {
        reference = new element_t*[count];
        working = new element_t*[count];
        results = new element_t[count];
        for (size_t i = 0; i < count; ++i) {
            reference[i] = new element_t[size];
            working[i] = new element_t[size];
            for (auto seq_changer : seq_changers) {
                seq_changer->generate(reference[i], size);
            }
        }
    }

    void test(std::vector< kth_statistic<element_t>* > algorithms) {
        std::cout << "Measurement '" << measurement_name
                  << "', size = " << size
                  << ", k = " << k
                  << ", count = " << count
                  << ":" << std::endl;

        bool hash_initialized = false;
        int expected_hash = 0;

        size_t algo_width = 0;
        for (auto algorithm : algorithms) {
            algo_width = std::max(algo_width, strlen(algorithm->name()));
        }

        for (kth_statistic<element_t> *algorithm : algorithms) {
            algorithm->resize(size);
            for (size_t i = 0; i < count; ++i) {
                array_copy(reference[i], size, working[i]);
            }

            const auto start = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < count; ++i) {
                results[i] = algorithm->find(working[i], size, k);
            }
            const auto finish = std::chrono::high_resolution_clock::now();

            int hash = 0;
            for (size_t i = 1; i < count; ++i) {
                hash = 31 * hash + int(results[0] != results[i]);
            }
            if (hash_initialized) {
                if (hash != expected_hash) {
                    std::cerr << "Error: hashes are different between " << algorithms[0]->name()
                              << " and " << algorithm->name() << std::endl;
                    std::exit(1);
                }
            } else {
                hash_initialized = true;
                expected_hash = hash;
            }

            const std::chrono::duration<double> elapsed_seconds(finish - start);
            const std::chrono::duration<double> normalized = elapsed_seconds / double(size) / double(count);

            std::cout << "    " << std::setw(algo_width) << algorithm->name()
                      << ": " << std::setprecision(4) << std::scientific << elapsed_seconds
                      << ", " << std::setprecision(4) << std::scientific << normalized
                      << " per element" << std::endl;
        }
    }

    ~performance_test() {
        for (size_t i = 0 ; i < count; ++i) {
            delete[] reference[i];
            delete[] working[i];
        }
        delete[] reference;
        delete[] working;
        delete[] results;
    }
};

template<typename element_t, typename rng_t>
struct uniform_int_generator : sequence_changer<element_t> {
    rng_t &rng;
    element_t min, max;

    uniform_int_generator(rng_t &rng, element_t min, element_t max): rng(rng), min(min), max(max) {}

    void generate(element_t *array, size_t size) {
        std::uniform_int_distribution<element_t> value_gen(min, max);
        for (size_t i = 0; i < size; ++i) {
            array[i] = value_gen(rng);
        }
    }
};

template<typename element_t, typename rng_t>
struct uniform_real_generator : sequence_changer<element_t> {
    rng_t &rng;
    element_t min, max;

    uniform_real_generator(rng_t &rng, element_t min, element_t max): rng(rng), min(min), max(max) {}

    void generate(element_t *array, size_t size) {
        std::uniform_real_distribution<element_t> value_gen(min, max);
        for (size_t i = 0; i < size; ++i) {
            array[i] = value_gen(rng);
        }
    }
};

template<typename element_t, typename comparator_t>
struct sorter : sequence_changer<element_t> {
    comparator_t cmp;

    void generate(element_t *array, size_t size) {
        std::sort(array, array + size, cmp);
    }
};

int main() {
    std::mt19937_64 rng(12314342342342LL);

    fixed_ratio_sample_sizes fss(10, 10);
    tuned_ratio_sample_sizes tss;

    stl_kth_statistic<int> stl_int;
    bidirectional_hoare_middle<int> hoare_mid_int;
    predicting_kth_statistic<int> predicting_int_fixed(fss, "simple predicting kth, fixed");
    predicting_kth_statistic<int> predicting_int_tuned(tss, "simple predicting kth, tuned");

    std::vector< kth_statistic<int>* > all_int { &stl_int, &hoare_mid_int,
                                                 &predicting_int_fixed, &predicting_int_tuned };

    stl_kth_statistic<double> stl_dbl;
    bidirectional_hoare_middle<double> hoare_mid_dbl;
    predicting_kth_statistic<double> predicting_dbl_fixed(fss, "simple predicting kth, fixed");
    predicting_kth_statistic<double> predicting_dbl_tuned(tss, "simple predicting kth, tuned");

    std::vector< kth_statistic<double>* > all_dbl { &stl_dbl, &hoare_mid_dbl,
                                                 &predicting_dbl_fixed, &predicting_dbl_tuned };

    uniform_int_generator<int, std::mt19937_64> gen_int_1(rng, -1000000000, +1000000000);
    uniform_real_generator<double, std::mt19937_64> gen_dbl_1(rng, -1.0, +1.0);
    sorter<int, std::less<int> > int_increasing_sorter;
    sorter<int, std::greater<int> > int_decreasing_sorter;
    sorter<double, std::less<double> > dbl_increasing_sorter;
    sorter<double, std::greater<double> > dbl_decreasing_sorter;

    std::vector< std::pair< char const *, std::vector< sequence_changer<int>* > > > int_tests = {
        { "UniformInt[-1e9, +1e9]", { &gen_int_1 } },
        { "UniformIntInc[-1e9, +1e9]", { &gen_int_1, &int_increasing_sorter } },
        { "UniformIntDec[-1e9, +1e9]", { &gen_int_1, &int_decreasing_sorter } }
    };

    std::vector< std::pair< char const *, std::vector< sequence_changer<double>* > > > dbl_tests = {
        { "UniformDouble[-1, +1]", { &gen_dbl_1 } },
        { "UniformDoubleInc[-1, +1]", { &gen_dbl_1, &dbl_increasing_sorter } },
        { "UniformDoubleDec[-1, +1]", { &gen_dbl_1, &dbl_decreasing_sorter } }
    };

    std::vector<size_t> divisors = { 2, 10 };
    for (size_t div : divisors) {
        std::cout << "********* Int, 1/" << div << " order stat **********\n" << std::endl;

        for (auto config : int_tests) {
            for (size_t i = 1, s = 10; i <= 7; ++i, s *= 10) {
                performance_test<int> test(config.first, s, s / div, 100000000 / s, config.second);
                test.test(all_int);
            }
            std::cout << std::endl;
        }

        std::cout << "********* Double, 1/" << div << " order stat **********\n" << std::endl;

        for (auto config : dbl_tests) {
            for (size_t i = 1, s = 10; i <= 7; ++i, s *= 10) {
                performance_test<double> test(config.first, s, s / div, 100000000 / s, config.second);
                test.test(all_dbl);
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
