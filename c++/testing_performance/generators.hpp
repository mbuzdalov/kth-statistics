#ifndef __KTH_STATISTIC__PERFORMANCE__GENERATORS_HPP__
#define __KTH_STATISTIC__PERFORMANCE__GENERATORS_HPP__

#include <algorithm>
#include <concepts>
#include <random>
#include <vector>

template<typename C, typename T>
concept sequence_changer = requires(C c, std::vector<T> &v, std::mt19937_64 &rng) {
    { c(rng, v.begin(), v.size()) } -> std::same_as<void>;
};

template<std::integral T, T min_value, T max_value>
struct integer_generator {
    void operator() (std::mt19937_64 &rng, std::vector<T>::iterator begin, size_t size) {
        std::uniform_int_distribution<T> value_gen(min_value, max_value);
        for (size_t i = 0; i < size; ++i) {
            *begin = value_gen(rng);
            ++begin;
        }
    }
};

template<std::floating_point T, T min_value, T max_value>
struct float_generator {
    void operator() (std::mt19937_64 &rng, std::vector<T>::iterator begin, size_t size) {
        std::uniform_real_distribution<T> value_gen(min_value, max_value);
        for (size_t i = 0; i < size; ++i) {
            *begin = value_gen(rng);
            ++begin;
        }
    }
};

template<typename T, std::strict_weak_order<T, T> C>
struct sorter {
    void operator() (std::mt19937_64 &rng, std::vector<T>::iterator begin, size_t size) {
        std::sort(begin, begin + size, C{});
    }
};

namespace _detail_ {
    template<typename T>
    void patch_with_chain(std::mt19937_64 &rng, size_t block_size, std::vector<T> &vector) {}

    template<typename T, sequence_changer<T> G1, sequence_changer<T>... GX>
    void patch_with_chain(std::mt19937_64 &rng, size_t block_size, std::vector<T> &vector) {
        G1 g1;
        for (size_t i = 0; i < vector.size(); i += block_size) {
            g1(rng, vector.begin() + i, block_size);
        }
        patch_with_chain<T, GX...>(rng, block_size, vector);
    }
}

template<typename T, sequence_changer<T>... G>
std::vector<T> generate_with_chain(std::mt19937_64 &rng, size_t block_size, size_t num_blocks) {
    std::vector<T> result(block_size * num_blocks);
    _detail_::patch_with_chain<T, G...>(rng, block_size, result);
    return result;
}

#endif // __KTH_STATISTIC__PERFORMANCE__GENERATORS_HPP__
