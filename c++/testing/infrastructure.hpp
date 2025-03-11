#ifndef __KTH_STATISTIC__TESTING__INFRASTRUCTURE_HPP__
#define __KTH_STATISTIC__TESTING__INFRASTRUCTURE_HPP__

#include <sstream>
#include <stdexcept>

#define FAIL_IF_NOT_EQUAL(expected, found) { \
    auto expected_val = (expected); \
    auto found_val = (found); \
    if (expected != found) { \
        std::ostringstream oss; \
        oss << "Test " << __func__ << " failed: '" << #found << "' was expected to be '" << expected_val << "' but was '" << found_val << "'"; \
        throw std::logic_error(oss.str()); \
    } \
}

#define FAIL_IF_FALSE(found) { \
    if (!(found)) { \
        std::ostringstream oss; \
        oss << "Test " << __func__ << " failed: condition '" << #found << "' was not satisfied"; \
        throw std::logic_error(oss.str()); \
    } \
}

#endif // __KTH_STATISTIC__TESTING__INFRASTRUCTURE_HPP__
