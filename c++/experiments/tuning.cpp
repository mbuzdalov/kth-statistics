#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <map>

#include "kth_statistic.h"
#include "kth_statistic_stl.h"
#include "kth_statistic_hoare.h"
#include "kth_statistic_predictor_simple.h"
#include "util.h"

struct variable_ratio_sample_sizes : sample_sizes {
    bool is_size_acceptable(size_t n) {
        return n >= 2 * _phase_1_divisor;
    }
    size_t n_phase_1_samples(size_t n) {
        return n / _phase_1_divisor;
    }
    size_t n_phase_2_samples(size_t n, size_t phase_1) {
        return std::min(phase_1, 1 + phase_1 / _phase_2_divisor);
    }

    size_t _phase_1_divisor, _phase_2_divisor;
};

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

    void test(kth_statistic<element_t> &algorithm, variable_ratio_sample_sizes &sizes, size_t max_divisor_prod) {
        bool hash_initialized = false;
        int expected_hash = 0;

        std::map< std::pair<size_t, size_t>, double > cache;

        algorithm.resize(size);
        for (size_t div_prod = 1; div_prod <= max_divisor_prod; ++div_prod) {
            for (size_t div_1 = 2; div_1 <= div_prod; ++div_1) {
                sizes._phase_1_divisor = div_1;
                sizes._phase_2_divisor = div_prod / div_1;

                size_t p1 = sizes.n_phase_1_samples(size);
                size_t p2 = sizes.n_phase_2_samples(size, p1);
                auto p12 = std::make_pair(p1, p2);
                if (cache.find(p12) != cache.end()) {
                    double s = cache[p12];
                    std::cout << div_prod << "," << div_1 << "," << std::setprecision(4) << std::scientific << s << ",dup" << std::endl;
                    continue;
                }

                for (size_t i = 0; i < count; ++i) {
                    array_copy(reference[i], size, working[i]);
                }

                const auto start = std::chrono::high_resolution_clock::now();
                for (size_t i = 0; i < count; ++i) {
                    results[i] = algorithm.find(working[i], size, k);
                }
                const auto finish = std::chrono::high_resolution_clock::now();

                int hash = 0;
                for (size_t i = 1; i < count; ++i) {
                    hash = 31 * hash + int(results[0] - results[i]);
                }
                if (hash_initialized) {
                    if (hash != expected_hash) {
                        std::cerr << "Hash check fails" << std::endl;
                        std::exit(1);
                    }
                } else {
                    hash_initialized = true;
                    expected_hash = hash;
                }

                const std::chrono::duration<double> elapsed_seconds(finish - start);
                const std::chrono::duration<double> normalized = elapsed_seconds / double(size) / double(count);

                double s = normalized.count();
                cache[p12] = s;
                std::cout << div_prod << "," << div_1 << "," << std::setprecision(4) << std::scientific << s << ",orig" << std::endl;
            }
            std::cout << std::endl;
        }
    }

    ~performance_test() {
        for (size_t i = 0; i < count; ++i) {
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

template<typename element_t, typename comparator_t>
struct sorter : sequence_changer<element_t> {
    comparator_t cmp;

    void generate(element_t *array, size_t size) {
        std::sort(array, array + size, cmp);
    }
};

int main(int argc, char *argv[]) {
    std::mt19937_64 rng(12314342342342LL);

    variable_ratio_sample_sizes vss;
    predicting_kth_statistic<int> predicting_int(vss, "_");

    uniform_int_generator<int, std::mt19937_64> gen_1(rng, -1000000000, +1000000000);
    sorter<int, std::less<int> > int_increasing_sorter;
    sorter<int, std::greater<int> > int_decreasing_sorter;

    std::vector< std::pair< char const *, std::vector< sequence_changer<int>* > > > tests = {
        { "UniformInt[-1e9, +1e9]", { &gen_1 } },
        { "UniformIntInc[-1e9, +1e9]", { &gen_1, &int_increasing_sorter } },
        { "UniformIntDec[-1e9, +1e9]", { &gen_1, &int_decreasing_sorter } }
    };

    size_t size, test_no;

    if (argc != 3 || (size = atoi(argv[1])) == 0 || (test_no = atoi(argv[2])) >= tests.size()) {
        std::cerr << "Usage: " << argv[0] << " <size> <generator number>" << std::endl;
        std::cerr << "   where <generator number> is one of" << std::endl;
        for (size_t i = 0; i < tests.size(); ++i) {
            std::cerr << "    " << i << ": " << tests[i].first << std::endl;
        }
        std::exit(1);
    }

    std::vector<size_t> divisors = { 2 };
    for (size_t div : divisors) {
        auto config = tests[test_no];
        performance_test<int> test(config.first, size, size / div, 100000000 / size, config.second);
        test.test(predicting_int, vss, std::min(size_t(3000), size / 2));
    }

    return 0;
}
