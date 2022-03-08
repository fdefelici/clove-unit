/*
 * clove-unit
 * v2.2.1
 * Unit Testing library for C/C++
 * https://github.com/fdefelici/clove-unit
 *
 */
#ifndef __CLOVE_H
#define __CLOVE_H

#define __CLOVE_VERSION_MAJOR 2
#define __CLOVE_VERSION_MINOR 2
#define __CLOVE_VERSION_PATCH 1
#define __CLOVE_VERSION "2.2.1"

#ifdef __linux
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif //_GNU_SOURCE
#endif //__linux

#ifdef _WIN32
#define __CLOVE_PATH_SEPARATOR '\\'
#else 
#define __CLOVE_PATH_SEPARATOR '/'
#endif //_WIN32

#pragma region PRIVATE APIs
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#pragma region PRIVATE APIs - String
static bool __clove_string_equal(const char* str1, const char* str2) {
    return strcmp(str1, str2) == 0;
}

static bool __clove_string_startswith(const char* str1, const char* prefix) {
    return strncmp(str1, prefix, strlen(prefix)) == 0;
}
#pragma endregion //String

#pragma region PRIVATE APIs - Time
typedef enum __clove_time_traslation_e {
    __CLOVE_TIME_TRASL_NANOS_PER_SEC = 1000000000, //1 BILLION
    __CLOVE_TIME_TRASL_MILLIS_PER_SEC = 1000,
    __CLOVE_TIME_TRASL_NANOS_PER_MILLIS = 1000000 //1 MILION
} __clove_time_traslation_e;

typedef struct __clove_time_t {
    long long seconds;
    long long nanos_after_seconds;
} __clove_time_t;

static __clove_time_t __clove_time_now();

static __clove_time_t __clove_time_sub(__clove_time_t* t1, __clove_time_t* t2) {
    __clove_time_t result;
    result.seconds = t1->seconds - t2->seconds;
    result.nanos_after_seconds = t1->nanos_after_seconds - t2->nanos_after_seconds;
    if (result.seconds > 0 && result.nanos_after_seconds < 0) {
        result.seconds--;
        result.nanos_after_seconds += __CLOVE_TIME_TRASL_NANOS_PER_SEC;
    }
    else if (result.seconds < 0 && result.nanos_after_seconds > 0) {
        result.seconds++;
        result.nanos_after_seconds -= __CLOVE_TIME_TRASL_NANOS_PER_SEC;
    }
    return result;
}

static __clove_time_t __clove_time_sum(__clove_time_t* t1, __clove_time_t* t2) {
    __clove_time_t result;
    result.seconds = t1->seconds + t2->seconds;
    result.nanos_after_seconds = t1->nanos_after_seconds + t2->nanos_after_seconds;
    if (result.nanos_after_seconds >= __CLOVE_TIME_TRASL_NANOS_PER_SEC) {
        result.seconds++;
        result.nanos_after_seconds -= __CLOVE_TIME_TRASL_NANOS_PER_SEC;
    }
    return result;
}

static unsigned long long __clove_time_to_millis(__clove_time_t* t) {
    unsigned long long result = 0;
    result += t->seconds * __CLOVE_TIME_TRASL_MILLIS_PER_SEC;
    result += t->nanos_after_seconds / __CLOVE_TIME_TRASL_NANOS_PER_MILLIS;
    return result;
}

static unsigned long long __clove_time_to_nanos(__clove_time_t* t) {
    unsigned long long result = 0;
    result += t->seconds * __CLOVE_TIME_TRASL_NANOS_PER_SEC;
    result += t->nanos_after_seconds;
    return result;
}

#ifdef _WIN32
#include <windows.h>
#include <time.h>

static __clove_time_t __clove_time_now() {
    static bool first_time = true;
    static LARGE_INTEGER count_per_sec;
    __clove_time_t result;
    result.seconds = 0;
    result.nanos_after_seconds = 0;


    if (first_time) {
        first_time = false;
        if (QueryPerformanceFrequency(&count_per_sec) == 0) {
            return result;
        }
    }

    LARGE_INTEGER count;
    if (QueryPerformanceCounter(&count) == 0) {
        return result;
    }

    result.seconds = count.QuadPart / count_per_sec.QuadPart;
    result.nanos_after_seconds = ((count.QuadPart % count_per_sec.QuadPart) * __CLOVE_TIME_TRASL_NANOS_PER_SEC) / count_per_sec.QuadPart;
    return result;
}
#else 
#include <time.h>
#include <unistd.h>
static __clove_time_t __clove_time_now() {
    struct timespec time_data;
    clock_gettime(CLOCK_REALTIME, &time_data);

    __clove_time_t result;
    result.seconds = time_data.tv_sec;
    result.nanos_after_seconds = time_data.tv_nsec;
    return result;
}
#endif //_WIN32
#pragma endregion




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

