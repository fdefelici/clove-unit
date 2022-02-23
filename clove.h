/*
 * clove-unit
 * v2.1.3
 * Unit Testing library for C
 * https://github.com/fdefelici/clove-unit
 *
 */

#ifndef CLOVE_H
#define CLOVE_H

#ifdef __linux
#define _GNU_SOURCE
#endif 

#pragma region PRIVATE APIs
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#pragma region PRIVATE APIs - Stack

 //Stack not generalized. By now just managing size_t items for implenting Iterative QuickSort 
typedef struct __clove_stack_t {
    unsigned char* items;
    size_t capacity;
    size_t count;
    size_t item_size;
} __clove_stack_t;

static void __clove_stack_init(__clove_stack_t* stack, size_t initial_capacity) {
    stack->capacity = initial_capacity;
    stack->count = 0;
    stack->item_size = sizeof(size_t);
    stack->items = (unsigned char*)malloc(stack->item_size * stack->capacity);
}

static bool __clove_stack_is_empty(__clove_stack_t* stack) {
    return stack->count == 0;
}

static void __clove_stack_push(__clove_stack_t* stack, size_t item) {
    if (stack->count == stack->capacity) {
        stack->capacity *= 2;
        stack->items = (unsigned char*)realloc(stack->items, stack->item_size * stack->capacity);
    }
    size_t byte_index = stack->count * stack->item_size;
    size_t* item_ptr = (size_t*)&(stack->items[byte_index]);
    *item_ptr = item;
    stack->count++;
}

static size_t __clove_stack_pop(__clove_stack_t* stack) {
    if (stack->count == 0) return -1; //shouldn't happen

    size_t byte_index = (stack->count - 1) * stack->item_size;
    size_t* item_ptr = (size_t*)&(stack->items[byte_index]);
    stack->count--;
    return *item_ptr;
}

static void __clove_stack_free(__clove_stack_t* stack) {
    if (!stack) return;
    free(stack->items);
    stack->items = NULL;
    stack->capacity = 0;
    stack->count = 0;
    stack->item_size = 0;
}

#pragma endregion

#pragma region PRIVATE APIs - Vector
typedef struct __clove_vector_params_t {
    size_t item_size;
    size_t initial_capacity;
    void (*item_ctor)(void*);
    void (*item_dtor)(void*);
} __clove_vector_params_t;


typedef struct __clove_vector_t {
    unsigned char* items;
    size_t capacity;
    size_t count;
    size_t item_size;
    void (*item_ctor)(void*);
    void (*item_dtor)(void*);
    void* swap_temp;
} __clove_vector_t;

#define __CLOVE_VECTOR_DEFAULT_PARAMS { .item_size = 0, .initial_capacity = 10, .item_ctor = NULL, .item_dtor = NULL }

static void __clove_vector_init(__clove_vector_t* vector, __clove_vector_params_t* params) {
    vector->capacity = params->initial_capacity;
    vector->count = 0;
    vector->item_size = params->item_size;
    vector->items = (unsigned char*)malloc(vector->item_size * vector->capacity);
    vector->item_ctor = params->item_ctor;
    vector->item_dtor = params->item_dtor;
    vector->swap_temp = malloc(vector->item_size);
}

static size_t __clove_vector_count(__clove_vector_t* vector) {
    return vector->count;
}

static void* __clove_vector_add_empty(__clove_vector_t* vector) {
    if (vector->count == vector->capacity) {
        vector->capacity *= 2;
        vector->items = (unsigned char*)realloc(vector->items, vector->item_size * vector->capacity);
    }
    size_t byte_index = vector->count * vector->item_size;
    vector->count++;
    void* item = (void*)&(vector->items[byte_index]);
    if (vector->item_ctor) vector->item_ctor(item);
    return item;
}

static void* __clove_vector_get(__clove_vector_t* vector, size_t index) {
    if (index < 0) return NULL;
    if (index >= vector->count) return NULL;
    size_t byte_index = index * vector->item_size;
    return (void*)&(vector->items[byte_index]);
}

static void __clove_vector_set(__clove_vector_t* vector, size_t index, void* item) {
    void* found = __clove_vector_get(vector, index);
    if (!found) return;
    memcpy(found, item, vector->item_size);
}

static void __clove_vector_free(__clove_vector_t* vector) {
    if (vector->item_dtor) {
        for (int i = 0; i < vector->count; ++i) {
            void* item = __clove_vector_get(vector, i);
            vector->item_dtor(item);
        }
    }
    free(vector->items);
    free(vector->swap_temp);
    vector->capacity = 0;
    vector->count = 0;
}

static void __clove_vector_swap(__clove_vector_t* vector, size_t index1, size_t index2) {
    void* curr = __clove_vector_get(vector, index1);
    void* next = __clove_vector_get(vector, index2);
    if (!curr || !next) return;
    memcpy(vector->swap_temp, curr, vector->item_size);
    __clove_vector_set(vector, index1, next);
    __clove_vector_set(vector, index2, vector->swap_temp);
}

//QuickSort
static size_t __clove_vector_quicksort_partition(__clove_vector_t* vector, int (*comparator)(void*, void*), size_t start_index, size_t end_index) {
    size_t pivot_index = start_index;
    size_t left_index = start_index;
    size_t right_index = end_index;

    void* item = NULL;
    void* pivot = NULL;
    while (left_index < right_index) {
        //Moving pivot to right
        bool item_is_gte = true;
        while (item_is_gte && pivot_index < right_index) {
            item = __clove_vector_get(vector, right_index);
            pivot = __clove_vector_get(vector, pivot_index);
            item_is_gte = (comparator(item, pivot) >= 0);
            if (item_is_gte) right_index--;
        }

        if (pivot_index != right_index) {
            __clove_vector_swap(vector, pivot_index, right_index);
            pivot_index = right_index;
        }

        if (left_index == right_index) break;

        //Moving pivot to left
        bool item_is_lte = true;
        while (item_is_lte && pivot_index > left_index) {
            item = __clove_vector_get(vector, left_index);
            pivot = __clove_vector_get(vector, pivot_index);
            item_is_lte = (comparator(item, pivot) <= 0);
            if (item_is_lte) left_index++;
        }

        if (pivot_index != left_index) {
            __clove_vector_swap(vector, pivot_index, left_index);
            pivot_index = left_index;
        }
    }
    return pivot_index;
}

