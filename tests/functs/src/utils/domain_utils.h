#pragma once
#include "clove-unit.h"

__clove_suite_t create_suite(const char* name, size_t test_count);
__clove_test_t create_test(const char* name);
__clove_test_t create_test_fail(const char* name);
__clove_test_t create_test_skip(const char* name);
void suite_add_test(__clove_suite_t* suite, __clove_test_t* test); 