static __clove_vector_params_t __clove_vector_params_defaulted(size_t item_size) {
    __clove_vector_params_t params;
    params.item_size = item_size;
    params.initial_capacity = 10;
    params.item_ctor = NULL;
    params.item_dtor = NULL;
    return params;
}

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
        for (size_t i = 0; i < vector->count; ++i) {
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
#define strncpy_s(dest, dest_size, source, count) strncpy(dest, source, count) == NULL
#define strcpy_s(dest, dest_size, src) strcpy(dest, src) == NULL
#define strcat_s(dest, dest_size, source) strcat(dest, source) == NULL
#define strncat_s(dest, dest_size, source, count) strncat(dest, source, count) == NULL
#define _strdup strdup
#define sprintf_s(dest, dest_size, format, ...) sprintf(dest, format, __VA_ARGS__)
#define fopen_s(file_ptr2, file_path, mode) ((*(file_ptr2)) = fopen((file_path),  (mode))) == NULL
#endif

#define __CLOVE_TEST_PASSED 1
#define __CLOVE_TEST_FAILED 2
#define __CLOVE_TEST_SKIPPED 3
#define __CLOVE_FLOATING_PRECISION 0.000001f


typedef union __clove_generic_u {
    bool               _bool;
    char               _char;
    int                _int;
    unsigned int       _uint;
    long               _long;
    unsigned long      _ulong;
    long long          _llong;
    unsigned long long _ullong;
    float              _float;
    double             _double;
    char* _string;
    void* _ptr;
} __clove_generic_u;

typedef enum __clove_generic_type_e {
    __CLOVE_GENERIC_BOOL = 1,
    __CLOVE_GENERIC_CHAR = 2,
    __CLOVE_GENERIC_INT = 3,
    __CLOVE_GENERIC_UINT = 4,
    __CLOVE_GENERIC_LONG = 5,
    __CLOVE_GENERIC_ULONG = 6,
    __CLOVE_GENERIC_LLONG = 7,
    __CLOVE_GENERIC_ULLONG = 8,
    __CLOVE_GENERIC_FLOAT = 9,
    __CLOVE_GENERIC_DOUBLE = 10,
    __CLOVE_GENERIC_STRING = 11,
    __CLOVE_GENERIC_PTR = 12
} __clove_generic_type_e;

typedef enum __clove_assert_check_e {
    __CLOVE_ASSERT_EQ = 1,
    __CLOVE_ASSERT_NE = 2,
    __CLOVE_ASSERT_FAIL = 3
} __clove_assert_check_e;


typedef struct __clove_test_t {
    char* name;
    void (*funct)(struct __clove_test_t*);
    unsigned int result;
    __clove_time_t duration;
    char file_name[__CLOVE_STRING_LENGTH];
    struct {
        unsigned int line;
        __clove_assert_check_e assert;
        __clove_generic_type_e data_type;
        __clove_generic_u expected;
        __clove_generic_u actual;
    } issue;
} __clove_test_t;

typedef struct __clove_suite {
    char* name;
    __clove_vector_t tests;
    size_t test_count;
    struct {
        void (*setup_once)();
        void (*teardown_once)();
        void (*setup)();
        void (*teardown)();
    } fixtures;
    struct {
        __clove_time_t duration;
    } issue;

} __clove_suite_t;


static void __clove_empty_funct() { }

static void __clove_vector_test_ctor(void* test) {
    //cast to __clove_test_t* not needed
    memset(test, 0, sizeof(__clove_test_t));
}

static void __clove_vector_test_dtor(void* test_ptr) {
    __clove_test_t* test = (__clove_test_t*)test_ptr;
    free(test->name);

    //See CLOVE_STRING_EQ and CLOVE_STRING_NE where string allocation happen
    if (test->result == __CLOVE_TEST_FAILED && test->issue.data_type == __CLOVE_GENERIC_STRING) {
        free(test->issue.expected._string);
        free(test->issue.actual._string);
    }
}

static void __clove_vector_suite_ctor(void* suite_ptr) {
    __clove_suite_t* suite = (__clove_suite_t*)suite_ptr;
    suite->name = NULL;
    suite->fixtures.setup_once = __clove_empty_funct;
    suite->fixtures.teardown_once = __clove_empty_funct;
    suite->fixtures.setup = __clove_empty_funct;
    suite->fixtures.teardown = __clove_empty_funct;

    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(__clove_test_t));
    params.item_ctor = __clove_vector_test_ctor;
    params.item_dtor = __clove_vector_test_dtor;
    __clove_vector_init(&(suite->tests), &params);
    suite->test_count = 0;

    suite->issue.duration.seconds = 0;
    suite->issue.duration.nanos_after_seconds = 0;
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
    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(__clove_test_t));
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

#define __CLOVE_INFO "[\x1b[1;34mINFO\x1b[0m]"
#define __CLOVE_WARN "[\x1b[33mWARN\x1b[0m]"
#define __CLOVE_ERRO "[\x1b[1;31mERRO\x1b[0m]"
#define __CLOVE_PASSED "[\x1b[1;32mPASS\x1b[0m]"
#define __CLOVE_SKIPPED "[\x1b[33mSKIP\x1b[0m]"
#define __CLOVE_FAILED "[\x1b[1;31mFAIL\x1b[0m]"

static void __clove_setup_ansi_console();

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

typedef struct __clove_report_t {
    void (*start)(struct __clove_report_t* _this, int suite_count, int test_count);
    void (*end)(struct __clove_report_t* _this, int test_count, int passed, int skipped, int failed);
    void (*test_executed)(struct __clove_report_t* _this, __clove_suite_t* suite, __clove_test_t* test, size_t test_number);
    void (*free)(struct __clove_report_t* _this);
} __clove_report_t;

typedef struct __clove_report_console_t {
    __clove_report_t base;
    __clove_time_t start_time;
} __clove_report_console_t;

static void __clove_report_console_start(__clove_report_t* _this, int suite_count, int test_count) {
    __clove_report_console_t* report = (__clove_report_console_t*)_this;
    report->start_time = __clove_time_now();

    __clove_setup_ansi_console();
    printf("%s Executing Test Runner in 'Fail Safe Verbose' mode\n", __CLOVE_INFO);
    printf("%s Suite / Tests found: %d / %d \n", __CLOVE_INFO, suite_count, test_count);
}

static void __clove_report_console_end(__clove_report_t* _this, int test_count, int passed, int skipped, int failed) {
    __clove_report_console_t* report = (__clove_report_console_t*)_this;
    __clove_time_t end_time = __clove_time_now();
    __clove_time_t diff = __clove_time_sub(&end_time, &(report->start_time));
    unsigned long long millis = __clove_time_to_millis(&diff);

    printf("%s Total: %d, Passed: %d, Failed: %d, Skipped: %d\n", __CLOVE_INFO, test_count, passed, failed, skipped);
    printf("%s Run duration: %llu ms\n", __CLOVE_INFO, millis);
    if (passed == test_count) { printf("%s Run result: SUCCESS :-)\n", __CLOVE_INFO); }
    else if (failed > 0) { printf("%s Run result: FAILURE :_(\n", __CLOVE_ERRO); }
    else if (skipped > 0) { printf("%s Run result: OK, but some test has been skipped!\n", __CLOVE_WARN); }
}

static char* __clove_string_escape(const char* string) {
    size_t str_len = strlen(string);
    size_t esc_len = str_len * 2 + 1; //worst case where each char need escape
    char* escaped = (char*)calloc(esc_len, sizeof(char));

    size_t esc_index = 0;
    for (size_t str_index = 0; str_index < str_len; ++str_index) {
        char c = string[str_index];
        switch (c)
        {
        case '\b':
            escaped[esc_index++] = '\\';
            escaped[esc_index] = 'b';
            break;
        case '\f':
            escaped[esc_index++] = '\\';
            escaped[esc_index] = 'f';
            break;
        case '\n':
            escaped[esc_index++] = '\\';
            escaped[esc_index] = 'n';
            break;
        case '\r':
            escaped[esc_index++] = '\\';
            escaped[esc_index] = 'r';
            break;
        case '\t':
            escaped[esc_index++] = '\\';
            escaped[esc_index] = 't';
            break;
        case '"':
            escaped[esc_index++] = '\\';
            escaped[esc_index] = '"';
            break;
        case '\\':
            escaped[esc_index++] = '\\';
            escaped[esc_index] = '\\';
            break;
        default:
            escaped[esc_index] = c;
            break;
        }
        esc_index++;
    }
    return escaped;
}