static void __clove_vector_quicksort_iterative(__clove_vector_t* vector, int (*comparator)(void*, void*), size_t start_index, size_t end_index) {
    __clove_stack_t index_pairs_stack;
    __clove_stack_init(&index_pairs_stack, vector->count);

    __clove_stack_push(&index_pairs_stack, start_index);
    __clove_stack_push(&index_pairs_stack, end_index);

    while (!__clove_stack_is_empty(&index_pairs_stack)) {
        end_index = __clove_stack_pop(&index_pairs_stack);
        start_index = __clove_stack_pop(&index_pairs_stack);

        if (start_index >= end_index) continue;

        //find pivot and put it in right position
        size_t pivot_index = __clove_vector_quicksort_partition(vector, comparator, start_index, end_index);

        //left array indexes
        if (pivot_index != 0) { //protect size_t overflow (for instance this happen for already sorted items)
            __clove_stack_push(&index_pairs_stack, start_index);
            __clove_stack_push(&index_pairs_stack, pivot_index - 1);
        }

        //right array indexes
        if (pivot_index != SIZE_MAX) { //protect size_t overflow (for symmetry)
            __clove_stack_push(&index_pairs_stack, pivot_index + 1);
            __clove_stack_push(&index_pairs_stack, end_index);
        }
    }
    __clove_stack_free(&index_pairs_stack);
}

static void __clove_vector_sort(__clove_vector_t* vector, int (*comparator)(void*, void*)) {
    if (vector->count <= 1) return;
    __clove_vector_quicksort_iterative(vector, comparator, 0, vector->count - 1);
}
#pragma endregion //Vector


#define __CLOVE_STRING_LENGTH 256
#define __CLOVE_TEST_ENTRY_LENGTH 60

// wrapper for Microsoft API
#ifndef _WIN32
#define strtok_s(str, delimiters, context) strtok_r(str, delimiters, context)
#define strncpy_s(strDest, numberOfElements, strSource, count) strncpy(strDest, strSource, count) == NULL
#define strcpy_s(dest, dest_size, src) strcpy(dest, src) == NULL
#define _strdup strdup
#define sprintf_s(dest, dest_size, format, ...) sprintf(dest, format, __VA_ARGS__)
#endif


typedef struct __clove_test_t {
    //char name[__CLOVE_TEST_ENTRY_LENGTH];
    char* name;
    void (*funct)(struct __clove_test_t*);
    unsigned int result;
    char file_name[__CLOVE_STRING_LENGTH];
    unsigned int line;
    char err_msg[__CLOVE_STRING_LENGTH];
} __clove_test_t;

typedef struct __clove_suite {
    char* name;
    __clove_vector_t tests;
    int test_count;
    struct {
        void (*setup_once)();
        void (*teardown_once)();
        void (*setup)();
        void (*teardown)();
    } fixtures;
} __clove_suite_t;


static void __clove_empty_funct() { }

static void __clove_vector_test_ctor(void* test) {
    //cast to __clove_test_t* not needed
    memset(test, 0, sizeof(__clove_test_t));
}

static void __clove_vector_test_dtor(void* test_ptr) {
    __clove_test_t* test = (__clove_test_t*)test_ptr;
    free(test->name);
}

static void __clove_vector_suite_ctor(void* suite_ptr) {
    __clove_suite_t* suite = (__clove_suite_t*)suite_ptr;
    suite->name = NULL;
    suite->fixtures.setup_once = __clove_empty_funct;
    suite->fixtures.teardown_once = __clove_empty_funct;
    suite->fixtures.setup = __clove_empty_funct;
    suite->fixtures.teardown = __clove_empty_funct;

    __clove_vector_params_t params = __CLOVE_VECTOR_DEFAULT_PARAMS;
    params.item_size = sizeof(__clove_test_t);
    params.item_ctor = __clove_vector_test_ctor;
    params.item_dtor = __clove_vector_test_dtor;
    __clove_vector_init(&(suite->tests), &params);
    suite->test_count = 0;
}

static void __clove_vector_suite_dtor(void* suite_ptr) {
    __clove_suite_t* suite = (__clove_suite_t*)suite_ptr;
    free(suite->name);
    __clove_vector_free(&suite->tests);
}

static void __clove_vector_suite_ctor_manual(void* suite_ptr) {
    __clove_suite_t* suite = (__clove_suite_t*)suite_ptr;
    suite->name = NULL;
    suite->fixtures.setup_once = __clove_empty_funct;
    suite->fixtures.teardown_once = __clove_empty_funct;
    suite->fixtures.setup = __clove_empty_funct;
    suite->fixtures.teardown = __clove_empty_funct;

    /* Not needed when in Manual mode
    __clove_vector_params_t params = __CLOVE_VECTOR_DEFAULT_PARAMS;
    params.item_size = sizeof(__clove_test_t);
    params.item_ctor = __clove_vector_test_ctor;
    __clove_vector_init(&(suite->tests), &params);
    */
    suite->test_count = 0;
}
static void __clove_vector_suite_dtor_manual(void* suite_ptr) {
    //When manual, suite name comes from a static address, so dont have to be freed
    //free(suite->name);
    __clove_suite_t* suite = (__clove_suite_t*)suite_ptr;
    __clove_vector_free(&suite->tests);
}


#define __CLOVE_TEST_PASSED 1
#define __CLOVE_TEST_FAILED 2
#define __CLOVE_TEST_SKIPPED 3

#define __CLOVE_ASSERT_CHECK_EQUALITY 1
#define __CLOVE_ASSERT_CHECK_DIFFERENCE 2
#define __CLOVE_ASSERT_CHECK_TRUE 3
#define __CLOVE_ASSERT_CHECK_FALSE 4

#define __CLOVE_FLOATING_PRECISION 0.000001f

#define __CLOVE_INFO "[\x1b[1;34mINFO\x1b[0m]"
#define __CLOVE_WARN "[\x1b[33mWARN\x1b[0m]"
#define __CLOVE_ERRO "[\x1b[1;31mERRO\x1b[0m]"
#define __CLOVE_PASSED "[\x1b[1;32mPASS\x1b[0m]"
#define __CLOVE_SKIPPED "[\x1b[33mSKIP\x1b[0m]"
#define __CLOVE_FAILED "[\x1b[1;31mFAIL\x1b[0m]"


static void __clove_fail(char* msg, __clove_test_t* _this) {
    _this->result = __CLOVE_TEST_FAILED;
    strcpy_s(_this->err_msg, __CLOVE_STRING_LENGTH, msg);
}

static void __clove_pass(char* msg, __clove_test_t* _this) {
    _this->result = __CLOVE_TEST_PASSED;
    strcpy_s(_this->err_msg, __CLOVE_STRING_LENGTH, msg);
}

