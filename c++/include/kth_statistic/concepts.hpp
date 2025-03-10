#ifndef __KTH_STATISTIC__CONCEPTS_HPP__
#define __KTH_STATISTIC__CONCEPTS_HPP__

#include <concepts>
#include <iterator>
#include <string>

template<
    template<typename, typename, typename> typename A,
    typename T, typename C, typename I
> concept kth_statistic_algorithm
     = std::regular<T>
    && std::strict_weak_order<C, T, T>
    && std::random_access_iterator<I>
    && std::same_as<T, typename std::iterator_traits<I>::value_type>
    && requires(A<T, C, I> a, size_t size, size_t k, I iter)
{
    { a.max_size() } -> std::same_as<size_t>;
    { a.resize(size) } -> std::same_as<void>;
    { a.name() } -> std::same_as<std::string>;
    { a(iter, size, k) } -> std::same_as<T>;
};

#endif // __KTH_STATISTIC__CONCEPTS_HPP__
