#ifndef __KTH_STATISTIC__TESTING__SAFE_ITERATOR_HPP__
#define __KTH_STATISTIC__TESTING__SAFE_ITERATOR_HPP__

#include <iterator>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>

template<typename T>
class safe_iterator {
public:
    using self_type = safe_iterator<T>;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::random_access_iterator_tag;

private:
    static constexpr ptrdiff_t POS_INF = std::numeric_limits<ptrdiff_t>::max();
    static constexpr ptrdiff_t NEG_INF = std::numeric_limits<ptrdiff_t>::min();

    std::vector<T> *base;
    ptrdiff_t index;

    void compare_precheck(self_type const &that) const {
        if (base != that.base) {
            throw std::invalid_argument("Attempt to compare iterators from different vectors");
        }
    }

    size_t index_precheck(size_t offset) const {
        if (index == POS_INF) {
            throw std::out_of_range("Attempt to dereference a safe_iterator which is in the positive saturated state");
        }
        if (index == NEG_INF) {
            throw std::out_of_range("Attempt to dereference a safe_iterator which is in the negative saturated state");
        }
        if (index >= 0) {
            size_t ii = size_t(index);
            if (ii < std::numeric_limits<size_t>::max() - offset && ii + offset < base->size()) {
                return ii + offset;
            }
        } else {
            size_t ii = size_t(-index);
            if (ii <= offset && offset - ii < base->size()) {
                return offset - ii;
            }
        }
        std::ostringstream oss;
        oss << "Attempt to derefence a safe_iterator with an invalid index: index = " << index << ", offset = " << offset << ", size " << base->size();
        throw std::out_of_range(oss.str());
    }

    std::vector<T> &precheck_base() const {
        if (base == NULL) {
            throw std::logic_error("Attempt to dereference a safe_iterator pointing to nowhere");
        } else {
            return *base;
        }
    }

    self_type &move_pos(ptrdiff_t delta) {
        if (index < POS_INF - delta) {
            index += delta;
        } else {
            index = POS_INF;
        }
        return *this;
    }

    self_type &move_neg(ptrdiff_t delta) {
        if (index > NEG_INF + delta) {
            index -= delta;
        } else {
            index = NEG_INF;
        }
        return *this;
    }

public:

    safe_iterator() noexcept: base(NULL), index(0) {}
    safe_iterator(std::vector<T> &base, ptrdiff_t index = 0) noexcept: base(&base), index(index) {}

    safe_iterator(self_type const &that) noexcept = default;
    safe_iterator(self_type      &&that) noexcept = default;

    self_type &operator = (self_type const &that) noexcept = default;
    self_type &operator = (self_type      &&that) noexcept = default;

    ~safe_iterator() noexcept = default;

    bool operator == (self_type const &that) const {
        compare_precheck(that);
        return index == that.index;
    }

    bool operator != (self_type const &that) const {
        compare_precheck(that);
        return index != that.index;
    }

    bool operator < (self_type const &that) const {
        compare_precheck(that);
        return index < that.index;
    }

    bool operator <= (self_type const &that) const {
        compare_precheck(that);
        return index <= that.index;
    }

    bool operator > (self_type const &that) const {
        compare_precheck(that);
        return index > that.index;
    }

    bool operator >= (self_type const &that) const {
        compare_precheck(that);
        return index >= that.index;
    }

    ptrdiff_t operator - (self_type const &that) const {
        if (base != that.base) {
            throw std::invalid_argument("Attempt to subtract iterators from different vectors");
        }
        return ptrdiff_t(index) - ptrdiff_t(that.index);
    }

    self_type &operator ++() {
        if (index != NEG_INF) {
            ++index;
        }
        return *this;
    }

    self_type operator ++(int) {
        self_type result { *this };
        ++(*this);
        return result;
    }

    self_type &operator --() {
        if (index != POS_INF) {
            --index;
        }
        return *this;
    }

    self_type operator --(int) {
        T result { *this };
        --(*this);
        return result;
    }

    self_type &operator += (ptrdiff_t delta) {
        return delta >= 0 ? move_pos(delta) : move_neg(-delta);
    }

    self_type operator + (ptrdiff_t delta) const {
        return self_type { *this } += delta;
    }

    self_type &operator -= (ptrdiff_t delta) {
        return delta >= 0 ? move_neg(delta) : move_pos(-delta);
    }

    self_type operator - (ptrdiff_t delta) const {
        return self_type { *this } -= delta;
    }

    friend self_type operator + (ptrdiff_t delta, self_type const &ptr) {
        return ptr + delta;
    }

    T &operator *() const {
        return precheck_base()[index_precheck(0)];
    }

    T &operator [] (size_t offset) const {
        return precheck_base()[index_precheck(offset)];
    }
};

#endif // __KTH_STATISTIC__TESTING__SAFE_ITERATOR_HPP__