static void __clove_check_int(const unsigned int check_mode, int expected, int result, __clove_test_t* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%d] but was [%d]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_uint(const unsigned int check_mode, unsigned int expected, unsigned int result, __clove_test_t* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%u] but was [%u]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_long(const unsigned int check_mode, long expected, long result, __clove_test_t* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%ld] but was [%ld]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_ulong(const unsigned int check_mode, unsigned long expected, unsigned long result, __clove_test_t* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%lu] but was [%lu]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_llong(const unsigned int check_mode, long long expected, long long result, __clove_test_t* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%lld] but was [%lld]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_ullong(const unsigned int check_mode, unsigned long long expected, unsigned long long result, __clove_test_t* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%llu] but was [%llu]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_char(const unsigned int check_mode, char expected, char result, __clove_test_t* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%c] but was [%c]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_bool(const unsigned int check_mode, int result, __clove_test_t* _this) {
    /*
        In C standard:
        - 0 is FALSE
        - 1 is TRUE
        NOTE: in practice anything different from 0 is considered TRUE
    */
    int pass_scenario = 0;
    char expectedBool[6];
    char resultBool[6];

    if (check_mode == __CLOVE_ASSERT_CHECK_TRUE) {
        pass_scenario = 0 != result; // not(0) is TRUE
        strcpy_s(expectedBool, 6, "true");
        strcpy_s(resultBool, 6, "false");
    }
    else if (check_mode == __CLOVE_ASSERT_CHECK_FALSE) {
        pass_scenario = 0 == result; // 0 is FALSE
        strcpy_s(expectedBool, 6, "false");
        strcpy_s(resultBool, 6, "true");
    }

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%s] but was [%s]", expectedBool, resultBool);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_null(const unsigned int check_mode, const char* expected, void* result, __clove_test_t* _this) {
    int pass_scenario;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) pass_scenario = result == NULL;
    else pass_scenario = result != NULL;

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%s] but was [%p]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_ptr(const unsigned int check_mode, void* expected, void* result, __clove_test_t* _this) {
    int pass_scenario;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) pass_scenario = expected == result;
    else pass_scenario = expected != result;

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%p] but was [%p]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_float(const unsigned int check_mode, float expected, float result, __clove_test_t* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = fabsf(expected - result) <= __CLOVE_FLOATING_PRECISION; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = fabsf(expected - result) > __CLOVE_FLOATING_PRECISION; }

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%f] but was [%f]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_double(const unsigned int check_mode, double expected, double result, __clove_test_t* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = (float)fabs(expected - result) <= __CLOVE_FLOATING_PRECISION; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = (float)fabs(expected - result) > __CLOVE_FLOATING_PRECISION; }

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%f] but was [%f]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_string(const unsigned int check_mode, const char* expected, const char* result, __clove_test_t* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = strcmp(expected, result) == 0; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = strcmp(expected, result) != 0; }

    if (pass_scenario) {
        __clove_pass("", _this);
    }
    else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf_s(msg, sizeof(msg), "expected [%s] but was [%s]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_pad_right(char* result, char* strToPad) {
    int targetStrLen = __CLOVE_TEST_ENTRY_LENGTH;           // Target output length  
    const char* padding = "...................................................................................";

    int padLen = targetStrLen - (int)strlen(strToPad); // Calc Padding length
    if (padLen < 0) padLen = 0;    // Avoid negative length

    // FORMAT SPECIFIER: https://www.tutorialspoint.com/format-specifiers-in-c
    // %* => minimal length taken from the padding string
    // .* => precision, exact length of the string taken from the padding string
    sprintf_s(result, __CLOVE_STRING_LENGTH, "%s%*.*s", strToPad, padLen, padLen, padding);  // LEFT Padding 
}

static const char* __clove_rel_src(const char* path) {
    //https://stackoverflow.com/questions/9834067/difference-between-char-and-const-char
    const char* subpath = strstr(path, "\\src");
    if (subpath == NULL) subpath = strstr(path, "\\tests");
    if (subpath == NULL) return path;
    return subpath + 1;
}

static void __clove_exec_suite(__clove_suite_t* suite, int test_counter, unsigned int* passed, unsigned int* failed, unsigned int* skipped) {
    suite->fixtures.setup_once();

    for (int i = 0; i < suite->test_count; i++) {
        __clove_test_t* each_test = (__clove_test_t*)__clove_vector_get(&suite->tests, i);
        each_test->result = __CLOVE_TEST_SKIPPED;

        suite->fixtures.setup();
        each_test->funct(each_test);
        suite->fixtures.teardown();

        char result[__CLOVE_STRING_LENGTH], strToPad[__CLOVE_TEST_ENTRY_LENGTH];
        snprintf(strToPad, __CLOVE_TEST_ENTRY_LENGTH, "%d) %s.%s", test_counter + i, suite->name, each_test->name);

        __clove_pad_right(result, strToPad);

        switch (each_test->result) {
        case __CLOVE_TEST_PASSED: {
            (*passed)++;
            printf("%s %s%s\n", __CLOVE_INFO, result, __CLOVE_PASSED);
            break;
        }
        case __CLOVE_TEST_FAILED: {
            (*failed)++;
            printf("%s %s%s => %s@%d: %s\n", __CLOVE_ERRO, result, __CLOVE_FAILED, each_test->file_name, each_test->line, each_test->err_msg);
            break;
        }
        case __CLOVE_TEST_SKIPPED: {
            (*skipped)++;
            printf("%s %s%s\n", __CLOVE_WARN, result, __CLOVE_SKIPPED);
            break;
        }
        default:
            break;
        }
    }
    suite->fixtures.teardown_once();
}

static void __clove_exec_suites(__clove_suite_t* suites, int suite_count, int test_count) {
    printf("%s Executing Test Runner in 'Fail Safe Verbose' mode\n", __CLOVE_INFO);
    printf("%s Suite / Tests found: %d / %d \n", __CLOVE_INFO, suite_count, test_count);
    unsigned int passed = 0;
    unsigned int failed = 0;
    unsigned int skipped = 0;

    int test_start_counter = 1;
    for (int i = 0; i < suite_count; ++i) {
        __clove_suite_t* each_suite = &suites[i];
        __clove_exec_suite(each_suite, test_start_counter, &passed, &failed, &skipped);
        test_start_counter += each_suite->test_count;
    }

    printf("%s Total: %d, Passed: %d, Failed: %d, Skipped: %d\n", __CLOVE_INFO, test_count, passed, failed, skipped);
    if (passed == test_count) { printf("%s Test result => SUCCESS :-)\n", __CLOVE_INFO); }
    else if (failed > 0) { printf("%s Test result => FAILURE :_(\n", __CLOVE_ERRO); }
    else if (skipped > 0) { printf("%s Test result => OK, but some test has been skipped!\n", __CLOVE_WARN); }
}