static void __clove_string_ellipse(const char* string, size_t str_len, size_t pos, char* out, size_t out_len) {
    if (str_len == 1) {
        out[0] = '\0';
        return;
    }
    if (str_len <= out_len) {
        strcpy_s(out, out_len, string);
        return;
    }

    bool left_ellipse = false;
    bool right_ellipse = false;
    if (pos >= 4) {
        left_ellipse = true;
    }

    if (str_len - pos >= 4) {
        right_ellipse = true;
    }


    size_t out_start = 0;
    size_t start_index = 0;
    size_t out_end = out_len - 1;

    if (left_ellipse) {
        out_start = 3;
        start_index = right_ellipse ? pos - 3 : str_len - 1 - 12;
        out[0] = '.'; out[1] = '.'; out[2] = '.';
    }

    if (right_ellipse) {
        out_end -= 3;
        out[out_end] = '.'; out[out_end + 1] = '.'; out[out_end + 2] = '.';
    }
    out[out_len - 1] = '\0';


    size_t to_copy = out_end - out_start;

    //Copia stringa
    for (size_t i = 0; i < to_copy; ++i) {
        out[out_start + i] = string[start_index + i];
    }
}

static void __clove_report_console_string_ellipse(
    const char* exp, size_t exp_size,
    const char* act, size_t act_size,
    char* exp_short, char* act_short, size_t short_len)
{
    size_t iter_len = exp_size < act_size ? exp_size : act_size;
    if (iter_len == 0) {
        __clove_string_ellipse(exp, exp_size, 0, exp_short, short_len);
        __clove_string_ellipse(act, act_size, 0, act_short, short_len);
        return;
    }

    for (size_t i = 0; i < iter_len; ++i) {
        if (exp[i] != act[i]) {
            __clove_string_ellipse(exp, exp_size, i, exp_short, short_len);
            __clove_string_ellipse(act, act_size, i, act_short, short_len);
            return;
        }
    }
}

static void __clove_report_console_test_executed(struct __clove_report_t* _this, __clove_suite_t* suite, __clove_test_t* test, size_t test_number) {
    char result[__CLOVE_STRING_LENGTH], strToPad[__CLOVE_TEST_ENTRY_LENGTH];
    snprintf(strToPad, __CLOVE_TEST_ENTRY_LENGTH, "%zu) %s.%s", test_number, suite->name, test->name);
    __clove_pad_right(result, strToPad);

    if (test->result == __CLOVE_TEST_PASSED) {
        float millis = (float)(__clove_time_to_nanos(&(test->duration))) / (float)__CLOVE_TIME_TRASL_NANOS_PER_MILLIS;
        int decimal = millis > 1.f ? 0 : 3;
        printf("%s %s%s (%.*f ms)\n", __CLOVE_INFO, result, __CLOVE_PASSED, decimal, millis);
    }
    else if (test->result == __CLOVE_TEST_FAILED) {
        char msg[__CLOVE_STRING_LENGTH] = "FAILURE but NO MESSAGE!!!";

        if (test->issue.assert == __CLOVE_ASSERT_FAIL) {
            sprintf_s(msg, sizeof(msg), "%s", "A FAIL assertion was met!");
        }
        else {
            switch (test->issue.data_type)
            {
            case __CLOVE_GENERIC_BOOL: {
                const char* exp = test->issue.expected._bool ? "true" : "false";
                const char* act = test->issue.actual._bool ? "true" : "false";
                sprintf_s(msg, sizeof(msg), "expected [%s] but was [%s]", exp, act);
                break;
            }
            case __CLOVE_GENERIC_CHAR: {
                const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                const char exp = test->issue.expected._char;
                const char act = test->issue.actual._char;
                sprintf_s(msg, sizeof(msg), "%sexpected [%c] but was [%c]", non, exp, act);
                break;
            }
            case __CLOVE_GENERIC_INT: {
                const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                const int exp = test->issue.expected._int;
                const int act = test->issue.actual._int;
                sprintf_s(msg, sizeof(msg), "%sexpected [%d] but was [%d]", non, exp, act);
                break;
            }
            case __CLOVE_GENERIC_UINT: {
                const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                const unsigned int exp = test->issue.expected._uint;
                const unsigned int act = test->issue.actual._uint;
                sprintf_s(msg, sizeof(msg), "%sexpected [%u] but was [%u]", non, exp, act);
                break;
            }
            case __CLOVE_GENERIC_LONG: {
                const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                const long exp = test->issue.expected._long;
                const long act = test->issue.actual._long;
                sprintf_s(msg, sizeof(msg), "%sexpected [%ld] but was [%ld]", non, exp, act);
                break;
            }
            case __CLOVE_GENERIC_ULONG: {
                const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                const unsigned long exp = test->issue.expected._ulong;
                const unsigned long act = test->issue.actual._ulong;
                sprintf_s(msg, sizeof(msg), "%sexpected [%lu] but was [%lu]", non, exp, act);
                break;
            }
            case __CLOVE_GENERIC_LLONG: {
                const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                const long long exp = test->issue.expected._llong;
                const long long act = test->issue.actual._llong;
                sprintf_s(msg, sizeof(msg), "%sexpected [%lld] but was [%lld]", non, exp, act);
                break;
            }
            case __CLOVE_GENERIC_ULLONG: {
                const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                const unsigned long long exp = test->issue.expected._ullong;
                const unsigned long long act = test->issue.actual._ullong;
                sprintf_s(msg, sizeof(msg), "%sexpected [%llu] but was [%llu]", non, exp, act);
                break;
            }
            case __CLOVE_GENERIC_FLOAT: {
                const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                const float exp = test->issue.expected._float;
                const float act = test->issue.actual._float;
                sprintf_s(msg, sizeof(msg), "%sexpected [%f] but was [%f]", non, exp, act);
                break;
            }
            case __CLOVE_GENERIC_DOUBLE: {
                const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                const double exp = test->issue.expected._double;
                const double act = test->issue.actual._double;
                sprintf_s(msg, sizeof(msg), "%sexpected [%f] but was [%f]", non, exp, act);
                break;
            }
            case __CLOVE_GENERIC_STRING: {
                const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                const char* exp = test->issue.expected._string;
                const char* act = test->issue.actual._string;

                const size_t max_len = 16;
                const size_t exp_len = strlen(exp);
                const size_t act_len = strlen(act);

                if (exp_len <= max_len && act_len <= max_len) {
                    char* exp_escaped = __clove_string_escape(exp);
                    char* act_escaped = __clove_string_escape(act);
                    sprintf_s(msg, sizeof(msg), "%sexpected \"%s\" but was \"%s\"", non, exp_escaped, act_escaped);
                    free(exp_escaped);
                    free(act_escaped);
                }
                else {
                    char exp_short[16];
                    char act_short[16];
                    __clove_report_console_string_ellipse(exp, exp_len, act, act_len, exp_short, act_short, 16);

                    char* exp_escaped = __clove_string_escape(exp_short);
                    char* act_escaped = __clove_string_escape(act_short);
                    sprintf_s(msg, sizeof(msg), "%sexpected [%zu]\"%s\" but was [%zu]\"%s\"", non, exp_len, exp_escaped, act_len, act_escaped);
                    free(exp_escaped);
                    free(act_escaped);
                }
                break;
            }
            case __CLOVE_GENERIC_PTR: {
                const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                const void* exp = test->issue.expected._ptr;
                const void* act = test->issue.actual._ptr;
                sprintf_s(msg, sizeof(msg), "%sexpected [%p] but was [%p]", non, exp, act);
                break;
            }
            default:
                break;
            }
        }
        printf("%s %s%s => %s@%d: %s\n", __CLOVE_ERRO, result, __CLOVE_FAILED, test->file_name, test->issue.line, msg);
    }
    else if (test->result == __CLOVE_TEST_SKIPPED) {
        printf("%s %s%s\n", __CLOVE_WARN, result, __CLOVE_SKIPPED);
    }
}


