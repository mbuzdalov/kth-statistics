#pragma once

#include "kth_statistic.h"

void test_common(kth_statistic<int> *algorithm, size_t size, char const *test_name, size_t max_size);
void test_all_01s(kth_statistic<int> *algorithm, size_t size);
void test_all_perms(kth_statistic<int> *algorithm, size_t size);