#define __CLOVE_TEST_GUARD \
    if (_this->result == __CLOVE_TEST_FAILED) { return; }\
    if (_this->file_name[0] == '\0') strcpy_s(_this->file_name, __CLOVE_STRING_LENGTH, __clove_rel_src(__FILE__));\
    _this->line=__LINE__;

#ifdef _WIN32
#define __CLOVE_PATH_SEPARATOR '\\'

#pragma region SetupAnsiConsoleForWindows
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif

static void __clove_setup_ansi_console() {
    DWORD outMode = 0, inMode = 0;
    HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE stdinHandle = GetStdHandle(STD_INPUT_HANDLE);

    if (stdoutHandle == INVALID_HANDLE_VALUE || stdinHandle == INVALID_HANDLE_VALUE) {
        //exit(GetLastError());
        return; //if fails let clove to work anyway
    }

    if (!GetConsoleMode(stdoutHandle, &outMode) || !GetConsoleMode(stdinHandle, &inMode)) {
        //exit(GetLastError());
        return; //if fails let clove to work anyway
    }

    DWORD outModeInit = outMode;
    DWORD inModeInit = inMode;

    // Enable ANSI escape codes
    outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    // Set stdin as no echo and unbuffered
    inMode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);

    if (!SetConsoleMode(stdoutHandle, outMode) || !SetConsoleMode(stdinHandle, inMode)) {
        //exit(GetLastError());
        return; //if fails let clove to work anyway
    }
}
#pragma endregion //SetupAnsiConsoleForWindows
#else
#define __CLOVE_PATH_SEPARATOR '/'

static void __clove_setup_ansi_console() {
    /* Nothing to do at the moment for other OS */
}
#endif //_WIN32

extern char* __clove_exec_path;
extern char* __clove_exec_base_path;

static void __clove_replace_char(char* str, char src_chr, char dst_chr) {
    int size = (int)strlen(str);
    for (int i = 0; i < size; ++i) {
        if (str[i] == src_chr) {
            str[i] = dst_chr;
        }
    }
}

static char* __clove_basepath(char* path) {
    //make sure path contains only separator specific for the OS
#ifdef _WIN32
    __clove_replace_char(path, '/', __CLOVE_PATH_SEPARATOR);
#else
    __clove_replace_char(path, '\\', __CLOVE_PATH_SEPARATOR);
#endif //_WIN32

    const char* last_addr = strrchr((const char*)path, __CLOVE_PATH_SEPARATOR);
    int bytes_count;

    if (!last_addr) {
        bytes_count = (int)strlen(path);
    }
    else {
        bytes_count = (int)(last_addr - path);
    }
    int size = bytes_count + 1; // +1 take into account null terminator

    char* base_path = (char*)malloc(sizeof(char) * size);
    strncpy_s(base_path, size, path, bytes_count);
    return base_path;
}

#pragma region Private APIs - Manual
#define __CLOVE_RUNNER_MANUAL(...) \
char* __clove_exec_path;\
char* __clove_exec_base_path;\
int main(int argc, char* argv[]) {\
    __clove_setup_ansi_console();\
    __clove_exec_path = argv[0]; \
    __clove_exec_base_path = __clove_basepath(argv[0]); \
    static void (*suite_ptr[])(__clove_suite_t*) = {__VA_ARGS__};\
    int suite_count = sizeof(suite_ptr) / sizeof(suite_ptr[0]); \
    __clove_vector_params_t vector_params = __CLOVE_VECTOR_DEFAULT_PARAMS; \
    vector_params.item_size = sizeof(__clove_suite_t); \
    vector_params.initial_capacity = suite_count; \
    vector_params.item_ctor = __clove_vector_suite_ctor_manual; \
    vector_params.item_dtor = __clove_vector_suite_dtor_manual; \
    __clove_vector_t suites; \
    __clove_vector_init(&suites, &vector_params); \
    int test_count = 0;\
    for(int i=0; i < suite_count; ++i) {\
        __clove_suite_t* suite = (__clove_suite_t*)__clove_vector_add_empty(&suites); \
        suite_ptr[i](suite);\
        test_count += suite->test_count;\
    }\
    __clove_exec_suites((__clove_suite_t*)(suites.items), suite_count, test_count);\
    free(__clove_exec_base_path); \
    __clove_vector_free(&suites); \
    return 0;\
}

#define __CLOVE_SUITE_DECL_MANUAL(title) void title(__clove_suite_t *_this_suite);

#define __CLOVE_SUITE_MANUAL(title) \
void title(__clove_suite_t *_this_suite) { \
    static char name[] = #title;\
    _this_suite->name = name;
#define __CLOVE_SUITE_SETUP_ONCE_MANUAL(funct) _this_suite->fixtures.setup_once = funct;
#define __CLOVE_SUITE_TEARDOWN_ONCE_MANUAL(funct) _this_suite->fixtures.teardown_once = funct;
#define __CLOVE_SUITE_SETUP_MANUAL(funct) _this_suite->fixtures.setup = funct;
#define __CLOVE_SUITE_TEARDOWN_MANUAL(funct) _this_suite->fixtures.teardown = funct;
#define __CLOVE_SUITE_TESTS_MANUAL(...) \
    static void (*func_ptr[])(__clove_test_t*) = {__VA_ARGS__};\
    static char functs_as_str[] = #__VA_ARGS__;\
    int test_count = sizeof(func_ptr) / sizeof(func_ptr[0]);\
    _this_suite->name = name;\
    _this_suite->test_count = test_count;\
    __clove_vector_params_t vector_params = __CLOVE_VECTOR_DEFAULT_PARAMS; \
    vector_params.item_size = sizeof(__clove_test_t); \
    vector_params.initial_capacity = test_count; \
    vector_params.item_ctor = __clove_vector_test_ctor; \
    __clove_vector_init(&_this_suite->tests, &vector_params); \
    char *context = NULL;\
    for(int i=0; i < test_count; ++i) {\
        char *token;\
        if (i==0) { token = strtok_s(functs_as_str, ", ", &context); }\
        else { token = strtok_s(NULL, ", ", &context); }\
        __clove_test_t* test = (__clove_test_t*)__clove_vector_add_empty(&_this_suite->tests); \
        test->name = token;\
        test->funct = (*func_ptr[i]);\
    }\
}