static void __clove_report_console_free(__clove_report_t* report) {
    free(report);
}

static __clove_report_console_t* __clove_report_console_new() {
    __clove_report_console_t* result = (__clove_report_console_t*)malloc(sizeof(__clove_report_console_t));
    result->base.start = __clove_report_console_start;
    result->base.end = __clove_report_console_end;
    result->base.test_executed = __clove_report_console_test_executed;
    result->base.free = __clove_report_console_free;
    return result;
}



static const char* __clove_get_exec_base_path();
static void __clove_replace_char(char* path, char find, char replace);

static char* __clove_path_concat(const char separator, const char* path1, const char* path2) {
    size_t count = strlen(path1) + 1 + strlen(path2) + 1;
    char* path = (char*)calloc(count, sizeof(char));

    strcat_s(path, count, path1);
    strncat_s(path, count, &separator, 1);
    strcat_s(path, count, path2);

    __clove_replace_char(path, '/', separator);
    __clove_replace_char(path, '\\', separator);

    return path;
}

static char* __clove_path_rel_to_abs_exec_path(const char* rel_path) {
    const char* base_path = __clove_get_exec_base_path();
    char* abs_path = __clove_path_concat(__CLOVE_PATH_SEPARATOR, base_path, rel_path);
    return abs_path;
}

static bool __clove_path_is_relative(const char* path) {    
    if (__clove_string_startswith(path, "\\")) return false; //windows
    if (strlen(path) > 2 && path[1] == ':') return false;    //windows
    if (__clove_string_startswith(path, "/")) return false;  //unix
    return true;
}

typedef struct __clove_report_json_t {
    __clove_report_t base;
    const char* file_path;
    const char* clove_version;
    unsigned int api_version;
    FILE* file;
    __clove_suite_t* current_suite;
    size_t test_count;
} __clove_report_json_t;


static void __clove_report_json_start(__clove_report_t* _this, int suite_count, int test_count) {
    __clove_report_json_t* instance = (__clove_report_json_t*)_this;

    fopen_s(&(instance->file), instance->file_path, "wb"); //binary mode so \n will stay \n (and not converted to \r\n on windows)
    if (instance->file == NULL) { printf("ERROR OPENING FILE: %s\n", instance->file_path); }

    fprintf(instance->file, "{\n");
    fprintf(instance->file, "\t\"clove_version\" : \"%s\",\n", instance->clove_version);
    fprintf(instance->file, "\t\"api_version\" : %u,\n", instance->api_version);
    fprintf(instance->file, "\t\"result\" : {\n");
    fprintf(instance->file, "\t\t\"suite_count\" : %d,\n", suite_count);
    fprintf(instance->file, "\t\t\"test_count\" : %d,\n", test_count);
    fprintf(instance->file, "\t\t\"suites\" : {\n");
}

static void __clove_report_json_end(__clove_report_t* _this, int test_count, int passed, int skipped, int failed) {
    __clove_report_json_t* instance = (__clove_report_json_t*)_this;

    int status = -1;
    if (passed == test_count) status = __CLOVE_TEST_PASSED;
    else if (failed > 0) status = __CLOVE_TEST_FAILED;
    else if (skipped > 0) status = __CLOVE_TEST_SKIPPED;

    fprintf(instance->file, "\t\t},\n"); //suites
    fprintf(instance->file, "\t\t\"test_passed\" : %d,\n", passed);
    fprintf(instance->file, "\t\t\"test_skipped\" : %d,\n", skipped);
    fprintf(instance->file, "\t\t\"test_failed\" : %d,\n", failed);
    fprintf(instance->file, "\t\t\"status\" : %d\n", status);
    fprintf(instance->file, "\t}\n"); //result
    fprintf(instance->file, "}"); //object

    fclose(instance->file);
}

