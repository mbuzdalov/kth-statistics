#pragma once

#include "kth_statistic.h"

void test_common(kth_statistic<int> *algorithm, size_t size, char const *test_name, size_t max_size);
void test_all_01s(kth_statistic<int> *algorithm, size_t size);
void test_all_perms(kth_statistic<int> *algorithm, size_t size);
void test_random(kth_statistic<int> *algorithm, size_t size, size_t count, size_t seed);
void test_random_repeated(kth_statistic<int> *algorithm, size_t size, size_t count, size_t seed);