#define __CLOVE_TEST_MANUAL(title) static void title(__clove_test_t *_this) 

#pragma endregion //Manual


#pragma region Private APIs - Autodiscovery
typedef struct __clove_symbols_context_t {
    __clove_suite_t* last_suite;
    __clove_vector_t suites;
    int suites_count;
    int tests_count;
    size_t prefix_length;
} __clove_symbols_context_t;

typedef struct __clove_symbols_function_t {
    char* name;
    void* pointer;
} __clove_symbols_function_t;

typedef void (*__clove_symbols_function_action)(__clove_symbols_function_t, __clove_symbols_context_t* context);

//For each OS / symbols table format
static int __clove_symbols_for_each_function_by_prefix(const char* prefix, __clove_symbols_function_action action, __clove_symbols_context_t* action_context);

static void __clove_symbols_function_collect(__clove_symbols_function_t exported_funct, __clove_symbols_context_t* context) {
    static char* end_suite_separator = "___";
    static int end_suite_separator_length = 3;
    static char* test_separator = "20_";
    static int test_separator_length = 3;

    char* test_full_name = exported_funct.name;

    char* begin_suite_name = test_full_name + context->prefix_length;
    char* end_suite_name = strstr(begin_suite_name, end_suite_separator);
    size_t size = end_suite_name - begin_suite_name;

    char* suite_name = (char*)malloc(size + 1); //size+1 for null terminator
    strncpy_s(suite_name, size + 1, begin_suite_name, size);
    suite_name[size] = '\0'; //maybe could be avoided?!

    //if suite changes, then set as next suite to collect the tests
    if (context->last_suite == NULL || strcmp(suite_name, context->last_suite->name) != 0) {
        context->last_suite = (__clove_suite_t*)__clove_vector_add_empty(&context->suites);
        context->last_suite->name = suite_name;
        context->suites_count++;
    }

    char* test_name = end_suite_name + end_suite_separator_length;
    if (test_name[0] == '1' && test_name[1] == '1') {
        context->last_suite->fixtures.setup_once = (void (*)())exported_funct.pointer;
    }
    else if (test_name[0] == '1' && test_name[1] == '2') {
        context->last_suite->fixtures.teardown_once = (void (*)())exported_funct.pointer;
    }
    else if (test_name[0] == '1' && test_name[1] == '3') {
        context->last_suite->fixtures.setup = (void (*)())exported_funct.pointer;
    }
    else if (test_name[0] == '1' && test_name[1] == '4') {
        context->last_suite->fixtures.teardown = (void (*)())exported_funct.pointer;
    }
    else if (test_name[0] == '2' && test_name[1] == '0') {
        __clove_test_t* test = (__clove_test_t*)__clove_vector_add_empty(&context->last_suite->tests);
        //Switched to string allocation to make test structs indipendent from the source memory
        //test->name = test_name + test_separator_length;
        test->name = _strdup(test_name + test_separator_length);
        test->funct = (void (*)(__clove_test_t*))exported_funct.pointer;
        context->last_suite->test_count++;
        context->tests_count++;
    }
}

#ifdef _WIN32
#include <windows.h>

static PIMAGE_EXPORT_DIRECTORY __clove_symbols_get_export_table_from(HMODULE module)
{
    // Start at the base of the module. The MS-DOS stub begins there.
    PBYTE base = (PBYTE)module;
    PIMAGE_DOS_HEADER mds = (PIMAGE_DOS_HEADER)module;

    PIMAGE_NT_HEADERS nt_header = (PIMAGE_NT_HEADERS)(base + mds->e_lfanew);
    if (IMAGE_NT_SIGNATURE != nt_header->Signature) {
        // Bad signature -- invalid image or module handle
        return NULL;
    }

    // Get the COFF file header.
    PIMAGE_FILE_HEADER cfh = &nt_header->FileHeader;

    // Get the "optional" header (it's not actually optional for executables).
    PIMAGE_OPTIONAL_HEADER oh = &nt_header->OptionalHeader;

    // Finally, get the export directory table.
    if (IMAGE_DIRECTORY_ENTRY_EXPORT >= oh->NumberOfRvaAndSizes) {
        // This image doesn't have an export directory table.
        return NULL;
    }
    DWORD rva = oh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    PIMAGE_EXPORT_DIRECTORY edt = (PIMAGE_EXPORT_DIRECTORY)(base + rva);
    return edt;
}

static int __clove_symbols_for_each_function_by_prefix(const char* prefix, __clove_symbols_function_action action, __clove_symbols_context_t* action_context) {
    HMODULE module = GetModuleHandle(0);
    PIMAGE_EXPORT_DIRECTORY export_dir = __clove_symbols_get_export_table_from(module);
    if (!export_dir) {
        return -1;
    }

    //Note: Don't know why if there is no exported function results NumberOfNames = 64. NumberOfFunctions = 0 instead.
    //      So checking both counters to be sure if there is any exported function
    if (export_dir->NumberOfNames == 0 || export_dir->NumberOfFunctions == 0) {
        return -1;
    }

    PBYTE base_addr = (PBYTE)module;
    DWORD names_count = export_dir->NumberOfNames;
    PDWORD names_ptr = (PDWORD)(base_addr + export_dir->AddressOfNames);
    DWORD ordinal_base_number = export_dir->Base;
    PWORD ordinals_ptr = (PWORD)(base_addr + export_dir->AddressOfNameOrdinals); //ordinal offset from base ordinal
    PDWORD functs_address_ptr = (PDWORD)(base_addr + export_dir->AddressOfFunctions);

    size_t prefix_length = strlen(prefix);

    //Takes advantage of symbols are lexically sorted. 
    //And also that prefix starting with "__" will come first. So no need to start a binary search
    unsigned char match_ongoing = 0;
    for (size_t i = 0; i < export_dir->NumberOfNames; ++i)
    {
        char* each_name = (char*)(base_addr + names_ptr[i]);
        unsigned long each_ordinal = ordinal_base_number + ordinals_ptr[i];
        unsigned long* each_funct_addr = (PDWORD)(base_addr + functs_address_ptr[each_ordinal - ordinal_base_number]);
        //printf("%lu) %s [0x%p]\n", each_ordinal, each_name, each_funct_addr);

        if (strncmp(prefix, each_name, prefix_length) == 0) {
            if (!match_ongoing) match_ongoing = 1;
            __clove_symbols_function_t funct = { each_name, each_funct_addr };
            action(funct, action_context);
        }
        else {
            //At the first failure, if match was ongoing then there are no more symbol with that prefix
            if (match_ongoing) break;
        }
    }
    return 0;
}
#elif __APPLE__
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <mach-o/getsect.h>
#include <mach-o/loader.h>
#include <mach-o/swap.h>
#include <mach-o/dyld.h>