#define __CLOVE_REPORT_JSON_PRINT_DATA(data) \
    if (test->issue.assert == __CLOVE_ASSERT_FAIL) {\
        fprintf(instance->file, "%s", "");\
    }\
    else {\
        switch(test->issue.data_type)\
        {\
        case __CLOVE_GENERIC_BOOL:\
            fprintf(instance->file, "%s", data._bool ? "true" : "false");\
            break;\
        case __CLOVE_GENERIC_CHAR: {\
            fprintf(instance->file, "%c", data._char);\
            break;\
        }\
        case __CLOVE_GENERIC_INT: {\
            fprintf(instance->file, "%d", data._int);\
            break;\
        }\
        case __CLOVE_GENERIC_UINT: {\
            fprintf(instance->file, "%u", data._uint);\
            break;\
        }\
        case __CLOVE_GENERIC_LONG: {\
            fprintf(instance->file, "%ld", data._long);\
            break;\
        }\
        case __CLOVE_GENERIC_ULONG: {\
            fprintf(instance->file, "%lu", data._ulong);\
            break;\
        }\
        case __CLOVE_GENERIC_LLONG: {\
            fprintf(instance->file, "%lld", data._llong);\
            break;\
        }\
        case __CLOVE_GENERIC_ULLONG: {\
            fprintf(instance->file, "%llu", data._ullong);\
            break;\
        }\
        case __CLOVE_GENERIC_FLOAT: {\
            fprintf(instance->file, "%f", data._float);\
            break;\
        }\
        case __CLOVE_GENERIC_DOUBLE: {\
            fprintf(instance->file, "%f", data._double);\
            break;\
        }\
        case __CLOVE_GENERIC_STRING: {\
            char* escaped = __clove_string_escape(data._string);\
            fprintf(instance->file, "%s", escaped);\
            free(escaped);\
            break;\
        }\
        case __CLOVE_GENERIC_PTR: {\
            fprintf(instance->file, "%p", data._ptr);\
            break;\
        }\
        default:\
            break;\
        }\
    }

static void __clove_report_json_test_executed(__clove_report_t* _this, __clove_suite_t* suite, __clove_test_t* test, size_t test_number) {
    __clove_report_json_t* instance = (__clove_report_json_t*)_this;
    //case for suites > 1
    if (instance->current_suite != NULL && instance->current_suite != suite) {
        fseek(instance->file, -1L, SEEK_CUR); //replacing "\n" with ",\n"
        fprintf(instance->file, ",\n");
    }

    if (instance->current_suite == NULL || instance->current_suite != suite) {
        char escaped_file[__CLOVE_STRING_LENGTH];
        strcpy_s(escaped_file, __CLOVE_STRING_LENGTH, test->file_name);
        __clove_replace_char(escaped_file, '\\', '/');

        fprintf(instance->file, "\t\t\t\"%s\" : {\n", suite->name);
        fprintf(instance->file, "\t\t\t\t\"_data\" : {\n");
        fprintf(instance->file, "\t\t\t\t\t\"file\" : \"%s\"\n", escaped_file);
        fprintf(instance->file, "\t\t\t\t},\n");
        instance->current_suite = suite;
        instance->test_count = 0;
    }

    instance->test_count++;
    fprintf(instance->file, "\t\t\t\t\"%s\" : {\n", test->name);
    fprintf(instance->file, "\t\t\t\t\t\"status\" : %d,\n", test->result);
    fprintf(instance->file, "\t\t\t\t\t\"duration\" : %llu", __clove_time_to_nanos(&(test->duration)));
    if (test->result == __CLOVE_TEST_FAILED) {
        fprintf(instance->file, ",\n");
        fprintf(instance->file, "\t\t\t\t\t\"line\" : %u,\n", test->issue.line);
        fprintf(instance->file, "\t\t\t\t\t\"assert\" : %d,\n", test->issue.assert);
        fprintf(instance->file, "\t\t\t\t\t\"type\" : %d,\n", test->issue.data_type);
        fprintf(instance->file, "\t\t\t\t\t\"expected\" : \"");
        __CLOVE_REPORT_JSON_PRINT_DATA(test->issue.expected);
        fprintf(instance->file, "\",\n");
        fprintf(instance->file, "\t\t\t\t\t\"actual\" : \"");
        __CLOVE_REPORT_JSON_PRINT_DATA(test->issue.actual);
        fprintf(instance->file, "\"\n");
    }
    else {
        fprintf(instance->file, "\n");
    }
    fprintf(instance->file, "\t\t\t\t}");


    if (instance->test_count < suite->test_count) {
        fprintf(instance->file, ",\n");
    }
    else {
        fprintf(instance->file, "\n");
        fprintf(instance->file, "\t\t\t}\n"); //close suite
    }
}

static void __clove_report_json_free(__clove_report_t* report) {
    free(report);
}

static __clove_report_json_t* __clove_report_json_new(const char* file_path, const char* clove_version) {
    __clove_report_json_t* result = (__clove_report_json_t*)malloc(sizeof(__clove_report_json_t));
    result->base.start = __clove_report_json_start;
    result->base.end = __clove_report_json_end;
    result->base.test_executed = __clove_report_json_test_executed;
    result->base.free = __clove_report_json_free;
    result->file_path = file_path;
    result->clove_version = clove_version;
    result->api_version = 1;
    result->current_suite = NULL;
    result->test_count = 0;
    return result;
}



#define __CLOVE_ASSERTION_CHECK(mode, exp, act, type, field, test) \
    bool pass_scenario = false;\
    if (check_mode == __CLOVE_ASSERT_EQ) { pass_scenario = exp == act; }\
    else if (check_mode == __CLOVE_ASSERT_NE) { pass_scenario = exp != act; }\
    if (pass_scenario) _this->result =  __CLOVE_TEST_PASSED;\
    else { \
        _this->result =  __CLOVE_TEST_FAILED;\
        _this->issue.assert = mode;\
        _this->issue.data_type = type;\
        _this->issue.expected.field = exp;\
        _this->issue.actual.field = act;\
    }

static void __clove_check_fail(__clove_test_t* _this) {
    _this->result = __CLOVE_TEST_FAILED;
    _this->issue.assert = __CLOVE_ASSERT_FAIL;
}

static void __clove_check_pass(__clove_test_t* _this) {
    _this->result = __CLOVE_TEST_PASSED;
}

static void __clove_check_int(__clove_assert_check_e check_mode, int expected, int result, __clove_test_t* _this) {
    __CLOVE_ASSERTION_CHECK(check_mode, expected, result, __CLOVE_GENERIC_INT, _int, _this)
}

static void __clove_check_uint(__clove_assert_check_e check_mode, unsigned int expected, unsigned int result, __clove_test_t* _this) {
    __CLOVE_ASSERTION_CHECK(check_mode, expected, result, __CLOVE_GENERIC_UINT, _uint, _this)
}

