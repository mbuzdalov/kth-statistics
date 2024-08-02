#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

#include "kth_statistic.h"
#include "kth_statistic_stl.h"
#include "kth_statistic_hoare.h"
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
                     std::vector< sequence_changer<element_t>* > seq_changers) :
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

            std::cout << "    " << algorithm->name() << ": " << elapsed_seconds << ", " << normalized << " per element" << std::endl;
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

int main() {
    std::mt19937_64 rng(12314342342342LL);

    stl_kth_statistic<int> stl_int;
    bidirectional_hoare_middle<int> hoare_mid_int;

    uniform_int_generator<int, std::mt19937_64> gen_1(rng, -1000000000, +1000000000);
    for (size_t i = 1, s = 10; i <= 7; ++i, s *= 10) {
        performance_test<int> test_1("UniformInt[-1e9, +1e9]", s, s / 2, 100000000 / s, { &gen_1 });
        test_1.test({ &stl_int, &hoare_mid_int });
    }

    return 0;
}