typedef struct __clove_symbols_macos_module_t {
    void* handle;
    size_t size; //mmap handle size;
    intptr_t address; //module base address 
} __clove_symbols_macos_module_t;

static intptr_t __clove_symbols_macos_image_slide(const char* path)
{
    for (uint32_t i = 0; i < _dyld_image_count(); i++)
    {
        if (strcmp(_dyld_get_image_name(i), path) == 0)
            return _dyld_get_image_vmaddr_slide(i);
    }
    return 0;
}

static int __clove_symbols_macos_open_module_handle(const char* module_path, __clove_symbols_macos_module_t* out_module) {
    int fd;
    if ((fd = open(module_path, O_RDONLY)) < 0) {
        return 1;
    }

    lseek(fd, 0, SEEK_SET);
    struct stat st;
    if (fstat(fd, &st) < 0) {
        return 2;
    }

    void* map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        return 3;
    }
    //mprotect(map, st.st_size, PROT_WRITE);
    close(fd);

    out_module->handle = map;
    out_module->size = st.st_size;
    out_module->address = __clove_symbols_macos_image_slide(module_path);
    return 0;
}

static void __clove_symbols_macos_close_module_handle(__clove_symbols_macos_module_t* module) {
    munmap(module->handle, module->size);
    module->handle = NULL;
    module->size = 0;
}

static struct load_command* __clove_symbols_macos_find_command(struct mach_header_64* header, uint32_t cmd) {
    struct load_command* lc = (struct load_command*)((uint64_t)header + sizeof(struct mach_header_64));
    for (uint32_t i = 0; i < header->ncmds; i++) {
        if (lc->cmd == cmd) {
            return lc;
        }
        lc = (struct load_command*)((uint64_t)lc + lc->cmdsize);
    }
    return NULL;
}

static int __clove_symbols_for_each_function_by_prefix(const char* prefix, __clove_symbols_function_action action, __clove_symbols_context_t* action_context) {
    const char* module_path = __clove_exec_path;

    __clove_symbols_macos_module_t module;
    if (__clove_symbols_macos_open_module_handle(module_path, &module) != 0) { return 1; };

    //Handling Mach-o file format x86_64 (little endian). This should works both for Intel and M1 cpus with 64 bits architecture
    uint32_t magic_number = *(uint32_t*)module.handle;
    if (magic_number != MH_MAGIC_64) {
        puts("Current executable format is not supported (it's not Mach-o 64bit little-endian!");
        return 2;
    }

    struct mach_header_64* header = (struct mach_header_64*)module.handle;
    struct load_command* symbol_lc = __clove_symbols_macos_find_command(header, LC_SYMTAB);
    struct symtab_command* symbol_cmd = (struct symtab_command*)symbol_lc;

    struct nlist_64* symbol_table_64 = (struct nlist_64*)((uint64_t)header + symbol_cmd->symoff);
    char* str_table = (char*)header + symbol_cmd->stroff;
    //symbol_cmd->nsyms #number of symbols in the symbol table 

    //Loading Dynamic Symbol table to scan only for external symbols symbols in the Symbol Table 
    struct load_command* dynsymbol_lc = __clove_symbols_macos_find_command(header, LC_DYSYMTAB);
    struct dysymtab_command* dysymbol_cmd = (struct dysymtab_command*)dynsymbol_lc;
    uint32_t external_symbol_start_index = dysymbol_cmd->iextdefsym;
    uint32_t external_symbol_end_index = external_symbol_start_index + dysymbol_cmd->nextdefsym - 1U;

    const size_t prefix_length = strlen(prefix);
    uint8_t match_ongoing = 0;
    for (uint32_t i = external_symbol_start_index; i <= external_symbol_end_index; i++) {
        struct nlist_64* sym = &symbol_table_64[i];
        uint32_t table_index = sym->n_un.n_strx;
        char* each_name = &str_table[table_index + 1]; //macho add one '_' before each symbol, so with +1 we want to skip it.
        void* each_funct_addr = (void*)(sym->n_value + module.address); //n_value = offset address within TEXT segment (includes base addr of the TEXT segment)

        //Symbols seems to be "locally" sorted, so all clove external functions seems to be next to each other and sorted
        if (strncmp(prefix, each_name, prefix_length) == 0) {
            if (!match_ongoing) {
                match_ongoing = 1;
            }
            __clove_symbols_function_t funct = { each_name, each_funct_addr };
            action(funct, action_context);
        }
        else {
            //At the first failure, if match was ongoing then there are no more symbol with that prefix
            if (match_ongoing) {
                break;
            }
        }

    }
    __clove_symbols_macos_close_module_handle(&module);
    return 0;
}
#elif __linux__
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <elf.h>
#include <link.h>

typedef struct __clove_symbols_lixux_module_t {
    void* handle;
    size_t size; //mmap handle size;
    uintptr_t address; //module base address 
} __clove_symbols_lixux_module_t;

static int __clove_symbols_lixux_dl_callback(struct dl_phdr_info* info, size_t size, void* data)
{
    const char* cb = (const char*)&__clove_symbols_lixux_dl_callback;
    const char* base = (const char*)info->dlpi_addr;
    const ElfW(Phdr)* first_load = NULL;

    for (int j = 0; j < info->dlpi_phnum; j++) {
        const ElfW(Phdr)* phdr = &info->dlpi_phdr[j];

        if (phdr->p_type == PT_LOAD) {
            const char* beg = base + phdr->p_vaddr;
            const char* end = beg + phdr->p_memsz;

            if (first_load == NULL) first_load = phdr;
            if (beg <= cb && cb < end) {
                // Found PT_LOAD that "covers" callback().
                //printf("ELF header is at %p, image linked at 0x%zx, relocation: 0x%zx\n", base + first_load->p_vaddr, first_load->p_vaddr, info->dlpi_addr);
                uintptr_t* addr_ptr = (uintptr_t*)data;
                *addr_ptr = info->dlpi_addr;
                return 1;
            }
        }
    }
    return 0;
}

static uintptr_t __clove_symbols_lixux_base_addr(const char* path)
{
    uintptr_t base_addr;
    dl_iterate_phdr(__clove_symbols_lixux_dl_callback, &base_addr);
    return base_addr;
}