static void __clove_check_long(__clove_assert_check_e check_mode, long expected, long result, __clove_test_t* _this) {
    __CLOVE_ASSERTION_CHECK(check_mode, expected, result, __CLOVE_GENERIC_LONG, _long, _this)
}

static void __clove_check_ulong(__clove_assert_check_e check_mode, unsigned long expected, unsigned long result, __clove_test_t* _this) {
    __CLOVE_ASSERTION_CHECK(check_mode, expected, result, __CLOVE_GENERIC_ULONG, _ulong, _this)
}

static void __clove_check_llong(__clove_assert_check_e check_mode, long long expected, long long result, __clove_test_t* _this) {
    __CLOVE_ASSERTION_CHECK(check_mode, expected, result, __CLOVE_GENERIC_LLONG, _llong, _this)
}

static void __clove_check_ullong(__clove_assert_check_e check_mode, unsigned long long expected, unsigned long long result, __clove_test_t* _this) {
    __CLOVE_ASSERTION_CHECK(check_mode, expected, result, __CLOVE_GENERIC_ULLONG, _ullong, _this)
}

static void __clove_check_char(__clove_assert_check_e check_mode, char expected, char result, __clove_test_t* _this) {
    __CLOVE_ASSERTION_CHECK(check_mode, expected, result, __CLOVE_GENERIC_CHAR, _char, _this)
}

static void __clove_check_bool(__clove_assert_check_e check_mode, bool expected, bool result, __clove_test_t* _this) {
    __CLOVE_ASSERTION_CHECK(check_mode, expected, result, __CLOVE_GENERIC_BOOL, _bool, _this)
}

static void __clove_check_null(__clove_assert_check_e check_mode, void* expected, void* result, __clove_test_t* _this) {
    __CLOVE_ASSERTION_CHECK(check_mode, expected, result, __CLOVE_GENERIC_PTR, _ptr, _this)
}

static void __clove_check_ptr(__clove_assert_check_e check_mode, void* expected, void* result, __clove_test_t* _this) {
    __CLOVE_ASSERTION_CHECK(check_mode, expected, result, __CLOVE_GENERIC_PTR, _ptr, _this)
}

static void __clove_check_float(__clove_assert_check_e check_mode, float expected, float result, __clove_test_t* _this) {
    bool pass_scenario = false;
    if (check_mode == __CLOVE_ASSERT_EQ) { pass_scenario = (fabsf(expected - result) <= __CLOVE_FLOATING_PRECISION); }
    else if (check_mode == __CLOVE_ASSERT_NE) { pass_scenario = (fabsf(expected - result) > __CLOVE_FLOATING_PRECISION); }

    if (pass_scenario) {
        _this->result = __CLOVE_TEST_PASSED;
    }
    else {
        _this->result = __CLOVE_TEST_FAILED;
        _this->issue.assert = check_mode;
        _this->issue.data_type = __CLOVE_GENERIC_FLOAT;
        _this->issue.expected._float = expected;
        _this->issue.actual._float = result;
    }
}

static void __clove_check_double(__clove_assert_check_e check_mode, double expected, double result, __clove_test_t* _this) {
    bool pass_scenario = false;
    if (check_mode == __CLOVE_ASSERT_EQ) { pass_scenario = ((float)fabs(expected - result) <= __CLOVE_FLOATING_PRECISION); }
    else if (check_mode == __CLOVE_ASSERT_NE) { pass_scenario = ((float)fabs(expected - result) > __CLOVE_FLOATING_PRECISION); }

    if (pass_scenario) {
        _this->result = __CLOVE_TEST_PASSED;
    }
    else {
        _this->result = __CLOVE_TEST_FAILED;
        _this->issue.assert = check_mode;
        _this->issue.data_type = __CLOVE_GENERIC_DOUBLE;
        _this->issue.expected._double = expected;
        _this->issue.actual._double = result;
    }
}

static void __clove_check_string(__clove_assert_check_e check_mode, const char* expected, const char* result, __clove_test_t* _this) {
    bool pass_scenario = false;
    if (check_mode == __CLOVE_ASSERT_EQ) { pass_scenario = strcmp(expected, result) == 0; }
    else if (check_mode == __CLOVE_ASSERT_NE) { pass_scenario = strcmp(expected, result) != 0; }

    if (pass_scenario) {
        _this->result = __CLOVE_TEST_PASSED;
    }
    else {
        _this->result = __CLOVE_TEST_FAILED;
        _this->issue.assert = check_mode;
        _this->issue.data_type = __CLOVE_GENERIC_STRING;
        _this->issue.expected._string = _strdup(expected); //in case is allocated on the stack within the test function
        _this->issue.actual._string = _strdup(result); //in case is allocated on the stack within the test function
        //TODO: free expected/actual in case of __CLOVE_GENERIC_STRING
    }
}

static const char* __clove_rel_src(const char* path) {
    //https://stackoverflow.com/questions/9834067/difference-between-char-and-const-char
    const char* subpath = strstr(path, "\\src");
    if (subpath == NULL) subpath = strstr(path, "\\tests");
    if (subpath == NULL) return path;
    return subpath + 1;
}

static void __clove_exec_suite(__clove_suite_t* suite, size_t test_counter, unsigned int* passed, unsigned int* failed, unsigned int* skipped, __clove_report_t* report) {
    __clove_time_t suite_start = __clove_time_now();
    suite->fixtures.setup_once();

    for (size_t i = 0; i < suite->test_count; i++) {
        __clove_test_t* each_test = (__clove_test_t*)__clove_vector_get(&suite->tests, i);
        each_test->result = __CLOVE_TEST_SKIPPED;

        __clove_time_t test_start = __clove_time_now();
        suite->fixtures.setup();
        each_test->funct(each_test);
        suite->fixtures.teardown();
        __clove_time_t test_end = __clove_time_now();
        each_test->duration = __clove_time_sub(&test_end, &test_start);

        switch (each_test->result) {
        case __CLOVE_TEST_PASSED: {
            (*passed)++;
            break;
        }
        case __CLOVE_TEST_FAILED: {
            (*failed)++;
            break;
        }
        case __CLOVE_TEST_SKIPPED: {
            (*skipped)++;
            break;
        }
        default:
            break;
        }
        report->test_executed(report, suite, each_test, test_counter + i);
    }
    suite->fixtures.teardown_once();
    __clove_time_t suite_end = __clove_time_now();
    suite->issue.duration = __clove_time_sub(&suite_end, &suite_start);
}

