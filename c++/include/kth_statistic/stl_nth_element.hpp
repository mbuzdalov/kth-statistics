#ifndef __KTH_STATISTIC__STL_NTH_ELEMENT_HPP__
#define __KTH_STATISTIC__STL_NTH_ELEMENT_HPP__

#include <algorithm>
#include <limits>
#include <string>

template <typename T, typename C, typename I>
struct kth_statistic_algorithm_stl {
    size_t max_size() const {
        return std::numeric_limits<size_t>::max();
    }

    void resize(size_t ignored) {}

    std::string name() const {
        return "std::nth_element";
    }

    T operator()(I iterator, size_t size, size_t k) const {
        std::nth_element(iterator, iterator + k, iterator + size, C{});
        return iterator[k];
    }
};

#endif // __KTH_STATISTIC__STL_NTH_ELEMENT_HPP__