static int __clove_symbols_lixux_open_module_handle(const char* module_path, __clove_symbols_lixux_module_t* out_module) {
    int fd;
    if ((fd = open(module_path, O_RDONLY)) < 0) {
        return 1;
    }

    lseek(fd, 0, SEEK_SET);
    struct stat st;
    if (fstat(fd, &st) < 0) {
        return 2;
    }

    void* map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        return 3;
    }
    //mprotect(map, st.st_size, PROT_WRITE);
    close(fd);

    out_module->handle = map;
    out_module->size = st.st_size;
    out_module->address = __clove_symbols_lixux_base_addr(module_path);
    return 0;
}

static void __clove_symbols_lixux_close_module_handle(__clove_symbols_lixux_module_t* module) {
    munmap(module->handle, module->size);
    module->handle = NULL;
    module->size = 0;
}


/*
 * Compare two functions by their name
 * Return negative if name1 is lesser than name2
 * Return positive if name1 is greather than name1
 * Return zero if name1 and name2 are equals
 */
static int __clove_symbols_funct_name_comparator(void* f1, void* f2) {
    __clove_symbols_function_t* funct1 = (__clove_symbols_function_t*)f1;
    __clove_symbols_function_t* funct2 = (__clove_symbols_function_t*)f2;
    return strcmp(funct1->name, funct2->name);
}

static int __clove_symbols_for_each_function_by_prefix(const char* prefix, __clove_symbols_function_action action, __clove_symbols_context_t* action_context) {
    const char* module_path = __clove_exec_path;

    __clove_symbols_lixux_module_t module;
    if (__clove_symbols_lixux_open_module_handle(module_path, &module) != 0) { return 1; }

    //Check Elf header to be 64 bit little endian
    unsigned char* magic = (unsigned char*)module.handle;
    bool is_elf = (magic[0] == 0x7f && magic[1] == 0x45 && magic[2] == 0x4c && magic[3] == 0x46);
    bool is_64 = (magic[4] == 0x02); //0x01 is 32 bit
    bool is_little = (magic[5] == 0x01); //0x02 is big endian

    if (!(is_elf && is_64 && is_little)) {
        puts("Current executable format is not supported (it's not ELF 64bit little-endian!");
        return 2;
    }

    Elf64_Ehdr* header = (Elf64_Ehdr*)module.handle;

    Elf64_Shdr* sections = (Elf64_Shdr*)((uint64_t)header + header->e_shoff);
    size_t section_count = header->e_shnum;

    Elf64_Sym* symbol_table = NULL;
    Elf64_Shdr* symbol_table_section = NULL;
    for (int i = 0; i < section_count; ++i) {
        if (sections[i].sh_type == SHT_SYMTAB) {
            symbol_table = (Elf64_Sym*)((uint64_t)header + sections[i].sh_offset);
            symbol_table_section = &sections[i];
            break;
        }
    }

    Elf64_Shdr* name_table_section = &sections[symbol_table_section->sh_link];
    char* symbol_name_table = (char*)((uint64_t)header + name_table_section->sh_offset);

    size_t symbol_count = symbol_table_section->sh_size / symbol_table_section->sh_entsize;

    //Vector could be replace with sorted tree to sort while scanning for clove functions
    __clove_vector_t clove_functions;
    //do macro with default that accept item size (it is mandatory basically)
    __clove_vector_params_t params = __CLOVE_VECTOR_DEFAULT_PARAMS;
    params.item_size = sizeof(__clove_symbols_function_t);
    __clove_vector_init(&clove_functions, &params);

    size_t prefix_length = strlen(prefix);

    for (size_t i = 0; i < symbol_count; ++i) {
        Elf64_Sym* sym = &symbol_table[i];
        if (ELF64_ST_TYPE(sym->st_info) == STT_FUNC) {
            size_t name_index = sym->st_name;
            char* sym_name = symbol_name_table + name_index;
            if (!strncmp(sym_name, prefix, prefix_length)) {
                __clove_symbols_function_t* each_funct = (__clove_symbols_function_t*)__clove_vector_add_empty(&clove_functions);
                each_funct->name = sym_name;
                each_funct->pointer = (void*)(module.address + sym->st_value);
            }
        }
    }

    //Sort Symbols Alphanumeric ascendent
    __clove_vector_sort(&clove_functions, __clove_symbols_funct_name_comparator);

    for (size_t i = 0; i < __clove_vector_count(&clove_functions); ++i)
    {
        __clove_symbols_function_t* each_funct = (__clove_symbols_function_t*)__clove_vector_get(&clove_functions, i);
        action(*each_funct, action_context);
        //puts(each_funct->name);
    }
    __clove_vector_free(&clove_functions);
    __clove_symbols_lixux_close_module_handle(&module);
    return 0;
}
#else 
//Not Possible. Shoud be one of the OS cases before
static int __clove_symbols_for_each_function_by_prefix(const char* prefix, __clove_symbols_function_action action, __clove_symbols_context_t* action_context) {
    puts("Autodiscovery not yet implemented for this OS!!!");
    return 1;
}
#endif //_WIN32 symbol table


#define __CLOVE_RUNNER_AUTO() \
char* __clove_exec_path;\
char* __clove_exec_base_path;\
int main(int argc, char* argv[]) {\
    __clove_setup_ansi_console();\
    __clove_exec_path = argv[0]; \
    __clove_exec_base_path = __clove_basepath(argv[0]); \
    __clove_symbols_context_t context; \
    __clove_vector_params_t vector_params = __CLOVE_VECTOR_DEFAULT_PARAMS; \
    vector_params.item_size = sizeof(__clove_suite_t); \
    vector_params.item_ctor = __clove_vector_suite_ctor; \
    vector_params.item_dtor = __clove_vector_suite_dtor; \
    __clove_vector_init(&context.suites, &vector_params); \
    context.suites_count = 0; \
    context.last_suite = NULL; \
    context.prefix_length = strlen("__clove_sym___"); \
    context.tests_count = 0; \
    int result = __clove_symbols_for_each_function_by_prefix("__clove_sym___", __clove_symbols_function_collect, &context); \
    if (result == 0) { \
        __clove_exec_suites((__clove_suite_t*)(context.suites.items), context.suites_count, context.tests_count); \
    } \
    free(__clove_exec_base_path); \
    __clove_vector_free(&context.suites); \
    return 0;\
}

static const char* __clove_get_exec_path() {
    return __clove_exec_path;
}
static const char* __clove_get_exec_base_path() {
    return __clove_exec_base_path;
}


#ifdef _WIN32
#define __CLOVE_API_EXPORT __declspec(dllexport)
#else 
#define __CLOVE_API_EXPORT
#endif //_WIN32