static void __clove_exec_suites(__clove_suite_t* suites, int suite_count, int test_count, __clove_report_t* report) {
    report->start(report, suite_count, test_count);

    unsigned int passed = 0;
    unsigned int failed = 0;
    unsigned int skipped = 0;

    size_t test_start_counter = 1;
    for (int i = 0; i < suite_count; ++i) {
        __clove_suite_t* each_suite = &suites[i];
        __clove_exec_suite(each_suite, test_start_counter, &passed, &failed, &skipped, report);
        test_start_counter += each_suite->test_count;
    }
    report->end(report, test_count, passed, skipped, failed);
}

#define __CLOVE_TEST_GUARD \
    if (_this->result == __CLOVE_TEST_FAILED) { return; }\
    if (_this->file_name[0] == '\0') strcpy_s(_this->file_name, __CLOVE_STRING_LENGTH, __clove_rel_src(__FILE__));\
    _this->issue.line=__LINE__;

#ifdef _WIN32
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

static char* __clove_path_basepath(char* path) {
    //make sure path contains only separator specific for the OS
#ifdef _WIN32
    __clove_replace_char(path, '/', __CLOVE_PATH_SEPARATOR);
#else
    __clove_replace_char(path, '\\', __CLOVE_PATH_SEPARATOR);
#endif //_WIN32

    const char* last_addr = strrchr((const char*)path, __CLOVE_PATH_SEPARATOR);
    int bytes_count;

    char dot_path[3] = { '.', __CLOVE_PATH_SEPARATOR, '\0' };
    if (!last_addr) {
        bytes_count = sizeof(dot_path) - 1; //equivalent to strlen
        path = dot_path;
    }
    else {
        bytes_count = (int)(last_addr - path);
    }
    int count = bytes_count + 1; // +1 take into account null terminator

    char* base_path = (char*)calloc(count, sizeof(char));
    strncpy_s(base_path, count, path, bytes_count);
    return base_path;
}

#pragma region Private APIs - Manual
#define __CLOVE_RUNNER_MANUAL(...) \
char* __clove_exec_path;\
char* __clove_exec_base_path;\
int main(int argc, char* argv[]) {\
    __clove_exec_path = argv[0]; \
    __clove_exec_base_path = __clove_path_basepath(argv[0]); \
    static void (*suite_ptr[])(__clove_suite_t*) = {__VA_ARGS__};\
    int suite_count = sizeof(suite_ptr) / sizeof(suite_ptr[0]); \
    __clove_vector_params_t vector_params = __clove_vector_params_defaulted(sizeof(__clove_suite_t)); \
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
    __clove_report_t* report = (__clove_report_t*)__clove_report_console_new(); \
    __clove_exec_suites((__clove_suite_t*)(suites.items), suite_count, test_count, report);\
    report->free(report);\
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
    __clove_vector_params_t vector_params = __clove_vector_params_defaulted(sizeof(__clove_test_t)); \
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
    static const char* end_suite_separator = "___";
    static int end_suite_separator_length = 3;
    static const char* test_separator = "20_";
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
    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(__clove_symbols_function_t));
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


#pragma region PRIVATE APIs - CommandLine
typedef struct __clove_cmdline_t {
    int arg_index;
    int argc;
    char* *argv;
} __clove_cmdline_t;

static __clove_cmdline_t __clove_cmdline_create(char** argv, int argc) {
    __clove_cmdline_t cmd;
    cmd.argv = argv;
    cmd.argc = argc;
    cmd.arg_index = 1;
    return cmd;
}

static bool __clove_cmdline_next_opt(__clove_cmdline_t* cmdline, char** opt_out) {
    if (cmdline->arg_index >= cmdline->argc) return false;
    
    char* current = cmdline->argv[cmdline->arg_index];
    if (strlen(current) == 2 && __clove_string_startswith(current, "-")) {
        *opt_out = current+1;
    }
    cmdline->arg_index++;
    return true;
}

static bool __clove_cmdline_next_arg(__clove_cmdline_t* cmdline, char** arg_out) {
    if (cmdline->arg_index >= cmdline->argc) return false;
    *arg_out = cmdline->argv[cmdline->arg_index];
    cmdline->arg_index++;
    return true;
}

#pragma endregion

static int __clove_run_tests_with_report(__clove_report_t* report) {
    int run_result = 0;

    __clove_symbols_context_t context;
    
    __clove_vector_params_t vector_params = __clove_vector_params_defaulted(sizeof(__clove_suite_t));
    vector_params.item_ctor = __clove_vector_suite_ctor;
    vector_params.item_dtor = __clove_vector_suite_dtor;
    __clove_vector_init(&context.suites, &vector_params);
    context.suites_count = 0;
    context.last_suite = NULL;
    context.prefix_length = strlen("__clove_sym___");
    context.tests_count = 0;
    
    int result = __clove_symbols_for_each_function_by_prefix("__clove_sym___", __clove_symbols_function_collect, &context);
    if (result == 0) {
        __clove_exec_suites((__clove_suite_t*)(context.suites.items), context.suites_count, context.tests_count, report);
    } else {
        run_result = -1;
    }
    __clove_vector_free(&context.suites);
    return run_result;
}


typedef enum __clove_cmdline_errno_t {
    __CLOVE_CMD_ERRNO_OK = 0,
    __CLOVE_CMD_ERRNO_GENERIC = 1,
    __CLOVE_CMD_ERRNO_INVALID_PARAM = 2,
} __clove_cmdline_errno_t;

static __clove_cmdline_errno_t __clove_cmdline_handle_version(__clove_cmdline_t* ctx) {
    printf("%s", __CLOVE_VERSION); //to avoid new_line character(s)
    return __CLOVE_CMD_ERRNO_OK;
}

static __clove_cmdline_errno_t __clove_cmdline_handle_report(__clove_cmdline_t* cmdline) {
    char* arg;
    bool found = __clove_cmdline_next_arg(cmdline, &arg);
    if (!found) return __CLOVE_CMD_ERRNO_INVALID_PARAM;

    __clove_report_t* report;
    if (__clove_string_equal("json", arg)) {
        char* file_path = "clove_report.json";
        found = __clove_cmdline_next_opt(cmdline, &arg);
        if (found && __clove_string_equal("f", arg)) {
            found = __clove_cmdline_next_arg(cmdline, &arg);
            if (found) file_path = arg;
        }
        char* report_path;
        if (__clove_path_is_relative(file_path)) {
            report_path = __clove_path_rel_to_abs_exec_path(file_path);
        } else {
            report_path = file_path;
        }
        report = (__clove_report_t*)__clove_report_json_new(report_path, __CLOVE_VERSION);
    } else if (__clove_string_equal("console", arg)) {
        report = (__clove_report_t* )__clove_report_console_new();
    } else {
        return __CLOVE_CMD_ERRNO_INVALID_PARAM;
    }

    int run_result = __clove_run_tests_with_report(report);
    report->free(report);

    if (run_result == 0) {
        return __CLOVE_CMD_ERRNO_OK;
    } 
    return __CLOVE_CMD_ERRNO_GENERIC;
}

static int __clove_runner_auto(int argc, char* argv[]) {
    __clove_exec_path = argv[0];
    __clove_exec_base_path = __clove_path_basepath(argv[0]);


    /* Supported Commands 
       > <exe>                      Run with console report
       > <exe> -r console           Run with console report (to implement: -m verbose or brief)
       > <exe> -r json              Run with json mode (to implement: -f <report-path.json>)
       > <exe> -v                   Print CLove-Unit version
    */
    __clove_cmdline_t cmdline = __clove_cmdline_create(argv, argc);
    __clove_cmdline_errno_t cmd_result = __CLOVE_CMD_ERRNO_OK;
    char* opt;
    bool found_opt = __clove_cmdline_next_opt(&cmdline, &opt);
    if (found_opt) {
        if (__clove_string_equal("v", opt)) {
            cmd_result = __clove_cmdline_handle_version(&cmdline);
        } else if (__clove_string_equal("r", opt)) {
            cmd_result = __clove_cmdline_handle_report(&cmdline);
        }
    } else {
        __clove_report_t* report = (__clove_report_t* )__clove_report_console_new();
        int run_result = __clove_run_tests_with_report(report);
        report->free(report);
        if (run_result != 0) cmd_result = __CLOVE_CMD_ERRNO_GENERIC;
    }
    free(__clove_exec_base_path);
    return cmd_result;
}


#define __CLOVE_RUNNER_AUTO() \
char* __clove_exec_path;\
char* __clove_exec_base_path;\
int main(int argc, char* argv[]) {\
    return __clove_runner_auto(argc, argv);\
}

static const char* __clove_get_exec_path() {
    return __clove_exec_path;
}
static const char* __clove_get_exec_base_path() {
    return __clove_exec_base_path;
}

#ifdef __cplusplus
#define __CLOVE_API_EXTERN extern "C"
#else
#define __CLOVE_API_EXTERN
#endif //__cplusplus

#ifdef _WIN32
#define __CLOVE_API_EXPORT __CLOVE_API_EXTERN __declspec(dllexport)
#else 
#define __CLOVE_API_EXPORT __CLOVE_API_EXTERN
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
#define CLOVE_PASS() __CLOVE_TEST_GUARD __clove_check_pass(_this);
#define CLOVE_FAIL() __CLOVE_TEST_GUARD __clove_check_fail(_this);
#define CLOVE_IS_TRUE(res) __CLOVE_TEST_GUARD __clove_check_bool(__CLOVE_ASSERT_EQ, true, res, _this);
#define CLOVE_IS_FALSE(res) __CLOVE_TEST_GUARD __clove_check_bool(__CLOVE_ASSERT_EQ, false, res, _this);
#define CLOVE_CHAR_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_char(__CLOVE_ASSERT_EQ, exp, res, _this);
#define CLOVE_CHAR_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_char(__CLOVE_ASSERT_NE, exp, res, _this);
#define CLOVE_INT_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_int(__CLOVE_ASSERT_EQ, exp, res, _this);
#define CLOVE_INT_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_int(__CLOVE_ASSERT_NE, exp, res, _this);
#define CLOVE_UINT_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_uint(__CLOVE_ASSERT_EQ, exp, res, _this);
#define CLOVE_UINT_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_uint(__CLOVE_ASSERT_NE, exp, res, _this);
#define CLOVE_LONG_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_long(__CLOVE_ASSERT_EQ, exp, res, _this);
#define CLOVE_LONG_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_long(__CLOVE_ASSERT_NE, exp, res, _this);
#define CLOVE_LLONG_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_llong(__CLOVE_ASSERT_EQ, exp, res, _this);
#define CLOVE_LLONG_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_llong(__CLOVE_ASSERT_NE, exp, res, _this);
#define CLOVE_ULONG_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_ulong(__CLOVE_ASSERT_EQ, exp, res, _this);
#define CLOVE_ULONG_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_ulong(__CLOVE_ASSERT_NE, exp, res, _this);
#define CLOVE_ULLONG_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_ullong(__CLOVE_ASSERT_EQ, exp, res, _this);
#define CLOVE_ULLONG_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_ullong(__CLOVE_ASSERT_NE, exp, res, _this);
#define CLOVE_FLOAT_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_float(__CLOVE_ASSERT_EQ, exp, res, _this);
#define CLOVE_FLOAT_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_float(__CLOVE_ASSERT_NE, exp, res, _this);
#define CLOVE_DOUBLE_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_double(__CLOVE_ASSERT_EQ, exp, res, _this);
#define CLOVE_DOUBLE_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_double(__CLOVE_ASSERT_NE, exp, res, _this);
#define CLOVE_NULL(res) __CLOVE_TEST_GUARD __clove_check_null(__CLOVE_ASSERT_EQ, NULL, (void*)res, _this);
#define CLOVE_NOT_NULL(res) __CLOVE_TEST_GUARD __clove_check_null(__CLOVE_ASSERT_NE, NULL, (void*)res, _this);
#define CLOVE_PTR_EQ(p1, p2) __CLOVE_TEST_GUARD __clove_check_ptr(__CLOVE_ASSERT_EQ, (void*)p1, (void*)p2, _this);
#define CLOVE_PTR_NE(p1, p2) __CLOVE_TEST_GUARD __clove_check_ptr(__CLOVE_ASSERT_NE, (void*)p1, (void*)p2, _this);
#define CLOVE_STRING_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_string(__CLOVE_ASSERT_EQ, exp, res, _this);
#define CLOVE_STRING_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_string(__CLOVE_ASSERT_NE, exp, res, _this);
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

#endif //__CLOVE_H