#define __CLOVE_SUITE_METHOD_AUTO_2(suite, title, param) __CLOVE_API_EXPORT void __clove_sym___##suite##___##title(param)
#define __CLOVE_SUITE_METHOD_AUTO_1(suite, name, param) __CLOVE_SUITE_METHOD_AUTO_2(suite, name, param)

#define __CLOVE_SUITE_SETUP_ONCE_AUTO() __CLOVE_SUITE_METHOD_AUTO_1( CLOVE_SUITE_NAME, 11_setuponce, void)
#define __CLOVE_SUITE_TEARDOWN_ONCE_AUTO() __CLOVE_SUITE_METHOD_AUTO_1( CLOVE_SUITE_NAME, 12_teardownonce, void)
#define __CLOVE_SUITE_SETUP_AUTO() __CLOVE_SUITE_METHOD_AUTO_1( CLOVE_SUITE_NAME, 13_setup, void)
#define __CLOVE_SUITE_TEARDOWN_AUTO() __CLOVE_SUITE_METHOD_AUTO_1( CLOVE_SUITE_NAME, 14_teardown, void)
#define __CLOVE_TEST_AUTO(title) __CLOVE_SUITE_METHOD_AUTO_1( CLOVE_SUITE_NAME, 20_ ## title, __clove_test_t *_this)
#pragma endregion //Autodiscovery
#pragma endregion // Private APIs

#pragma region PUBLIC APIs
#pragma region Public APIs - UTILS
/*
 * Provide the executable path
 */
#define CLOVE_EXEC_PATH() __clove_get_exec_path()
 /*
  * Provide the executable base path
  */
#define CLOVE_EXEC_BASE_PATH() __clove_get_exec_base_path()
#pragma endregion //UTILS

#pragma region Public APIs - ASSERTIONS
#define CLOVE_PASS() __CLOVE_TEST_GUARD __clove_pass("", _this);
#define CLOVE_FAIL() __CLOVE_TEST_GUARD __clove_fail("A FAIL assertion was met!", _this);
#define CLOVE_IS_TRUE(res) __CLOVE_TEST_GUARD __clove_check_bool(__CLOVE_ASSERT_CHECK_TRUE, res, _this);
#define CLOVE_IS_FALSE(res) __CLOVE_TEST_GUARD __clove_check_bool(__CLOVE_ASSERT_CHECK_FALSE, res, _this);
#define CLOVE_CHAR_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_char(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_CHAR_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_char(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#define CLOVE_INT_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_int(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_INT_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_int(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#define CLOVE_UINT_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_uint(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_UINT_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_uint(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#define CLOVE_LONG_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_long(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_LONG_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_long(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#define CLOVE_LLONG_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_llong(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_LLONG_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_llong(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#define CLOVE_ULONG_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_ulong(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_ULONG_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_ulong(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#define CLOVE_ULLONG_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_ullong(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_ULLONG_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_ullong(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#define CLOVE_FLOAT_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_float(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_FLOAT_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_float(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#define CLOVE_DOUBLE_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_double(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_DOUBLE_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_double(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#define CLOVE_NULL(res) __CLOVE_TEST_GUARD __clove_check_null(__CLOVE_ASSERT_CHECK_EQUALITY, "NULL", res, _this);
#define CLOVE_NOT_NULL(res) __CLOVE_TEST_GUARD __clove_check_null(__CLOVE_ASSERT_CHECK_DIFFERENCE, "!NULL", res, _this);
#define CLOVE_PTR_EQ(p1, p2) __CLOVE_TEST_GUARD __clove_check_ptr(__CLOVE_ASSERT_CHECK_EQUALITY, p1, p2, _this);
#define CLOVE_PTR_NE(p1, p2) __CLOVE_TEST_GUARD __clove_check_ptr(__CLOVE_ASSERT_CHECK_DIFFERENCE, p1, p2, _this);
#define CLOVE_STRING_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_string(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_STRING_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_string(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#pragma endregion //ASSERTIONS


#if !defined(CLOVE_ENABLE_MANUAL) || defined(CLOVE_SUITE_NAME)
#define __CLOVE_MODE_AUTO
#else 
#define __CLOVE_MODE_MANUAL
#endif

#ifdef __CLOVE_MODE_MANUAL

  /*
   * Test Execution.
   * Take as input a list of suites.
   */
#define CLOVE_RUNNER(...) __CLOVE_RUNNER_MANUAL(__VA_ARGS__)

   /*
    * Suite declaration. (basically a standard method forward declaration)
    * Useful to work with compilation unit instead of just using header file to implement test and test suite
    */
#define CLOVE_SUITE_DECL(title) __CLOVE_SUITE_DECL_MANUAL(title);

    /*
     * Suite Implementation
     */
#define CLOVE_SUITE(title) __CLOVE_SUITE_MANUAL(title)
#define CLOVE_SUITE_SETUP_ONCE(funct) __CLOVE_SUITE_SETUP_ONCE_MANUAL(funct)
#define CLOVE_SUITE_TEARDOWN_ONCE(funct) __CLOVE_SUITE_TEARDOWN_ONCE_MANUAL(funct)
#define CLOVE_SUITE_SETUP(funct) __CLOVE_SUITE_SETUP_MANUAL(funct)
#define CLOVE_SUITE_TEARDOWN(funct) __CLOVE_SUITE_TEARDOWN_MANUAL(funct)
#define CLOVE_SUITE_TESTS(...) __CLOVE_SUITE_TESTS_MANUAL(__VA_ARGS__)
     /*
      * Define a new test named 'title'
      */
#define CLOVE_TEST(title) __CLOVE_TEST_MANUAL(title)


#else // AUTODISCOVERY ENABLED

#define CLOVE_RUNNER() __CLOVE_RUNNER_AUTO()
  //Need CLOVE_SUITE_NAME
#define CLOVE_SUITE_SETUP_ONCE() __CLOVE_SUITE_SETUP_ONCE_AUTO()
#define CLOVE_SUITE_TEARDOWN_ONCE() __CLOVE_SUITE_TEARDOWN_ONCE_AUTO()
#define CLOVE_SUITE_SETUP() __CLOVE_SUITE_SETUP_AUTO()
#define CLOVE_SUITE_TEARDOWN() __CLOVE_SUITE_TEARDOWN_AUTO()
#define CLOVE_TEST(title) __CLOVE_TEST_AUTO(title)

#endif //__CLOVE_MODE_MANUAL


#pragma endregion //Public APIs

#endif //CLOVE_H