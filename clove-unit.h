/*
 * clove-unit
 * v2.4.2
 * Single-Header Unit Testing library for C/C++
 * https://github.com/fdefelici/clove-unit
 *
 */
#ifndef __CLOVE_H
#define __CLOVE_H

#define __CLOVE_VERSION_MAJOR 2
#define __CLOVE_VERSION_MINOR 4
#define __CLOVE_VERSION_PATCH 2
#define __CLOVE_VERSION "2.4.2"

//Preventing "unknown-pragmas" warning on GCC <= 12 for '#pragma region' usage
//NOTE1: GCC and G++ v13+ support '#pragma region' by the way.
//NOTE2: This workaround doesn't work for G++ version <= 12 because of a bug 
//       that has been addressed only in G++ v13 (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=53431)
#if defined(__GNUC__) && __GNUC__ <= 12 // GNU Unknown Pragma Fix: START
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif 

#pragma region PRIVATE

#pragma region PRIVATE - Target Init
#ifdef __linux
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif //_GNU_SOURCE
#endif //__linux

#ifdef __cplusplus
#define __CLOVE_EXTERN_C extern "C"
#else
#define __CLOVE_EXTERN_C
#endif //__cplusplus
#pragma endregion // Target Init

#pragma region PRIVATE - DECLARATION
#pragma region PRIVATE - Utils Decl
#include <stdio.h>
__CLOVE_EXTERN_C void  __clove_utils_empty_funct(void);

extern char* __clove_exec_path;
extern char* __clove_exec_base_path;
__CLOVE_EXTERN_C const char* __clove_get_exec_base_path(void);
__CLOVE_EXTERN_C const char* __clove_get_exec_path(void);

//Switch implementation for pointer types
#define __CLOVE_SWITCH_BEG(X) \
    { \
        size_t* expr = (size_t*)(size_t)X; \
        if (false)  { }
#define __CLOVE_SWITCH_CASE(Y) \
    else if (expr == (size_t*)(size_t)Y)
#define __CLOVE_SWITCH_DEFAULT() \
    else 
#define __CLOVE_SWITCH_END() }

//Custom suppressing "unused parameter" warnings for multi-compilers
#define __CLOVE_UNUSED_VAR(x) (void)(x)

#define __CLOVE_MACRO_COMBINE_INTERNAL(A, B) A##B
#define __CLOVE_MACRO_COMBINE(A, B) __CLOVE_MACRO_COMBINE_INTERNAL(A, B)
#pragma endregion // Utils Decl

#pragma region PRIVATE - Math Decl
//mainly math header operations re-implemented to avoid explicit linking to math library on unix os (with option -lm).
__CLOVE_EXTERN_C unsigned int __clove_math_powi(unsigned int base, unsigned int exp);
__CLOVE_EXTERN_C float __clove_math_decimalf(unsigned char precision);
__CLOVE_EXTERN_C double __clove_math_decimald(unsigned char precision);
#pragma endregion

#pragma region PRIVATE - Path Decl
#include <stdbool.h>

#ifdef _WIN32
#define __CLOVE_PATH_SEPARATOR '\\'
#define __CLOVE_PATH_SEPARATOR_STR "\\"
#else 
#define __CLOVE_PATH_SEPARATOR '/'
#define __CLOVE_PATH_SEPARATOR_STR "/"
#endif //_WIN32

__CLOVE_EXTERN_C char* __clove_path_concat(const char separator, const char* path1, const char* path2);
__CLOVE_EXTERN_C const char* __clove_path_relative(const char* abs_path, const char* base_path);
__CLOVE_EXTERN_C char* __clove_path_rel_to_abs_exec_path(const char* rel_path);
__CLOVE_EXTERN_C bool __clove_path_is_relative(const char* path);
__CLOVE_EXTERN_C void __clove_path_to_os(char* path);
__CLOVE_EXTERN_C char* __clove_path_basepath(const char* path);
#pragma endregion // Path Decl

#pragma region PRIVATE - Memory Decl
__CLOVE_EXTERN_C void* __clove_memory_malloc(size_t size);
__CLOVE_EXTERN_C void* __clove_memory_calloc(size_t size);
__CLOVE_EXTERN_C void* __clove_memory_realloc(void* source, size_t size);
__CLOVE_EXTERN_C bool __clove_memory_memcpy(void* dest, size_t dest_size, const void* src, size_t src_size);
__CLOVE_EXTERN_C bool __clove_memory_memset(void* dest, size_t size, unsigned char value);

#define __CLOVE_MEMORY_MALLOC_TYPE_N(TYPE, COUNT) (TYPE*)__clove_memory_malloc(sizeof(TYPE) * COUNT)
#define __CLOVE_MEMORY_MALLOC_TYPE(TYPE) __CLOVE_MEMORY_MALLOC_TYPE_N(TYPE, 1)

#define __CLOVE_MEMORY_CALLOC_TYPE_N(TYPE, COUNT) (TYPE*)__clove_memory_calloc(sizeof(TYPE) * COUNT)
#define __CLOVE_MEMORY_CALLOC_TYPE(TYPE) __CLOVE_MEMORY_CALLOC_TYPE_N(TYPE, 1)
#pragma endregion //Memory Decl

#pragma region PRIVATE - Console Decl
#include <stdarg.h>
__CLOVE_EXTERN_C void __clove_console_printf(const char* format, ...);
__CLOVE_EXTERN_C void __clove_console_vprintf(const char* format, va_list args);
__CLOVE_EXTERN_C void __clove_console_write(const char* str);
__CLOVE_EXTERN_C void __clove_console_writeline(const char* str);
#pragma endregion //Console Decl

#pragma region PRIVATE - File Decl
#include <stdarg.h>
__CLOVE_EXTERN_C FILE* __clove_file_open(const char* path, const char* mode);
__CLOVE_EXTERN_C void __clove_file_close(FILE* file);
__CLOVE_EXTERN_C void __clove_file_printf(FILE* file, const char* format, ...);
__CLOVE_EXTERN_C void __clove_file_vprintf(FILE* file, const char* format, va_list args);
__CLOVE_EXTERN_C void __clove_file_write(FILE* file, const char* str);
__CLOVE_EXTERN_C void __clove_file_writeline(FILE* file, const char* str);
#pragma endregion //File Decl

#pragma region PRIVATE - String Decl
#include <stdbool.h>
__CLOVE_EXTERN_C bool __clove_string_equal(const char* str1, const char* str2);
__CLOVE_EXTERN_C bool __clove_string_equal_any(const char* str1, size_t count, ...);
__CLOVE_EXTERN_C bool __clove_string_startswith(const char* str1, const char* prefix);
__CLOVE_EXTERN_C bool __clove_string_endswith(const char* str1, const char* suffix);
__CLOVE_EXTERN_C bool __clove_string_strncmp(const char* str1, const char* str2, size_t count);
__CLOVE_EXTERN_C bool __clove_string_strcpy(char* dest, size_t dest_size, const char* source);
__CLOVE_EXTERN_C bool __clove_string_strncpy(char* dest, size_t dest_size, const char* source, size_t count);
__CLOVE_EXTERN_C bool __clove_string_strcat(char* dest, size_t dest_size, const char* source);
__CLOVE_EXTERN_C bool __clove_string_strncat(char* dest, size_t dest_size, const char* source, size_t count);
__CLOVE_EXTERN_C char* __clove_string_strdup(const char* str);
__CLOVE_EXTERN_C void __clove_string_sprintf(char* dest, size_t dest_size, const char* format, ...);
__CLOVE_EXTERN_C void __clove_string_vsprintf(char* dest, size_t dest_size, const char* format, va_list args);
__CLOVE_EXTERN_C size_t __clove_string_length(const char* str);
__CLOVE_EXTERN_C const char* __clove_string_strstr(const char* str1, const char* str2);
__CLOVE_EXTERN_C bool __clove_string_contains(const char* string, const char* contained);
__CLOVE_EXTERN_C char* __clove_string_escape(const char* string);
__CLOVE_EXTERN_C char* __clove_string_csv_escape(const char* string);
__CLOVE_EXTERN_C void __clove_string_ellipse(const char* string, size_t str_len, size_t pos, char* out, size_t out_size);
__CLOVE_EXTERN_C void __clove_string_replace_char(char* path, char find, char replace);
#pragma endregion // String Decl

#pragma region PRIVATE - String View Decl
#include <stdbool.h>
typedef struct __clove_string_view_t  {
    const char* begin;
    size_t length;
} __clove_string_view_t;

__CLOVE_EXTERN_C __clove_string_view_t __clove_string_view_from_offs(const char* source, size_t begin_offset, size_t end_offset);
__CLOVE_EXTERN_C __clove_string_view_t __clove_string_view_from_be(const char* begin, const char* end);
__CLOVE_EXTERN_C __clove_string_view_t __clove_string_view_from_len(const char* begin, size_t length);
__CLOVE_EXTERN_C __clove_string_view_t __clove_string_view_from_str(const char* str);
__CLOVE_EXTERN_C size_t __clove_string_view_length(const __clove_string_view_t* view);
__CLOVE_EXTERN_C bool __clove_string_view_equals(const __clove_string_view_t* view1, const __clove_string_view_t* view2);
__CLOVE_EXTERN_C bool __clove_string_view_ncmp(const __clove_string_view_t* view1, const __clove_string_view_t* view2, size_t count);
__CLOVE_EXTERN_C bool __clove_string_view_endswith(const __clove_string_view_t* view, const __clove_string_view_t* suffix);
__CLOVE_EXTERN_C bool __clove_string_view_nendswith(const __clove_string_view_t* view, const __clove_string_view_t* suffix, size_t suffix_begin_offset);
__CLOVE_EXTERN_C bool __clove_string_view_strequals(const __clove_string_view_t* view, const char* str);
__CLOVE_EXTERN_C const char* __clove_string_view_begin(const __clove_string_view_t* view);
__CLOVE_EXTERN_C const char* __clove_string_view_end(const __clove_string_view_t* view);
__CLOVE_EXTERN_C char* __clove_string_view_as_string(const __clove_string_view_t* view);
__CLOVE_EXTERN_C char __clove_string_view_at(const __clove_string_view_t* view, size_t index);
__CLOVE_EXTERN_C bool __clove_string_view_contains(const __clove_string_view_t* view, const __clove_string_view_t* fixture);
#pragma endregion // String View Decl

#pragma region PRIVATE - Time Decl
typedef enum __clove_time_traslation_e {
    __CLOVE_TIME_TRASL_NANOS_PER_SEC = 1000000000, //1 BILLION
    __CLOVE_TIME_TRASL_MILLIS_PER_SEC = 1000,
    __CLOVE_TIME_TRASL_NANOS_PER_MILLIS = 1000000 //1 MILION
} __clove_time_traslation_e;

typedef struct __clove_time_t {
    long long seconds;
    long long nanos_after_seconds;
} __clove_time_t;

__CLOVE_EXTERN_C __clove_time_t __clove_time_now(void);
__CLOVE_EXTERN_C __clove_time_t __clove_time_sub(__clove_time_t* t1, __clove_time_t* t2);
__CLOVE_EXTERN_C __clove_time_t __clove_time_sum(__clove_time_t* t1, __clove_time_t* t2);
__CLOVE_EXTERN_C unsigned long long __clove_time_to_millis(__clove_time_t* t);
__CLOVE_EXTERN_C unsigned long long __clove_time_to_nanos(__clove_time_t* t);
#pragma endregion // Time Decl

#pragma region PRIVATE - Stack Decl
//Stack not generalized. By now just managing size_t items for implenting Iterative QuickSort 
typedef struct __clove_stack_t {
    unsigned char* items;
    size_t capacity;
    size_t count;
    size_t item_size;
} __clove_stack_t;

__CLOVE_EXTERN_C void __clove_stack_init(__clove_stack_t* stack, size_t initial_capacity);
__CLOVE_EXTERN_C bool __clove_stack_is_empty(__clove_stack_t* stack);
__CLOVE_EXTERN_C void __clove_stack_push(__clove_stack_t* stack, size_t item);
__CLOVE_EXTERN_C size_t __clove_stack_pop(__clove_stack_t* stack);
__CLOVE_EXTERN_C void __clove_stack_free(__clove_stack_t* stack);
#pragma endregion // Stack Decl

#pragma region PRIVATE - Vector Decl
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

__CLOVE_EXTERN_C __clove_vector_params_t __clove_vector_params_defaulted(size_t item_size);
__CLOVE_EXTERN_C __clove_vector_t __clove_vector_null(void);
__CLOVE_EXTERN_C void __clove_vector_init(__clove_vector_t* vector, __clove_vector_params_t* params);
__CLOVE_EXTERN_C size_t __clove_vector_count(const __clove_vector_t* vector);
__CLOVE_EXTERN_C bool __clove_vector_is_empty(const __clove_vector_t* vector);
__CLOVE_EXTERN_C void* __clove_vector_add_slot(__clove_vector_t* vector);
__CLOVE_EXTERN_C void __clove_vector_add_all(__clove_vector_t* vector, const __clove_vector_t* other);
__CLOVE_EXTERN_C void* __clove_vector_get(const __clove_vector_t* vector, size_t index);
__CLOVE_EXTERN_C void __clove_vector_set(__clove_vector_t* vector, size_t index, void* item);
__CLOVE_EXTERN_C void __clove_vector_free(__clove_vector_t* vector);
__CLOVE_EXTERN_C void __clove_vector_swap(__clove_vector_t* vector, size_t index1, size_t index2);
__CLOVE_EXTERN_C size_t __clove_vector_quicksort_partition(__clove_vector_t* vector, int (*comparator)(void*, void*), size_t start_index, size_t end_index);
__CLOVE_EXTERN_C void __clove_vector_quicksort_iterative(__clove_vector_t* vector, int (*comparator)(void*, void*), size_t start_index, size_t end_index);
__CLOVE_EXTERN_C void __clove_vector_sort(__clove_vector_t* vector, int (*comparator)(void*, void*));
__CLOVE_EXTERN_C void __clove_vector_collection_dtor(void* vector);

#define __CLOVE_VECTOR_INIT(VECTOR_PTR, TYPE) \
    __clove_vector_params_t __CLOVE_MACRO_COMBINE(params,__LINE__) = __clove_vector_params_defaulted(sizeof(TYPE)); \
    __clove_vector_init(VECTOR_PTR, &__CLOVE_MACRO_COMBINE(params,__LINE__));

#define __CLOVE_VECTOR_INIT_CAPACITY(VECTOR_PTR, TYPE, CAPACITY) \
    __clove_vector_params_t __CLOVE_MACRO_COMBINE(params,__LINE__) = __clove_vector_params_defaulted(sizeof(TYPE)); \
    __CLOVE_MACRO_COMBINE(params,__LINE__).initial_capacity = CAPACITY; \
    __clove_vector_init(VECTOR_PTR, &__CLOVE_MACRO_COMBINE(params,__LINE__));

#define __CLOVE_VECTOR_INIT_PARAMS(VECTOR_PTR, PARAMS) \
    __clove_vector_params_t __CLOVE_MACRO_COMBINE(params,__LINE__) = PARAMS; \
    __clove_vector_init(VECTOR_PTR, &__CLOVE_MACRO_COMBINE(params,__LINE__));

#define __CLOVE_VECTOR_ADD(VECTOR_PTR, TYPE, ITEM) \
    TYPE* __CLOVE_MACRO_COMBINE(__vector_slot,__LINE__) = (TYPE*)__clove_vector_add_slot(VECTOR_PTR); \
    *__CLOVE_MACRO_COMBINE(__vector_slot,__LINE__) = ITEM;

#define __CLOVE_VECTOR_FOREACH(VECTOR_PTR, TYPE, NAME, BODY) \
    for(size_t __CLOVE_MACRO_COMBINE(vector_index,__LINE__)=0; __CLOVE_MACRO_COMBINE(vector_index,__LINE__) < __clove_vector_count(VECTOR_PTR); ++__CLOVE_MACRO_COMBINE(vector_index,__LINE__)) { \
        TYPE* NAME = (TYPE*)__clove_vector_get(VECTOR_PTR, __CLOVE_MACRO_COMBINE(vector_index,__LINE__)); \
        BODY \
    }
#pragma endregion // Vector Decl

#pragma region PRIVATE - Map Decl
typedef struct __clove_map_node_t {
    char*       key;
    size_t      key_size;
    void*       value;
    struct __clove_map_node_t* next;
} __clove_map_node_t;

typedef struct __clove_map_params_t {
    size_t initial_hash_size;
    size_t (*hash_funct)(void*, size_t);
    void   (*item_dtor)(void*);
} __clove_map_params_t;

typedef struct __clove_map_t {
    size_t                   count;
    __clove_map_node_t**     hashmap; 
    size_t                   hashmap_size;
    size_t                   (*hash_funct)(void*, size_t);
    void                     (*item_dtor)(void*);
} __clove_map_t;

size_t __clove_map_hash_djb33x(void *key, size_t keylen);
__clove_map_params_t __clove_map_params_defaulted(void);
void __clove_map_init(__clove_map_t* map, __clove_map_params_t* params);
void __clove_map_free(__clove_map_t* map);
size_t __clove_map_count(__clove_map_t* map);
void __clove_map_put(__clove_map_t* dict, const char* key, void* value);
void* __clove_map_get(__clove_map_t* map, const char* key);
bool __clove_map_has_key(__clove_map_t* map, const char* key);

#define __CLOVE_MAP_INIT(MAP_PTR) \
    __clove_map_params_t __CLOVE_MACRO_COMBINE(params,__LINE__) = __clove_map_params_defaulted(); \
    __clove_map_init(MAP_PTR, &__CLOVE_MACRO_COMBINE(params,__LINE__));

#define __CLOVE_MAP_INIT_PARAMS(MAP_PTR, PARAMS) \
    __clove_map_params_t __CLOVE_MACRO_COMBINE(params,__LINE__) = PARAMS; \
    __clove_map_init(MAP_PTR, &__CLOVE_MACRO_COMBINE(params,__LINE__));

#pragma endregion // Map Decl

#pragma region PRIVATE - CommandLine Decl
#include <stdbool.h>
typedef struct __clove_cmdline_t {
    int arg_index;
    int argc;
    const char** argv;
    __clove_map_t map;
} __clove_cmdline_t;

typedef enum __clove_cmdline_errno_t {
    __CLOVE_CMD_ERRNO_UNMANAGED = -1,
    __CLOVE_CMD_ERRNO_OK = 0,
    __CLOVE_CMD_ERRNO_GENERIC = 1,
    __CLOVE_CMD_ERRNO_INVALID_PARAM = 2,
} __clove_cmdline_errno_t;

__CLOVE_EXTERN_C void __clove_cmdline_init(__clove_cmdline_t* cmdline, const char** argv, int argc);
__CLOVE_EXTERN_C void __clove_cmdline_free(__clove_cmdline_t* cmdline);
__CLOVE_EXTERN_C bool __clove_cmdline_next_opt(__clove_cmdline_t* cmdline, const char** opt_out);
__CLOVE_EXTERN_C bool __clove_cmdline_next_arg(__clove_cmdline_t* cmdline, const char** arg_out);
__CLOVE_EXTERN_C bool __clove_cmdline_has_opt(__clove_cmdline_t* cmdline, const char* opt);
__CLOVE_EXTERN_C bool __clove_cmdline_has_any_opt(__clove_cmdline_t* cmdline, const char* opt1, const char* opt2);
__CLOVE_EXTERN_C const char* __clove_cmdline_get_opt_value(__clove_cmdline_t* cmdline, const char* opt);
__CLOVE_EXTERN_C const char* __clove_cmdline_get_any_opt_value(__clove_cmdline_t* cmdline, const char* opt1, const char* opt2);
__CLOVE_EXTERN_C const char* __clove_cmdline_get_any_opt_value_defaulted(__clove_cmdline_t* cmdline, const char* opt1, const char* opt2, const char* default_value);
__CLOVE_EXTERN_C const __clove_vector_t* __clove_cmdline_get_opt_values(__clove_cmdline_t* cmdline, const char* opt);
__CLOVE_EXTERN_C void __clove_cmdline_add_opt(__clove_cmdline_t* cmd, const char* opt, const char* value);
//Command Handlers
 typedef __clove_cmdline_errno_t (*__clove_cmdline_handler_f)(__clove_cmdline_t*);
__CLOVE_EXTERN_C __clove_cmdline_errno_t __clove_cmdline_handle_help(__clove_cmdline_t* cmd);
__CLOVE_EXTERN_C __clove_cmdline_errno_t __clove_cmdline_handle_version(__clove_cmdline_t* cmd);
__CLOVE_EXTERN_C __clove_cmdline_errno_t __clove_cmdline_handle_run_tests(__clove_cmdline_t* cmd);
__CLOVE_EXTERN_C __clove_cmdline_errno_t __clove_cmdline_handle_default(__clove_cmdline_t* cmd);
__CLOVE_EXTERN_C void __clove_cmdline_create_test_expr(__clove_cmdline_t* cmd, const char* opt1, const char* opt2, __clove_vector_t* out_expressions);
#pragma endregion // CommandLine Decl

#pragma region PRIVATE - Test Decl
typedef union __clove_generic_u {
    bool               _bool;
    char               _char;
    int                _int;
    unsigned int       _uint;
    long               _long;
    unsigned long      _ulong;
    long long          _llong;
    unsigned long long _ullong;
    size_t             _sizet;
    float              _float;
    double             _double;
    char* _string;
    void* _ptr;
} __clove_generic_u;

/* TODO: Delete in favor of a crazy custom implementation of string enum... (See below)
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

typedef enum __clove_test_result_e {
    __CLOVE_TEST_RESULT_PASSED = 1,
    __CLOVE_TEST_RESULT_FAILED = 2,
    __CLOVE_TEST_RESULT_SKIPPED = 3
} __clove_test_result_e;
*/

/* Custom String Enum for Generic Type */
typedef const char* __clove_generic_type_e;
extern const char* __CLOVE_GENERIC_BOOL;
extern const char* __CLOVE_GENERIC_CHAR;
extern const char* __CLOVE_GENERIC_INT;
extern const char* __CLOVE_GENERIC_UINT;
extern const char* __CLOVE_GENERIC_LONG;
extern const char* __CLOVE_GENERIC_ULONG;
extern const char* __CLOVE_GENERIC_LLONG;
extern const char* __CLOVE_GENERIC_ULLONG;
extern const char* __CLOVE_GENERIC_SIZET;
extern const char* __CLOVE_GENERIC_FLOAT;
extern const char* __CLOVE_GENERIC_DOUBLE;
extern const char* __CLOVE_GENERIC_STRING;
extern const char* __CLOVE_GENERIC_PTR;
#define __CLOVE_GENERIC_TYPE_E_DECL() \
    const char* __CLOVE_GENERIC_BOOL = "BOOL"; \
    const char* __CLOVE_GENERIC_CHAR   = "CHAR"; \
    const char* __CLOVE_GENERIC_INT    = "INT"; \
    const char* __CLOVE_GENERIC_UINT   = "UINT"; \
    const char* __CLOVE_GENERIC_LONG   = "LONG"; \
    const char* __CLOVE_GENERIC_ULONG  = "ULONG"; \
    const char* __CLOVE_GENERIC_LLONG  = "LLONG"; \
    const char* __CLOVE_GENERIC_ULLONG = "ULLONG"; \
    const char* __CLOVE_GENERIC_SIZET  = "SIZET"; \
    const char* __CLOVE_GENERIC_FLOAT  = "FLOAT"; \
    const char* __CLOVE_GENERIC_DOUBLE = "DOUBLE"; \
    const char* __CLOVE_GENERIC_STRING = "STRING"; \
    const char* __CLOVE_GENERIC_PTR    = "PTR"; 
    
/* Custom String Enum for Assert Check */
typedef const char*  __clove_assert_check_e;
extern const char* __CLOVE_ASSERT_EQ;
extern const char* __CLOVE_ASSERT_NE;
extern const char* __CLOVE_ASSERT_LT;
extern const char* __CLOVE_ASSERT_LTE;
extern const char* __CLOVE_ASSERT_GT;
extern const char* __CLOVE_ASSERT_GTE;
extern const char* __CLOVE_ASSERT_FAIL;
#define __CLOVE_ASSERT_CHECK_E_DECL() \
    const char* __CLOVE_ASSERT_EQ   = "EQ";\
    const char* __CLOVE_ASSERT_NE   = "NE";\
    const char* __CLOVE_ASSERT_GT   = "GT";\
    const char* __CLOVE_ASSERT_GTE  = "GTE";\
    const char* __CLOVE_ASSERT_LT   = "LT";\
    const char* __CLOVE_ASSERT_LTE  = "LTE";\
    const char* __CLOVE_ASSERT_FAIL = "FAIL";

/* Custom String Enum for Test Result */
typedef const char* __clove_test_result_e;
extern const char* __CLOVE_TEST_RESULT_PASSED;
extern const char* __CLOVE_TEST_RESULT_SKIPPED;
extern const char* __CLOVE_TEST_RESULT_FAILED;
#define __CLOVE_TEST_RESULT_E_DECL() \
    const char* __CLOVE_TEST_RESULT_PASSED  = "PASS";\
    const char* __CLOVE_TEST_RESULT_FAILED  = "FAIL";\
    const char* __CLOVE_TEST_RESULT_SKIPPED = "SKIP";

typedef struct __clove_test_t {
    char* name;
    void (*funct)(struct __clove_test_t*);
    __clove_test_result_e result;
    __clove_time_t duration;
    const char* file_name;
    bool dry_run;
    size_t funct_line;
    struct {
        unsigned int line;
        __clove_assert_check_e assert;
        __clove_generic_type_e data_type;
        __clove_generic_u expected;
        __clove_generic_u actual;
        unsigned char floating_precision; //Just used for float/double to represent decimal digit. NOTE: Eventually refactor to a data struct to represent expected/actual and encapusulate also __clove_generic_u.
    } issue;
} __clove_test_t;

__CLOVE_EXTERN_C void __clove_vector_test_ctor(void* test);
__CLOVE_EXTERN_C void __clove_vector_test_dtor(void* test_ptr);
#pragma endregion // Test Decl

#pragma region PRIVATE - Suite Decl
typedef struct __clove_suite_t {
    char* name;
    __clove_vector_t tests;
    size_t test_count;
    struct {
        void (*setup_once)(void);
        void (*teardown_once)(void);
        void (*setup)(void);
        void (*teardown)(void);
    } fixtures;
    struct {
        __clove_time_t duration;
        size_t passed_count;
        size_t failed_count;
        size_t skipped_count;
    } issue;

} __clove_suite_t;

__CLOVE_EXTERN_C void __clove_vector_suite_ctor(void* suite_ptr);
__CLOVE_EXTERN_C void __clove_vector_suite_dtor(void* suite_ptr);
#pragma endregion

#pragma region PRIVATE - Assert Decl
#define __CLOVE_ASSERT_GUARD \
    if (_this->result == __CLOVE_TEST_RESULT_FAILED) { return; }\
    if (_this->file_name == NULL) _this->file_name = __FILE__; \
    _this->issue.line = __LINE__;

#define __CLOVE_ASSERT_INTEGER_CHECK(mode, exp, act, type, field, test) \
    bool pass_scenario = false;\
    if (check_mode == __CLOVE_ASSERT_EQ)       { pass_scenario = exp == act; }\
    else if (check_mode == __CLOVE_ASSERT_NE)  { pass_scenario = exp != act; }\
    else if (check_mode == __CLOVE_ASSERT_GT)  { pass_scenario = exp >  act; }\
    else if (check_mode == __CLOVE_ASSERT_GTE) { pass_scenario = exp >= act; }\
    else if (check_mode == __CLOVE_ASSERT_LT)  { pass_scenario = exp <  act; }\
    else if (check_mode == __CLOVE_ASSERT_LTE) { pass_scenario = exp <= act; }\
    if (pass_scenario) _this->result =  __CLOVE_TEST_RESULT_PASSED;\
    else { \
        _this->result =  __CLOVE_TEST_RESULT_FAILED;\
        _this->issue.assert = mode;\
        _this->issue.data_type = type;\
        _this->issue.expected.field = exp;\
        _this->issue.actual.field = act;\
    }

__CLOVE_EXTERN_C void __clove_assert_fail(__clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_pass(__clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_int(__clove_assert_check_e check_mode, int expected, int result, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_uint(__clove_assert_check_e check_mode, unsigned int expected, unsigned int result, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_long(__clove_assert_check_e check_mode, long expected, long result, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_ulong(__clove_assert_check_e check_mode, unsigned long expected, unsigned long result, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_llong(__clove_assert_check_e check_mode, long long expected, long long result, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_ullong(__clove_assert_check_e check_mode, unsigned long long expected, unsigned long long result, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_sizet(__clove_assert_check_e check_mode, size_t expected, size_t result, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_char(__clove_assert_check_e check_mode, char expected, char result, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_bool(__clove_assert_check_e check_mode, bool expected, bool result, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_null(__clove_assert_check_e check_mode, void* expected, void* result, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_ptr(__clove_assert_check_e check_mode, void* expected, void* result, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_float(__clove_assert_check_e check_mode, float expected, float result, unsigned char precision, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_double(__clove_assert_check_e check_mode, double expected, double result, unsigned char precision, __clove_test_t* _this);
__CLOVE_EXTERN_C void __clove_assert_string(__clove_assert_check_e check_mode, const char* expected, const char* result, __clove_test_t* _this);
#pragma endregion // Assert Decl

#pragma region PRIVATE - Stream Decl
typedef struct __clove_stream_t {
    bool (*open)(struct __clove_stream_t* _this);
    void (*close)(struct __clove_stream_t* _this);
    void (*writef)(struct __clove_stream_t* _this, const char* format, ...);
    void (*seek)(struct __clove_stream_t* _this, long offset, int origin);
    bool (*has_ansi_support)(struct __clove_stream_t* _this);
    void (*free)(struct __clove_stream_t* _this);
} __clove_stream_t;

typedef struct __clove_stream_console_t {
    __clove_stream_t base;
} __clove_stream_console_t;

__CLOVE_EXTERN_C __clove_stream_console_t* __clove_stream_console_new(void);
__CLOVE_EXTERN_C bool __clove_stream_console_open(__clove_stream_t* stream);
__CLOVE_EXTERN_C void __clove_stream_console_close(__clove_stream_t* stream);
__CLOVE_EXTERN_C void __clove_stream_console_writef(__clove_stream_t* stream, const char* format, ...);
__CLOVE_EXTERN_C void __clove_stream_console_seek(__clove_stream_t* stream, long offset, int origin);
__CLOVE_EXTERN_C bool __clove_stream_console_has_ansi_support(struct __clove_stream_t* _this);
__CLOVE_EXTERN_C void __clove_stream_console_free(__clove_stream_t* stream);

#include <stdio.h> 
typedef struct __clove_stream_file_t {
    __clove_stream_t base;
     const char* file_path;
    FILE* file; //No way to forward declaring FILE. the only way should be to use void*
} __clove_stream_file_t;

__CLOVE_EXTERN_C __clove_stream_file_t* __clove_stream_file_new(const char* file_path);
__CLOVE_EXTERN_C bool __clove_stream_file_open(__clove_stream_t* stream);
__CLOVE_EXTERN_C void __clove_stream_file_close(__clove_stream_t* stream);
__CLOVE_EXTERN_C void __clove_stream_file_writef(__clove_stream_t* stream, const char* format, ...);
__CLOVE_EXTERN_C void __clove_stream_file_seek(__clove_stream_t* stream, long offset, int origin);
__CLOVE_EXTERN_C bool __clove_stream_file_has_ansi_support(struct __clove_stream_t* _this);
__CLOVE_EXTERN_C void __clove_stream_file_free(__clove_stream_t* stream);

//In Memory Stream
typedef struct __clove_stream_memory_t {
    __clove_stream_t base;
    __clove_vector_t lines;
} __clove_stream_memory_t;

__CLOVE_EXTERN_C __clove_stream_memory_t* __clove_stream_memory_new(void);
__CLOVE_EXTERN_C bool __clove_stream_memory_open(__clove_stream_t* stream);
__CLOVE_EXTERN_C void __clove_stream_memory_close(__clove_stream_t* stream);
__CLOVE_EXTERN_C void __clove_stream_memory_writef(__clove_stream_t* stream, const char* format, ...);
__CLOVE_EXTERN_C void __clove_stream_memory_seek(__clove_stream_t* stream, long offset, int origin);
__CLOVE_EXTERN_C bool __clove_stream_memory_has_ansi_support(struct __clove_stream_t* _this);
__CLOVE_EXTERN_C void __clove_stream_memory_free(__clove_stream_t* stream);
__CLOVE_EXTERN_C char* __clove_stream_memory_get_line(__clove_stream_memory_t* mem_stream, size_t index);
__CLOVE_EXTERN_C char* __clove_stream_memory_as_string(__clove_stream_memory_t* mem_stream);
#pragma endregion //Stream Decl

#pragma region PRIVATE - Report Decl
typedef struct __clove_report_t {
    void (*start)(struct __clove_report_t* _this, size_t suite_count, size_t test_count);
    void (*end)(struct __clove_report_t* _this, size_t test_count, size_t passed, size_t skipped, size_t failed);
    void (*begin_suite)(struct __clove_report_t* _this, __clove_suite_t* suite, size_t index);
    void (*end_suite)(struct __clove_report_t* _this, __clove_suite_t* suite, size_t index);
    void (*end_test)(struct __clove_report_t* _this, __clove_suite_t* suite, __clove_test_t* test, size_t test_number);
    void (*free)(struct __clove_report_t* _this);
} __clove_report_t;

typedef struct __clove_test_expr_t {
    __clove_string_view_t suite_view;
    __clove_string_view_t test_view;
} __clove_test_expr_t;

typedef enum __clove_report_detail_e {
    __CLOVE_REPORT_DETAIL__NONE = 0,
    __CLOVE_REPORT_DETAIL__FAILED = 1,
    __CLOVE_REPORT_DETAIL__FAILED_SKIPPED = 2,
    __CLOVE_REPORT_DETAIL__PASSED_FAILED_SKIPPED = 3,
} __clove_report_detail_e;

typedef struct __clove_report_params_t {
    const char* tests_base_path;
    __clove_report_detail_e report_detail;
} __clove_report_params_t;

__CLOVE_EXTERN_C void __clove_test_expr_init(__clove_test_expr_t* expr,  const char* expr_str);
__CLOVE_EXTERN_C bool __clove_test_expr_validate_vw(const __clove_string_view_t* match, const __clove_string_view_t* view);
__CLOVE_EXTERN_C bool __clove_test_expr_validate(__clove_test_expr_t* expr, const __clove_string_view_t* suite, const __clove_string_view_t* test);
#pragma endregion

#pragma region PRIVATE - RunTests Report Pretty Decl
#include <stdbool.h>
typedef struct __clove_report_pretty_t {
    __clove_report_t base;
    __clove_stream_t* stream;
    __clove_report_params_t* params;
    __clove_time_t start_time;
    unsigned int max_test_digits;
    struct {
        const char* info;
        const char* warn;
        const char* erro;
        const char* pass;
        const char* skip;
        const char* fail;
    } labels;
} __clove_report_pretty_t;
__clove_report_pretty_t* __clove_report_run_tests_pretty_new(__clove_stream_t* stream, __clove_report_params_t* params);
__CLOVE_EXTERN_C void __clove_report_pretty_free(__clove_report_t* report);
__CLOVE_EXTERN_C void __clove_report_pretty_start(__clove_report_t* _this, size_t suite_count, size_t test_count);
__CLOVE_EXTERN_C void __clove_report_pretty_begin_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index);
__CLOVE_EXTERN_C void __clove_report_pretty_end_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index);
__CLOVE_EXTERN_C void __clove_report_pretty_end_test(__clove_report_t* _this, __clove_suite_t* suite, __clove_test_t* test, size_t test_number);
__CLOVE_EXTERN_C void __clove_report_pretty_end(__clove_report_t* _this, size_t test_count, size_t passed, size_t skipped, size_t failed);
__CLOVE_EXTERN_C void __clove_report_pretty_string_ellipse(const char* exp, size_t exp_len, const char* act, size_t act_len, char* exp_short, char* act_short, size_t short_size);
__CLOVE_EXTERN_C void __clove_report_pretty_pad_right(char* result, char* strToPad);
#define __CLOVE_STRING_LENGTH 256
#define __CLOVE_TEST_ENTRY_LENGTH 60
#define __PRETTY_PRINT_FAIL_ASSERT_MSG(buffer, buffer_size, assert, exp, act, print_type) \
{ \
    char phrase_format[40] = {0}; \
    if (assert == __CLOVE_ASSERT_EQ)      __clove_string_sprintf(phrase_format, sizeof(phrase_format), "expected [%s] but was [%s]", print_type, print_type); \
    else if(assert == __CLOVE_ASSERT_NE)  __clove_string_sprintf(phrase_format, sizeof(phrase_format), "not expected [%s] but was [%s]", print_type, print_type); \
    else if(assert == __CLOVE_ASSERT_GT)  __clove_string_sprintf(phrase_format, sizeof(phrase_format), "expected [%s > %s] but wasn't", print_type, print_type); \
    else if(assert == __CLOVE_ASSERT_GTE) __clove_string_sprintf(phrase_format, sizeof(phrase_format), "expected [%s >= %s] but wasn't", print_type, print_type); \
    else if(assert == __CLOVE_ASSERT_LT) __clove_string_sprintf(phrase_format, sizeof(phrase_format),  "expected [%s < %s] but wasn't", print_type, print_type); \
    else if(assert == __CLOVE_ASSERT_LTE) __clove_string_sprintf(phrase_format, sizeof(phrase_format), "expected [%s <= %s] but wasn't", print_type, print_type); \
    __clove_string_sprintf(buffer, buffer_size, phrase_format, exp, act); \
}
#pragma endregion

#pragma region PRIVATE - RunTests Report CSV Decl
#include <stdbool.h>
typedef struct __clove_report_run_tests_csv_t {
    __clove_report_t base;
    __clove_stream_t* stream;
     __clove_report_params_t* params;
} __clove_report_run_tests_csv_t;
__clove_report_run_tests_csv_t* __clove_report_run_tests_csv_new(__clove_stream_t* stream,  __clove_report_params_t* params);
__CLOVE_EXTERN_C void __clove_report_run_tests_csv_free(__clove_report_t* report);
__CLOVE_EXTERN_C void __clove_report_run_tests_csv_start(__clove_report_t* _this, size_t suite_count, size_t test_count);
__CLOVE_EXTERN_C void __clove_report_run_tests_csv_begin_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index);
__CLOVE_EXTERN_C void __clove_report_run_tests_csv_end_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index);
__CLOVE_EXTERN_C void __clove_report_run_tests_csv_end_test(__clove_report_t* _this, __clove_suite_t* suite, __clove_test_t* test, size_t test_number);
__CLOVE_EXTERN_C void __clove_report_run_tests_csv_end(__clove_report_t* _this, size_t test_count, size_t passed, size_t skipped, size_t failed);
__CLOVE_EXTERN_C void __clove_report_run_tests_csv_print_data(__clove_report_run_tests_csv_t* _this, __clove_test_t* test, __clove_generic_u* data);
#pragma endregion

#pragma region PRIVATE - Report Json Decl
typedef struct __clove_report_json_t {
    __clove_report_t base;
    __clove_stream_t* stream;
    __clove_report_params_t* params;
    const char* clove_version;
    const char* json_schema;
    __clove_suite_t* current_suite;
    bool is_first_suite_test;
    size_t test_count;
    size_t suite_tests_number;
    size_t suite_count;
    __clove_vector_t cached_suites;
    bool is_realtime_scenario;
    bool is_reporting_enabled;
} __clove_report_json_t;

__CLOVE_EXTERN_C __clove_report_json_t* __clove_report_run_tests_json_new(__clove_stream_t* stream, __clove_report_params_t* params);
__CLOVE_EXTERN_C void __clove_report_json_free(__clove_report_t* report);
__CLOVE_EXTERN_C void __clove_report_json_start(__clove_report_t* _this, size_t suite_count, size_t test_count);
__CLOVE_EXTERN_C void __clove_report_json_begin_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index);
__CLOVE_EXTERN_C void __clove_report_json_end_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index);
__CLOVE_EXTERN_C void __clove_report_json_end(__clove_report_t* _this, size_t test_count, size_t passed, size_t skipped, size_t failed);
__CLOVE_EXTERN_C void __clove_report_json_end_test(__clove_report_t* _this, __clove_suite_t* suite, __clove_test_t* test, size_t test_number);
__CLOVE_EXTERN_C void __clove_report_json_print_data(__clove_report_json_t* _this, __clove_test_t* test, __clove_generic_u* data);
#pragma endregion

#pragma region PRIVATE - Report List Test Decl
typedef struct __clove_report_list_tests_t {
    void (*begin)(struct __clove_report_list_tests_t* _this, size_t suite_count, size_t test_count);
    void (*begin_suite)(struct __clove_report_list_tests_t* _this, __clove_suite_t* suite, size_t index);
    void (*begin_test)(struct __clove_report_list_tests_t* _this, __clove_test_t* test, size_t index);
    void (*end_test)(struct __clove_report_list_tests_t* _this, __clove_test_t* test, size_t index);
    void (*end_suite)(struct __clove_report_list_tests_t* _this, __clove_suite_t* suite, size_t index);
    void (*end)(struct __clove_report_list_tests_t* _this);
    void (*free)(struct __clove_report_list_tests_t* _this);
} __clove_report_list_tests_t;

typedef struct __clove_report_list_tests_pretty_t {
   __clove_report_list_tests_t base;
   __clove_stream_t* stream;
   __clove_report_params_t* params;
   const char* suite_format;
   const char* test_format;
   bool is_suite_first_test;
   __clove_suite_t* current_suite;
} __clove_report_list_tests_pretty_t;

__clove_report_list_tests_pretty_t* __clove_report_list_tests_pretty_new(__clove_stream_t* stream, __clove_report_params_t* params);
void __clove_report_list_tests_pretty_free(__clove_report_list_tests_t* _this);
void __clove_report_list_tests_pretty_begin(__clove_report_list_tests_t* _this, size_t suite_count, size_t test_count);
void __clove_report_list_tests_pretty_begin_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index);
void __clove_report_list_tests_pretty_end_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index);
void __clove_report_list_tests_pretty_begin_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index);
void __clove_report_list_tests_pretty_end_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index);
void __clove_report_list_tests_pretty_end(__clove_report_list_tests_t* _this);

typedef struct __clove_report_list_tests_csv_t {
   __clove_report_list_tests_t base;
   __clove_stream_t* stream;
   __clove_report_params_t* params;
   __clove_suite_t* current_suite;
} __clove_report_list_tests_csv_t;

__clove_report_list_tests_csv_t* __clove_report_list_tests_csv_new(__clove_stream_t* stream, __clove_report_params_t* params);
void __clove_report_list_tests_csv_free(__clove_report_list_tests_t* _this);
void __clove_report_list_tests_csv_begin(__clove_report_list_tests_t* _this, size_t suite_count, size_t test_count);
void __clove_report_list_tests_csv_begin_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index);
void __clove_report_list_tests_csv_end_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index);
void __clove_report_list_tests_csv_begin_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index);
void __clove_report_list_tests_csv_end_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index);
void __clove_report_list_tests_csv_end(__clove_report_list_tests_t* _this);

typedef struct __clove_report_list_tests_json_t {
   __clove_report_list_tests_t base;
   __clove_stream_t* stream;
   __clove_report_params_t* params;
   __clove_suite_t* current_suite;
   const char* clove_version;
   const char* json_schema;
   size_t suite_count;
   bool is_suite_first_test;
} __clove_report_list_tests_json_t;

__clove_report_list_tests_json_t* __clove_report_list_tests_json_new(__clove_stream_t* stream, __clove_report_params_t* params);
void __clove_report_list_tests_json_free(__clove_report_list_tests_t* _this);
void __clove_report_list_tests_json_begin(__clove_report_list_tests_t* _this, size_t suite_count, size_t test_count);
void __clove_report_list_tests_json_begin_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index);
void __clove_report_list_tests_json_end_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index);
void __clove_report_list_tests_json_begin_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index);
void __clove_report_list_tests_json_end_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index);
void __clove_report_list_tests_json_end(__clove_report_list_tests_t* _this);

int __clove_cmd_list_test_execute(__clove_suite_t* suites, size_t suite_count, size_t test_count, __clove_report_list_tests_t* report);
#pragma endregion

#pragma region PRIVATE - Autodiscovery Decl
#include <stdbool.h>
typedef struct __clove_symbols_context_t {
    __clove_vector_t suites;
    size_t suites_count;
    size_t tests_count;
    const char* prefix;
    size_t prefix_length;
    const __clove_vector_t* includes;
    const __clove_vector_t* excludes;
} __clove_symbols_context_t;

typedef struct __clove_symbols_function_t {
    char* name;
    //Using union just to avoid explicit casting from object ptr to function ptr 'void* to void(*)()'
    //which emit warning using -Wpedantic flag with GCC compiler.
    union {
        void* obj_ptr;
        void (*fun_ptr)(void);
    };
} __clove_symbols_function_t;

__CLOVE_EXTERN_C bool __clove_symbols_function_validate(__clove_string_view_t* suite, __clove_string_view_t* type, __clove_string_view_t* name, __clove_symbols_context_t* context);
__CLOVE_EXTERN_C void __clove_symbols_function_collect(__clove_symbols_function_t exported_funct, __clove_symbols_context_t* context);
//For each OS / symbols table format
__CLOVE_EXTERN_C typedef void (*__clove_symbols_function_action)(__clove_symbols_function_t, __clove_symbols_context_t* context);
__CLOVE_EXTERN_C int __clove_symbols_for_each_function_by_prefix(__clove_symbols_context_t* context, __clove_symbols_function_action action);
#pragma endregion //Autodiscovery Decl

#pragma region PRIVATE - Run Decl
__CLOVE_EXTERN_C int __clove_runner_auto(int argc, char* argv[]);
__CLOVE_EXTERN_C int __clove_run_tests_with_report(__clove_report_t* report, __clove_vector_t* includes, __clove_vector_t* excludes);
__CLOVE_EXTERN_C int __clove_exec_suites(__clove_suite_t* suites, size_t suite_count, size_t test_count, __clove_report_t* report);
__CLOVE_EXTERN_C void __clove_exec_suite(__clove_suite_t* suite, size_t test_counter, size_t* passed, size_t* failed, size_t* skipped, __clove_report_t* report);
#pragma endregion // Run Decl

#pragma region PRIVATE - Api Decl
#ifdef _WIN32
#define __CLOVE_API_EXPORT __CLOVE_EXTERN_C __declspec(dllexport)
#else 
#define __CLOVE_API_EXPORT __CLOVE_EXTERN_C
#endif //_WIN32

//Note: Not exported for windows (symbol limit 65535). Even "extern c" directive is not needed
//      Furthermore using prefix __clove_symint__ so that this kind of function is excluded from Symbol Discovery
#define __CLOVE_SUITE_METHOD_INTERNAL_DECL_2(suite, title, param) void __clove_symint___##suite##___##title(param)
#define __CLOVE_SUITE_METHOD_INTERNAL_DECL_1(suite, name, param) __CLOVE_SUITE_METHOD_INTERNAL_DECL_2(suite, name, param)
#define __CLOVE_SUITE_METHOD_INTERNAL_INVOKE_2(suite, title, param) __clove_symint___##suite##___##title(param)
#define __CLOVE_SUITE_METHOD_INTERNAL_INVOKE_1(suite, title, param) __CLOVE_SUITE_METHOD_INTERNAL_INVOKE_2(suite, title, param)

#define __CLOVE_SUITE_METHOD_DECL_2(suite, title, param) __CLOVE_API_EXPORT void __clove_sym___##suite##___##title(param)
#define __CLOVE_SUITE_METHOD_DECL_1(suite, name, param) __CLOVE_SUITE_METHOD_DECL_2(suite, name, param)

#define __CLOVE_SUITE_SETUP_ONCE_AUTO() __CLOVE_SUITE_METHOD_DECL_1( CLOVE_SUITE_NAME, 11_setuponce, void)
#define __CLOVE_SUITE_TEARDOWN_ONCE_AUTO() __CLOVE_SUITE_METHOD_DECL_1( CLOVE_SUITE_NAME, 12_teardownonce, void)
#define __CLOVE_SUITE_SETUP_AUTO() __CLOVE_SUITE_METHOD_DECL_1( CLOVE_SUITE_NAME, 13_setup, void)
#define __CLOVE_SUITE_TEARDOWN_AUTO() __CLOVE_SUITE_METHOD_DECL_1( CLOVE_SUITE_NAME, 14_teardown, void)
#define __CLOVE_TEST_AUTO(title) \
    __CLOVE_SUITE_METHOD_INTERNAL_DECL_1( CLOVE_SUITE_NAME, 21_ ## title, __clove_test_t *_this); \
    __CLOVE_SUITE_METHOD_DECL_1( CLOVE_SUITE_NAME, 20_ ## title, __clove_test_t *_this) {\
        _this->file_name = __FILE__; \
        _this->funct_line = __LINE__; \
        if (_this->dry_run) return; \
        __CLOVE_SUITE_METHOD_INTERNAL_INVOKE_1(CLOVE_SUITE_NAME, 21_ ## title, _this); \
    } \
    __CLOVE_SUITE_METHOD_INTERNAL_DECL_1( CLOVE_SUITE_NAME, 21_ ## title, __clove_test_t *_this)
#pragma endregion

#pragma endregion // DECLARATION

#pragma region PRIVATE - IMPLEMENTATION
#ifdef CLOVE_IMPLEMENTATION
#pragma region PRIVATE - Utils Impl
#include <string.h>
#include <stdio.h>
void __clove_utils_empty_funct(void) { }

const char* __clove_get_exec_base_path(void) {
    return __clove_exec_base_path;
}

const char* __clove_get_exec_path(void) {
    return __clove_exec_path;
}
#pragma endregion // Utils Impl

#pragma region PRIVATE - Math Impl
#include <math.h>
unsigned int __clove_math_powi(unsigned int base, unsigned int exp) {
    unsigned int result = 1;
    for(unsigned int i=0; i < exp; ++i) {
        result *= base;
    }
    return result;
}

float __clove_math_decimalf(unsigned char precision) {
    unsigned int divider = __clove_math_powi(10, precision);
    return 1.0f / (float)divider;
}

double __clove_math_decimald(unsigned char precision) {
    unsigned int divider = __clove_math_powi(10, precision);
    return 1.0 / (double)divider;
}
#pragma endregion

#pragma region PRIVATE - Path Impl
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
char*  __clove_path_concat(const char separator, const char* path1, const char* path2) {
    size_t count = __clove_string_length(path1) + 1 + __clove_string_length(path2) + 1;
    char* path = __CLOVE_MEMORY_CALLOC_TYPE_N(char, count);

    __clove_string_strcat(path, count, path1);
    __clove_string_strncat(path, count, &separator, 1);
    __clove_string_strcat(path, count, path2);

    __clove_string_replace_char(path, '/', separator);
    __clove_string_replace_char(path, '\\', separator);

    return path;
}

const char* __clove_path_relative(const char* abs_path, const char* base_path) {
    if (!__clove_string_startswith(abs_path, base_path)) return abs_path;

    size_t base_path_length = __clove_string_length(base_path);
    const char* result = abs_path + base_path_length;
    if (__clove_string_startswith(result, __CLOVE_PATH_SEPARATOR_STR)) {
        result += 1;
    }
    return result;
}

char* __clove_path_rel_to_abs_exec_path(const char* rel_path) {
    const char* base_path = __clove_get_exec_base_path();
    char* abs_path = __clove_path_concat(__CLOVE_PATH_SEPARATOR, base_path, rel_path);
    return abs_path;
}

bool __clove_path_is_relative(const char* path) {
    if (__clove_string_startswith(path, "\\")) return false; //windows
    if (__clove_string_length(path) > 2 && path[1] == ':') return false;    //windows
    if (__clove_string_startswith(path, "/")) return false;  //unix or Windows
    return true;
}

void __clove_path_to_os(char* path) {
    __clove_string_replace_char(path, '/', __CLOVE_PATH_SEPARATOR);
    __clove_string_replace_char(path, '\\', __CLOVE_PATH_SEPARATOR);
}

char* __clove_path_basepath(const char* a_path) {
    // Find the last path separator character in the input path.
    const char* last_char = a_path + __clove_string_length(a_path) - 1;
    while (last_char > a_path && *last_char != '/' && *last_char != '\\') {
        --last_char;
    }

    // If there are no separators in the path, return the current directory path.
    if (last_char == a_path) {
        static char dot_path[3] = { '.', __CLOVE_PATH_SEPARATOR, '\0' };
        return __clove_string_strdup(dot_path);
    }

    // Calculate base path length based on the position of the last path separator.
    size_t base_length = last_char - a_path;
    char* base_path = __CLOVE_MEMORY_CALLOC_TYPE_N(char, base_length + 1);
    __clove_string_strncpy(base_path, base_length + 1, a_path, base_length);
    __clove_path_to_os(base_path);
    return base_path;
}
#pragma endregion // Path Impl

#pragma region PRIVATE - Console Impl
#include <stdio.h>
#include <stdarg.h>
void __clove_console_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    __clove_console_vprintf(format, args);
    va_end(args);
}
void __clove_console_vprintf(const char* format, va_list args) {
     vprintf(format, args);
}
void __clove_console_write(const char* str) {
    __clove_console_printf("%s", str);
}
void __clove_console_writeline(const char* str) {
    __clove_console_printf("%s\n", str);
}
#pragma endregion //Console Impl

#pragma region PRIVATE - File Impl
FILE* __clove_file_open(const char* path, const char* mode) {
#ifdef _WIN32
    FILE* file;
    fopen_s(&file, path, mode);
    return file;
#else
    return fopen(path, mode);
#endif
}

void __clove_file_close(FILE* file) {
    if (file) fclose(file);
}

void __clove_file_printf(FILE* file, const char* format, ...) {
    va_list args;
    va_start(args, format);
    __clove_file_vprintf(file, format, args);
    va_end(args);
}

void __clove_file_vprintf(FILE* file, const char* format, va_list args) {
    vfprintf(file, format, args);
}

void __clove_file_write(FILE* file, const char* str) {
    __clove_file_printf(file, "%s", str);
}

void __clove_file_writeline(FILE* file, const char* str) {
    __clove_file_printf(file, "%s\n", str);
}
#pragma endregion //File Impl


#pragma region PRIVATE - Memory Impl
#include <string.h>
void* __clove_memory_malloc(size_t size) {
    return malloc(size);
}

void* __clove_memory_calloc(size_t size) {
    return calloc(1, size);
}

void* __clove_memory_realloc(void* source, size_t size) {
    return realloc(source, size);
}

bool __clove_memory_memcpy(void* dest, size_t dest_size, const void* src, size_t src_size) {
#ifdef _WIN32
    return memcpy_s(dest, dest_size, src, src_size) == 0;
#else
    __CLOVE_UNUSED_VAR(dest_size);
    return memcpy(dest, src, src_size) != NULL;
#endif
}

bool __clove_memory_memset(void* dest, size_t size, unsigned char value) {
    return memset(dest, value, size) != NULL;
}
#pragma endregion //Memory Impl

#pragma region PRIVATE - String Impl
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
bool __clove_string_equal(const char* str1, const char* str2) {
    return strcmp(str1, str2) == 0;
}

bool __clove_string_equal_any(const char* str1, size_t count, ...) {
    va_list args;
    va_start(args, count); 
    bool result = false;
    for(size_t i=0; i < count; ++i) {
        const char* arg = va_arg(args, const char*);
        if (__clove_string_equal(str1, arg)) {
            result = true;
            break;
        }
    }
    va_end(args);
    return result;
}


bool __clove_string_startswith(const char* str1, const char* prefix) {
    if (!str1 || !prefix) return false;
    size_t prefix_len = __clove_string_length(prefix);
    if (prefix_len == 0) {
        size_t str_len = __clove_string_length(str1);
        if (str_len == 0) return true;
        else return false;
    }
    return __clove_string_strncmp(str1, prefix, prefix_len);
}

bool __clove_string_endswith(const char* str, const char* suffix) {
    if (!str || !suffix) return false;
    size_t str_len = __clove_string_length(str);
    size_t suf_len = __clove_string_length(suffix);
    if (suf_len >  str_len) return false;
    return __clove_string_strncmp(str + str_len - suf_len, suffix, suf_len);
}

bool __clove_string_strncmp(const char* str1, const char* str2, size_t count) {
    return strncmp(str1, str2, count) == 0;
}

bool __clove_string_strcpy(char* dest, size_t dest_size, const char* source) {
#ifdef _WIN32
    return strcpy_s(dest, dest_size, source) == 0;
#else
    __CLOVE_UNUSED_VAR(dest_size);
    return strcpy(dest, source) != NULL;
#endif
}

bool __clove_string_strncpy(char* dest, size_t dest_size, const char* source, size_t count) {
#ifdef _WIN32
    return strncpy_s(dest, dest_size, source, count) == 0;
#else
    __CLOVE_UNUSED_VAR(dest_size);
    return strncpy(dest, source, count) != NULL;
#endif
}

bool __clove_string_strcat(char* dest, size_t dest_size, const char* source) {
#ifdef _WIN32
    return strcat_s(dest, dest_size, source) == 0;
#else
    __CLOVE_UNUSED_VAR(dest_size);
    return strcat(dest, source) != NULL;
#endif
}

bool __clove_string_strncat(char* dest, size_t dest_size, const char* source, size_t count) {
#ifdef _WIN32
    return strncat_s(dest, dest_size, source, count) == 0;
#else
    __CLOVE_UNUSED_VAR(dest_size);
    return strncat(dest, source, count) != NULL;
#endif
}

char* __clove_string_strdup(const char* str) {
#ifdef _WIN32
    return _strdup(str);
#else
    return strdup(str);
#endif
}

void __clove_string_sprintf(char* dest, size_t dest_size, const char* format, ...) {
    va_list args;
    va_start(args, format);
#ifdef _WIN32
    vsnprintf_s(dest, dest_size, dest_size, format, args);
#else
    vsnprintf(dest, dest_size, format, args);
#endif
    va_end(args);
}

void __clove_string_vsprintf(char* dest, size_t dest_size, const char* format, va_list args) {
#ifdef _WIN32
    vsnprintf_s(dest, dest_size, dest_size, format, args);
#else
    vsnprintf(dest, dest_size, format, args);
#endif
}

size_t __clove_string_length(const char* str) {
    return strlen(str);
}

const char* __clove_string_strstr(const char* str1, const char* str2) {
    return strstr(str1, str2);
}

bool __clove_string_contains(const char* str, const char* contained) {
    return __clove_string_strstr(str, contained) != NULL;
}

char* __clove_string_escape(const char* string) {
    size_t str_len = __clove_string_length(string);
    size_t esc_len = str_len * 2 + 1; //worst case where each char needs escape
    char* escaped = __CLOVE_MEMORY_CALLOC_TYPE_N(char, esc_len);

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

char* __clove_string_csv_escape(const char* string) {
    size_t str_len = __clove_string_length(string);
    size_t esc_len = str_len * 2 + 2 + 1; //worst case where each char need escape + 2 for string who need to be enclosed in double quotes
    char* escaped = __CLOVE_MEMORY_CALLOC_TYPE_N(char, esc_len);

    bool has_comma = false;
    if (__clove_string_contains(string, ","))  has_comma = true;

    size_t esc_index = has_comma ? 1 : 0;
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
            escaped[esc_index++] = '"'; //csv escape char for double quote
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

    if (has_comma) {
        escaped[0] = '"';
        escaped[esc_index] = '"';
    }

    return escaped;
}

void __clove_string_ellipse(const char* string, size_t str_len, size_t pos, char* out, size_t out_size) {
    if (str_len == 1) {
        out[0] = '\0';
        return;
    }
    if (str_len <= out_size-1) {
        __clove_string_strcpy(out, out_size, string);
        return;
    }

    bool left_ellipse = false;
    bool right_ellipse = false;
    if (pos >= 4) {
        left_ellipse = true;
    }

    if (str_len-1 - pos >= 4) {
        right_ellipse = true;
    }

    size_t out_start = 0;
    size_t start_index = 0;
    size_t out_end = out_size - 1;

    if (left_ellipse) {
        out_start = 3;
        start_index = right_ellipse ? pos - 3 : str_len - 12;
        out[0] = '.'; out[1] = '.'; out[2] = '.';
    }

    if (right_ellipse) {
        out_end -= 3;
        out[out_end] = '.'; out[out_end + 1] = '.'; out[out_end + 2] = '.';
    }
    out[out_size - 1] = '\0';

    size_t to_copy = out_end - out_start;

    //Copia stringa
    for (size_t i = 0; i < to_copy; ++i) {
        out[out_start + i] = string[start_index + i];
    }
}

void __clove_string_replace_char(char* str, char src_chr, char dst_chr) {
    size_t size = __clove_string_length(str);
    for (size_t i = 0; i < size; ++i) {
        if (str[i] == src_chr) {
            str[i] = dst_chr;
        }
    }
}
#pragma endregion //String Impl

#pragma region PRIVATE - String View Impl
#include <stdlib.h>
__clove_string_view_t __clove_string_view_from_offs(const char* source, size_t begin_offset, size_t end_offset) {
    __clove_string_view_t v;
    v.begin = source + begin_offset;
    v.length = end_offset - begin_offset + 1;
    return v;
}

__clove_string_view_t __clove_string_view_from_be(const char* begin, const char* end) {
    return __clove_string_view_from_offs(begin, 0, end - begin);
}

__clove_string_view_t __clove_string_view_from_len(const char* begin, size_t length) {
    return __clove_string_view_from_offs(begin, 0, length-1);
}

__clove_string_view_t __clove_string_view_from_str(const char* str) {
    return __clove_string_view_from_offs(str, 0, __clove_string_length(str) - 1);
}

size_t __clove_string_view_length(const __clove_string_view_t* view) {
    return view->length;
}

const char* __clove_string_view_begin(const __clove_string_view_t* view) {
    return view->begin;
}

const char* __clove_string_view_end(const __clove_string_view_t* view) {
    return view->begin + view->length - 1;
}

bool __clove_string_view_equals(const __clove_string_view_t* view1, const __clove_string_view_t* view2) {
    if (view1->length != view2->length) return false;
    return strncmp(view1->begin, view2->begin, view1->length) == 0;  //richiamare ncmp
}

bool __clove_string_view_ncmp(const __clove_string_view_t* view1, const __clove_string_view_t* view2, size_t count) {
    return __clove_string_strncmp(view1->begin, view2->begin, count);
}

bool __clove_string_view_endswith(const __clove_string_view_t* view, const __clove_string_view_t* suffix) {
    if (suffix->length > view->length) return false;

    const char* end = __clove_string_view_end(view);
    const char* beg = end - suffix->length+1;
    return __clove_string_strncmp(beg, suffix->begin, suffix->length);
}

bool __clove_string_view_nendswith(const __clove_string_view_t* view, const __clove_string_view_t* suffix, size_t suffix_begin_offset) {
    __clove_string_view_t suffix_with_offset = __clove_string_view_from_str(suffix->begin + suffix_begin_offset);
    return __clove_string_view_endswith(view, &suffix_with_offset);
}

bool __clove_string_view_strequals(const __clove_string_view_t* view, const char* str) {
    size_t length = __clove_string_view_length(view);
    if (__clove_string_length(str) != length) return false;
    return __clove_string_strncmp(view->begin, str, length);
}

char* __clove_string_view_as_string(const __clove_string_view_t* view) {
    char* str = (char*)__clove_memory_malloc(view->length + 1); //size+1 for null terminator
    __clove_string_strncpy(str, view->length + 1, view->begin, view->length);
    str[view->length] = '\0'; //maybe could be avoided?!
    return str;
}

char __clove_string_view_at(const __clove_string_view_t* view, size_t index) {
    return view->begin[index]; //No index check, preserving undefined behaviour
}

bool __clove_string_view_contains(const __clove_string_view_t* view, const __clove_string_view_t* fixture) {
    if (fixture->length > view->length) return false;
    size_t residual_length = view->length;
    size_t index = 0;
    while(fixture->length <= residual_length) { 
        if (__clove_string_strncmp(view->begin + index, fixture->begin, fixture->length)) return true;
        residual_length--;
        index++;
    }
    return false;
}
#pragma endregion // String View Impl

#pragma region PRIVATE - Time Impl
__clove_time_t __clove_time_sub(__clove_time_t* t1, __clove_time_t* t2) {
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

__clove_time_t __clove_time_sum(__clove_time_t* t1, __clove_time_t* t2) {
    __clove_time_t result;
    result.seconds = t1->seconds + t2->seconds;
    result.nanos_after_seconds = t1->nanos_after_seconds + t2->nanos_after_seconds;
    if (result.nanos_after_seconds >= __CLOVE_TIME_TRASL_NANOS_PER_SEC) {
        result.seconds++;
        result.nanos_after_seconds -= __CLOVE_TIME_TRASL_NANOS_PER_SEC;
    }
    return result;
}

unsigned long long __clove_time_to_millis(__clove_time_t* t) {
    unsigned long long result = 0;
    result += t->seconds * __CLOVE_TIME_TRASL_MILLIS_PER_SEC;
    result += t->nanos_after_seconds / __CLOVE_TIME_TRASL_NANOS_PER_MILLIS;
    return result;
}

unsigned long long __clove_time_to_nanos(__clove_time_t* t) {
    unsigned long long result = 0;
    result += t->seconds * __CLOVE_TIME_TRASL_NANOS_PER_SEC;
    result += t->nanos_after_seconds;
    return result;
}

#ifdef _WIN32
#include <windows.h>
#include <time.h>
__clove_time_t __clove_time_now(void) {
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
__clove_time_t __clove_time_now(void) {
    struct timespec time_data;
    clock_gettime(CLOCK_REALTIME, &time_data);

    __clove_time_t result;
    result.seconds = time_data.tv_sec;
    result.nanos_after_seconds = time_data.tv_nsec;
    return result;
}
#endif //_WIN32
#pragma endregion // Time Impl

#pragma region PRIVATE - Stack Impl
#include <stdbool.h>
#include <stdlib.h>
void __clove_stack_init(__clove_stack_t* stack, size_t initial_capacity) {
    stack->capacity = initial_capacity;
    stack->count = 0;
    stack->item_size = sizeof(size_t);
    stack->items = (unsigned char*)__clove_memory_malloc(stack->item_size * stack->capacity);
}

bool __clove_stack_is_empty(__clove_stack_t* stack) {
    return stack->count == 0;
}

void __clove_stack_push(__clove_stack_t* stack, size_t item) {
    if (stack->count == stack->capacity) {
        stack->capacity *= 2;
        stack->items = (unsigned char*)__clove_memory_realloc(stack->items, stack->item_size * stack->capacity);
    }
    size_t byte_index = stack->count * stack->item_size;
    size_t* item_ptr = (size_t*)&(stack->items[byte_index]);
    *item_ptr = item;
    stack->count++;
}

size_t __clove_stack_pop(__clove_stack_t* stack) {
    if (stack->count == 0) return 0; //shouldn't happen

    size_t byte_index = (stack->count - 1) * stack->item_size;
    size_t* item_ptr = (size_t*)&(stack->items[byte_index]);
    stack->count--;
    return *item_ptr;
}

void __clove_stack_free(__clove_stack_t* stack) {
    if (!stack) return;
    free(stack->items);
    stack->items = NULL;
    stack->capacity = 0;
    stack->count = 0;
    stack->item_size = 0;
}
#pragma endregion // Stack Impl

#pragma region PRIVATE - Vector Impl
#include <stdlib.h>
#include <stdint.h>
__clove_vector_params_t __clove_vector_params_defaulted(size_t item_size) {
    __clove_vector_params_t params;
    params.item_size = item_size;
    params.initial_capacity = 10;
    params.item_ctor = NULL;
    params.item_dtor = NULL;
    return params;
}

__clove_vector_t __clove_vector_null(void) {
    __clove_vector_t v;
    v.capacity = 0;
    v.item_size = 0;
    v.count = 0;
    v.items = NULL;
    v.item_ctor = NULL;
    v.item_dtor = NULL;
    v.swap_temp = NULL;
    return v;
}

void __clove_vector_init(__clove_vector_t* vector, __clove_vector_params_t* params) {
    vector->capacity = params->initial_capacity;
    vector->count = 0;
    vector->item_size = params->item_size;
    vector->items = (unsigned char*)__clove_memory_malloc(vector->item_size * vector->capacity);
    vector->item_ctor = params->item_ctor;
    vector->item_dtor = params->item_dtor;
    vector->swap_temp = __clove_memory_malloc(vector->item_size);
}

size_t __clove_vector_count(const __clove_vector_t* vector) {
    return vector->count;
}

bool __clove_vector_is_empty(const __clove_vector_t* vector) {
    return vector->count == 0;
}

void* __clove_vector_add_slot(__clove_vector_t* vector) {
    if (vector->count == vector->capacity) {
        vector->capacity *= 2;
        vector->items = (unsigned char*)__clove_memory_realloc(vector->items, vector->item_size * vector->capacity);
    }
    size_t byte_index = vector->count * vector->item_size;
    vector->count++;
    void* item = (void*)&(vector->items[byte_index]);
    if (vector->item_ctor) vector->item_ctor(item);
    return item;
}

void __clove_vector_add_all(__clove_vector_t* vector, const __clove_vector_t* other) {
    if (vector->item_size != other->item_size) return;
    
    size_t vector_free_slots = vector->capacity - vector->count;
    if (vector_free_slots < other->count)
    {
        vector->capacity = vector->count + other->count;
        vector->items = (unsigned char*)__clove_memory_realloc(vector->items, vector->item_size * vector->capacity);
    }
    
    size_t byte_index = vector->count * vector->item_size;
    
    void* dest = (void*)&(vector->items[byte_index]);
    size_t dest_size = (vector->capacity - vector->count) * vector->item_size;
    void* src = other->items;
    size_t src_size =  other->count * other->item_size;
    __clove_memory_memcpy(dest, dest_size, src, src_size);
    
    vector->count += other->count;
}

void* __clove_vector_get(const __clove_vector_t* vector, size_t index) {
    //if (index < 0) return NULL; //can never happen bucause of size_t
    if (index >= vector->count) return NULL;
    size_t byte_index = index * vector->item_size;
    return (void*)&(vector->items[byte_index]);
}

void __clove_vector_set(__clove_vector_t* vector, size_t index, void* item) {
    void* found = __clove_vector_get(vector, index);
    if (!found) return;
    __clove_memory_memcpy(found, vector->item_size, item, vector->item_size);
}

void __clove_vector_free(__clove_vector_t* vector) {
    if (vector->item_dtor) {
        for (size_t i = 0; i < vector->count; ++i) {
            void* item = __clove_vector_get(vector, i);
            vector->item_dtor(item);
        }
        vector->item_dtor = NULL;
    }

    if (vector->items) {
        free(vector->items);
        vector->items = NULL;
    }

    if (vector->swap_temp) {
        free(vector->swap_temp);
        vector->swap_temp = NULL;
    }
    vector->capacity = 0;
    vector->count = 0;
}

void __clove_vector_swap(__clove_vector_t* vector, size_t index1, size_t index2) {
    void* curr = __clove_vector_get(vector, index1);
    void* next = __clove_vector_get(vector, index2);
    if (!curr || !next) return;
    __clove_memory_memcpy(vector->swap_temp, vector->item_size, curr, vector->item_size);
    __clove_vector_set(vector, index1, next);
    __clove_vector_set(vector, index2, vector->swap_temp);
}

//QuickSort
size_t __clove_vector_quicksort_partition(__clove_vector_t* vector, int (*comparator)(void*, void*), size_t start_index, size_t end_index) {
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

void __clove_vector_quicksort_iterative(__clove_vector_t* vector, int (*comparator)(void*, void*), size_t start_index, size_t end_index) {
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

void __clove_vector_sort(__clove_vector_t* vector, int (*comparator)(void*, void*)) {
    if (vector->count <= 1) return;
    __clove_vector_quicksort_iterative(vector, comparator, 0, vector->count - 1);
}

void __clove_vector_collection_dtor(void* vector) {
    __clove_vector_t* vector_ptr = (__clove_vector_t*)vector;
    __clove_vector_free(vector_ptr);
}
#pragma endregion // Vector Impl

#pragma region PRIVATE - Map Impl
#include <stdlib.h>
// Bernstein classic hash
size_t __clove_map_hash_djb33x(void *key, size_t keylen) {
    size_t hash = 5381;
    unsigned char *key_as_num = (unsigned char *)key;
    for (size_t i = 0; i < keylen; i++) {
        hash = ((hash << 5) + hash) ^ key_as_num[i];
    }
    return hash;
}

__clove_map_params_t __clove_map_params_defaulted(void) 
{
    __clove_map_params_t params;
    params.initial_hash_size = 10;
    params.hash_funct = __clove_map_hash_djb33x;
    params.item_dtor = NULL;
    return params;
}

void __clove_map_init(__clove_map_t* map, __clove_map_params_t* params) {
    map->hashmap_size = params->initial_hash_size;
    map->hashmap = __CLOVE_MEMORY_CALLOC_TYPE_N(__clove_map_node_t*, map->hashmap_size);
    map->hash_funct = params->hash_funct;
    map->item_dtor = params->item_dtor;
    map->count = 0;
}

void __clove_map_free(__clove_map_t* map) {
    for(size_t i=0; i < map->hashmap_size; ++i) {
        if (!map->hashmap[i]) continue;
        __clove_map_node_t* node = map->hashmap[i];

        __clove_map_node_t* current = node;
        while(current) {
            __clove_map_node_t* next = current->next;

            free(current->key);
            if (map->item_dtor) {
                //if dtor set, means map become owner of the item (and its memory)
                map->item_dtor(current->value);
                free(current->value);
            }
            free(current);

            current = next;
        }
    }
    free(map->hashmap);
    map->hashmap = NULL;
    map->hashmap_size = 0;
    map->count = 0;
}

size_t __clove_map_count(__clove_map_t* map) {
    return map->count;
}

bool __clove_map_has_key(__clove_map_t* map, const char* key) {
    size_t key_size = __clove_string_length(key);
    size_t hash = __clove_map_hash_djb33x((void*)key, key_size);
    size_t hash_index = hash % map->hashmap_size;

    __clove_map_node_t* node = map->hashmap[hash_index];
    while(node) {
        if (__clove_string_equal(key, node->key)) return true;
        node = node->next;
    }
    return false;
}

void __clove_map_put(__clove_map_t* dict, const char* key, void* value) {
    size_t key_size = __clove_string_length(key);
    size_t hash = dict->hash_funct((void*)key, key_size);
    size_t hash_index = hash % dict->hashmap_size;
    //Scenario 1: hash(Key) not present
    if (!dict->hashmap[hash_index]) {
        __clove_map_node_t* node = __CLOVE_MEMORY_MALLOC_TYPE(__clove_map_node_t);
        node->key = __clove_string_strdup(key);
        node->key_size = key_size;
        node->value = value;
        node->next = NULL;

        dict->hashmap[hash_index] = node;
        dict->count++;
    } else { //Scenario 2: hash(Key) already present 
        __clove_map_node_t* node = dict->hashmap[hash_index];
        __clove_map_node_t* last = NULL;
        
        //Scenario 2.1: key already exists
        while(node) {
            last = node;
            if (node->key_size == key_size && __clove_string_equal(key, node->key)) {
                node->value = value;
                return;
            }
            node = node->next;
        }

        //Scenario 2.2: key not exists
        __clove_map_node_t* new_node = __CLOVE_MEMORY_MALLOC_TYPE(__clove_map_node_t);
        new_node->key = __clove_string_strdup(key);
        new_node->key_size = key_size;
        new_node->value = value;
        new_node->next = NULL;

        last->next = new_node;
        dict->count++;
    }

}

void* __clove_map_get(__clove_map_t* map, const char* key) {
    size_t key_size = __clove_string_length(key);
    size_t hash = map->hash_funct((void*)key, key_size);
    size_t hash_index = hash % map->hashmap_size;

    __clove_map_node_t* node = map->hashmap[hash_index];
    while(node) {
        if (__clove_string_equal(key, node->key)) return node->value;
        node = node->next;
    }
    return NULL;
}
#pragma endregion // Map Impl

#pragma region PRIVATE - CommandLine Impl
#include <stdbool.h>
#include <string.h>
bool __clove_cmdline_next_opt(__clove_cmdline_t* cmdline, const char** opt_out) {
   while(cmdline->arg_index < cmdline->argc) {
        const char* current = cmdline->argv[cmdline->arg_index];
        cmdline->arg_index++;
        if (__clove_string_startswith(current, "--") && __clove_string_length(current) > 3) {
            *opt_out = current + 2;
            return true;
        } else if (__clove_string_startswith(current, "-") && __clove_string_length(current) == 2) {
            *opt_out = current + 1;
            return true;
        }
   }
   return false;
}

bool __clove_cmdline_next_arg(__clove_cmdline_t* cmdline, const char** arg_out) {
    if (cmdline->arg_index >= cmdline->argc) return false;
    const char* arg = cmdline->argv[cmdline->arg_index];
    if (__clove_string_startswith(arg, "-")) return false; 
    *arg_out = arg;
    cmdline->arg_index++;
    return true;
}

void __clove_cmdline_init(__clove_cmdline_t* cmd, const char** argv, int argc) {
    cmd->argv = argv;
    cmd->argc = argc;
    cmd->arg_index = 1;

    __clove_map_params_t params = __clove_map_params_defaulted();
    params.item_dtor = __clove_vector_collection_dtor;
    __clove_map_init(&cmd->map, &params);

    const char* opt;
    while(__clove_cmdline_next_opt(cmd, &opt)) {
        const char* arg = NULL;
        __clove_cmdline_next_arg(cmd, &arg);
        __clove_cmdline_add_opt(cmd, opt, arg);
    }
}

void __clove_cmdline_free(__clove_cmdline_t* cmd) {
    __clove_map_free(&(cmd->map));
    cmd->argv = NULL;
    cmd->argc = 0;
    cmd->arg_index = 0;
}

void __clove_cmdline_add_opt(__clove_cmdline_t* cmd, const char* opt, const char* value) {
    __clove_vector_t* values;
    if (__clove_map_has_key(&(cmd->map), opt)) {
        values = (__clove_vector_t*)__clove_map_get(&(cmd->map), opt);
    } else {
        values = __CLOVE_MEMORY_MALLOC_TYPE(__clove_vector_t);
        __CLOVE_VECTOR_INIT(values, char*);
        __clove_map_put(&(cmd->map), opt, values);
    }
    const char* *slot = (const char* *)__clove_vector_add_slot(values);
    *slot = value;
}

bool __clove_cmdline_has_opt(__clove_cmdline_t* cmdline, const char* opt) {
    return __clove_map_has_key(&(cmdline->map), opt);
}
const char* __clove_cmdline_get_opt_value(__clove_cmdline_t* cmdline, const char* opt) {
    const __clove_vector_t* values = __clove_cmdline_get_opt_values(cmdline, opt);
    if (!values || __clove_vector_count(values) == 0) return NULL;
    return *(char**)__clove_vector_get(values, 0);
}
const __clove_vector_t* __clove_cmdline_get_opt_values(__clove_cmdline_t* cmdline, const char* opt) {
    return (__clove_vector_t*)__clove_map_get(&(cmdline->map), opt);
}

bool __clove_cmdline_has_any_opt(__clove_cmdline_t* cmdline, const char* opt1, const char* opt2) {
    return __clove_cmdline_has_opt(cmdline, opt1) || __clove_cmdline_has_opt(cmdline, opt2);
}

const char* __clove_cmdline_get_any_opt_value(__clove_cmdline_t* cmdline, const char* opt1, const char* opt2) {
    const char* result = __clove_cmdline_get_opt_value(cmdline, opt1);
    if (result) return result;
    return __clove_cmdline_get_opt_value(cmdline, opt2);
}

const char* __clove_cmdline_get_any_opt_value_defaulted(__clove_cmdline_t* cmdline, const char* opt1, const char* opt2, const char* default_value) {
    const char* result = __clove_cmdline_get_any_opt_value(cmdline, opt1, opt2);
    if (result) return result;
    return default_value;
}

__clove_cmdline_errno_t __clove_cmdline_handle_help(__clove_cmdline_t* cmd) {
    if (!__clove_cmdline_has_any_opt(cmd, "h", "help")) return __CLOVE_CMD_ERRNO_UNMANAGED;    
    printf("CLove-Unit v%s\n", __CLOVE_VERSION); 
    printf("usage:\n");
    printf("%*s<executable> [options]\n", 3," ");
    printf("where options are:\n");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"<no-options>",             5," ", "Run all tests producing a 'pretty' print report (default behaviour).");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"-b, --base-path",          5," ", "Base path for test sources. Allow to shorten test file paths when running/listing tests.");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"-d, --run-detail <level>", 5," ", "Control Run Tests report detail level: '1' (failed), '2' (failed+skipped), '3' (passed+failed+skipped). Default is '3'.");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"-e, --exclude <expr>",     5," ", "Suite/Test expression to be excluded. Works when running/listing tests.");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"-h, --help",               5," ", "Display usage information.");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"-i, --include <expr>",     5," ", "Suite/Test expression to be included. Works when running/listing tests.");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"-l, --list-tests",         5," ", "List all/matching test cases in 'pretty' format (default).");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"-o, --output <stream>",    5," ", "Specify output stream for a report: 'stdout' (default) or <file path>.");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"-r, --report <format>",    5," ", "Specify report format when running tests: 'pretty', 'csv', 'json'.");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"-t, --run-tests",          5," ", "Execute all/matching test cases (same as <no-options>).");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"-v, --version",            5," ", "Show CLove-Unit version.");
    printf("%*s%-*s%*s%s\n", 3," ", 30,"-x, --error-on-test-fail", 5," ", "Run Tests process will end with error in case of test failure. Default is to end the process succesfully.");
    printf("\n");
    printf("For detailed usage please look at the README in https://github.com/fdefelici/clove-unit.\n");
    return __CLOVE_CMD_ERRNO_OK;
}

__clove_cmdline_errno_t __clove_cmdline_handle_version(__clove_cmdline_t* cmd) {
    if (!__clove_cmdline_has_any_opt(cmd, "v", "version")) return __CLOVE_CMD_ERRNO_UNMANAGED;
    printf("%s", __CLOVE_VERSION); //to avoid new_line character(s)
    return __CLOVE_CMD_ERRNO_OK;
}

__clove_cmdline_errno_t __clove_cmdline_handle_run_tests(__clove_cmdline_t* cmd) {
    if (!__clove_cmdline_has_any_opt(cmd, "t", "run-tests")) return __CLOVE_CMD_ERRNO_UNMANAGED;
    
    const char* opt_report = __clove_cmdline_get_any_opt_value_defaulted(cmd, "r", "report", "pretty");
    if (!__clove_string_equal_any(opt_report, 3, "pretty", "json", "csv")) return __CLOVE_CMD_ERRNO_INVALID_PARAM;
    
    const char* opt_detail = __clove_cmdline_get_any_opt_value_defaulted(cmd, "d", "run-detail", "3");
    if (!__clove_string_equal_any(opt_detail, 3, "1", "2", "3")) return __CLOVE_CMD_ERRNO_INVALID_PARAM;    
    
    const char* opt_out = __clove_cmdline_get_any_opt_value_defaulted(cmd, "o", "output", "stdout");
    const char* opt_base_path = __clove_cmdline_get_any_opt_value_defaulted(cmd, "b", "base-path", "");
    const bool opt_enable_error = __clove_cmdline_has_any_opt(cmd, "x", "error-on-test-fail");

     __clove_vector_t includes;
    __clove_cmdline_create_test_expr(cmd, "i", "include", &includes);

    __clove_vector_t excludes;
    __clove_cmdline_create_test_expr(cmd, "e", "exclude", &excludes);
    
    //Select stream
    __clove_stream_t* stream;
    if (__clove_string_equal(opt_out, "stdout")) {
        stream = (__clove_stream_t*)__clove_stream_console_new();
    } else { // file path
        const char* report_path;
        if (__clove_path_is_relative(opt_out)) {
            report_path = __clove_path_rel_to_abs_exec_path(opt_out);
        }
        else {
            report_path = opt_out;
        } 
        stream = (__clove_stream_t*)__clove_stream_file_new(report_path);
    }

    //Select Report
    __clove_report_params_t report_params;
    
    //ensure base path is in os format
    char* base_path_fixed = __clove_string_strdup(opt_base_path);
    __clove_path_to_os(base_path_fixed);    
    report_params.tests_base_path = base_path_fixed;

    if (__clove_string_equal(opt_detail, "1")) 
        report_params.report_detail = __CLOVE_REPORT_DETAIL__FAILED;
    else if (__clove_string_equal(opt_detail, "2"))
        report_params.report_detail = __CLOVE_REPORT_DETAIL__FAILED_SKIPPED;
    else if (__clove_string_equal(opt_detail, "3"))
        report_params.report_detail = __CLOVE_REPORT_DETAIL__PASSED_FAILED_SKIPPED;

    __clove_report_t* report;
    if (__clove_string_equal("json", opt_report)) {
        report = (__clove_report_t*)__clove_report_run_tests_json_new(stream, &report_params);
    } else if (__clove_string_equal("pretty", opt_report)) {
        report = (__clove_report_t*)__clove_report_run_tests_pretty_new(stream, &report_params);
    } else if (__clove_string_equal("csv", opt_report)) {
        report = (__clove_report_t*)__clove_report_run_tests_csv_new(stream, &report_params);
    } else {
        //Just to avoid compile warnings. This can never happen because of validation did before.
        return __CLOVE_CMD_ERRNO_UNMANAGED;
    }
    
    //Run report
    int run_result = __clove_run_tests_with_report(report, &includes, &excludes);
    
    //Clean
    report->free(report);
    stream->free(stream);
    __clove_vector_free(&includes);
    __clove_vector_free(&excludes);
    free(base_path_fixed);

    //Result
    if (run_result == 1) return __CLOVE_CMD_ERRNO_GENERIC;
    if (run_result == 2 && opt_enable_error) return __CLOVE_CMD_ERRNO_GENERIC;
    return __CLOVE_CMD_ERRNO_OK;
}

__clove_cmdline_errno_t __clove_cmdline_handle_default(__clove_cmdline_t* cmd) {
    __clove_cmdline_add_opt(cmd, "t", NULL);
    return __clove_cmdline_handle_run_tests(cmd);
}

__clove_cmdline_errno_t __clove_cmdline_handle_list_tests(__clove_cmdline_t* cmd) {
    if (!__clove_cmdline_has_any_opt(cmd, "l", "list-tests")) return __CLOVE_CMD_ERRNO_UNMANAGED;
    
    const char* opt_report = __clove_cmdline_get_any_opt_value_defaulted(cmd, "r", "report", "pretty");
    if (!__clove_string_equal_any(opt_report, 3, "pretty", "json", "csv")) return __CLOVE_CMD_ERRNO_INVALID_PARAM;    
    
    const char* opt_out = __clove_cmdline_get_any_opt_value_defaulted(cmd, "o", "output", "stdout");
    const char* opt_base_path = __clove_cmdline_get_any_opt_value_defaulted(cmd, "b", "base-path", "");

     __clove_vector_t includes;
    __clove_cmdline_create_test_expr(cmd, "i", "include", &includes);

    __clove_vector_t excludes;
    __clove_cmdline_create_test_expr(cmd, "e", "exclude", &excludes);

    //ensure base path is in os format
    char* base_path_os = __clove_string_strdup(opt_base_path);
    __clove_path_to_os(base_path_os);    
    __clove_report_params_t report_params;
    report_params.tests_base_path = base_path_os;

    __clove_stream_t* stream;
    if (__clove_string_equal(opt_out, "stdout")) {
        stream = (__clove_stream_t*)__clove_stream_console_new();
    } else {
        const char* report_path;
        if (__clove_path_is_relative(opt_out)) {
            report_path = __clove_path_rel_to_abs_exec_path(opt_out);
        }
        else {
            report_path = opt_out;
        } 
        stream = (__clove_stream_t*)__clove_stream_file_new(report_path);
    }

    //Select Report Format
    __clove_report_list_tests_t* report;
    if (__clove_string_equal("json", opt_report)) {
        report = (__clove_report_list_tests_t*)__clove_report_list_tests_json_new(stream, &report_params);
    } else if (__clove_string_equal("pretty", opt_report)) {
        report = (__clove_report_list_tests_t*)__clove_report_list_tests_pretty_new(stream, &report_params);
    } else if (__clove_string_equal("csv", opt_report)) {
        report = (__clove_report_list_tests_t*)__clove_report_list_tests_csv_new(stream, &report_params);
    } else {
        //Just to avoid compile warnings. This can never happen because of validation did before.
        return __CLOVE_CMD_ERRNO_UNMANAGED;
    }

    int run_result = 0;
     __clove_symbols_context_t context;
    context.includes = &includes;
    context.excludes = &excludes;

    __clove_vector_params_t vector_params = __clove_vector_params_defaulted(sizeof(__clove_suite_t));
    vector_params.item_ctor = __clove_vector_suite_ctor;
    vector_params.item_dtor = __clove_vector_suite_dtor;
    __clove_vector_init(&context.suites, &vector_params);
    context.prefix = "__clove_sym___";
    context.prefix_length = __clove_string_length("__clove_sym___");
    context.suites_count = 0;
    context.tests_count = 0;

    int result = __clove_symbols_for_each_function_by_prefix(&context, __clove_symbols_function_collect);
    if (result == 0) {
        run_result = __clove_cmd_list_test_execute((__clove_suite_t*)(context.suites.items), context.suites_count, context.tests_count, report);
    }
    else {
        run_result = 1;
    }
    report->free(report);
    stream->free(stream);

    free(base_path_os);
    __clove_vector_free(&context.suites);
    __clove_vector_free((__clove_vector_t*)context.includes);
    __clove_vector_free((__clove_vector_t*)context.excludes);

    if (run_result != 0) return __CLOVE_CMD_ERRNO_GENERIC;
    return __CLOVE_CMD_ERRNO_OK;
}

void __clove_cmdline_create_test_expr(__clove_cmdline_t* cmd, const char* opt1, const char* opt2,  __clove_vector_t* expressions) {
    *expressions = __clove_vector_null();
    
    bool has_opt1 = __clove_cmdline_has_opt(cmd, opt1);
    bool has_opt2 = __clove_cmdline_has_opt(cmd, opt2);
    if ( !has_opt1 && !has_opt2) return;
    
    __clove_vector_t values;
    __CLOVE_VECTOR_INIT(&values, char*);

    if (has_opt1) {
        const __clove_vector_t* values1 = __clove_cmdline_get_opt_values(cmd, opt1);
        __clove_vector_add_all(&values, values1);
    }
    if (has_opt2) {
        const __clove_vector_t* values2 = __clove_cmdline_get_opt_values(cmd, opt2);
        __clove_vector_add_all(&values, values2);
    }

    size_t values_count = __clove_vector_count(&values);
    __CLOVE_VECTOR_INIT_CAPACITY(expressions, __clove_test_expr_t, values_count);

    __CLOVE_VECTOR_FOREACH(&values, char*, expr_str, {
        __clove_test_expr_t expr;
        __clove_test_expr_init(&expr, *expr_str);

        __CLOVE_VECTOR_ADD(expressions, __clove_test_expr_t, expr);
    });
    
    __clove_vector_free(&values);
}
#pragma endregion // CommandLine Impl

#pragma region PRIVATE - Test Impl
#include <string.h>
void __clove_vector_test_ctor(void* test) {
    //cast to __clove_test_t* not needed
    __clove_memory_memset(test, sizeof(__clove_test_t), 0);
}

void __clove_vector_test_dtor(void* test_ptr) {
    __clove_test_t* test = (__clove_test_t*)test_ptr;
    free(test->name);

    //See CLOVE_STRING_EQ and CLOVE_STRING_NE where string allocation happen
    if (test->result == __CLOVE_TEST_RESULT_FAILED && test->issue.data_type == __CLOVE_GENERIC_STRING) {
        free(test->issue.expected._string);
        free(test->issue.actual._string);
    }
}
#pragma endregion

#pragma region PRIVATE - Suite Impl
void __clove_vector_suite_ctor(void* suite_ptr) {
    __clove_suite_t* suite = (__clove_suite_t*)suite_ptr;
    suite->name = NULL;
    suite->fixtures.setup_once = __clove_utils_empty_funct;
    suite->fixtures.teardown_once = __clove_utils_empty_funct;
    suite->fixtures.setup = __clove_utils_empty_funct;
    suite->fixtures.teardown = __clove_utils_empty_funct;

    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(__clove_test_t));
    params.item_ctor = __clove_vector_test_ctor;
    params.item_dtor = __clove_vector_test_dtor;
    __clove_vector_init(&(suite->tests), &params);
    suite->test_count = 0;

    suite->issue.duration.seconds = 0;
    suite->issue.duration.nanos_after_seconds = 0;
    suite->issue.passed_count = 0;
    suite->issue.failed_count = 0;
    suite->issue.skipped_count = 0;
}

void __clove_vector_suite_dtor(void* suite_ptr) {
    __clove_suite_t* suite = (__clove_suite_t*)suite_ptr;
    free(suite->name);
    __clove_vector_free(&suite->tests);
}
#pragma endregion // Suite Impl

#pragma region PRIVATE - Assert Impl
#include <math.h>
void __clove_assert_fail(__clove_test_t* _this) {
    _this->result = __CLOVE_TEST_RESULT_FAILED;
    _this->issue.assert = __CLOVE_ASSERT_FAIL;
}

void __clove_assert_pass(__clove_test_t* _this) {
    _this->result = __CLOVE_TEST_RESULT_PASSED;
}

void __clove_assert_int(__clove_assert_check_e check_mode, int expected, int result, __clove_test_t* _this) {
    __CLOVE_ASSERT_INTEGER_CHECK(check_mode, expected, result, __CLOVE_GENERIC_INT, _int, _this)
}

void __clove_assert_uint(__clove_assert_check_e check_mode, unsigned int expected, unsigned int result, __clove_test_t* _this) {
    __CLOVE_ASSERT_INTEGER_CHECK(check_mode, expected, result, __CLOVE_GENERIC_UINT, _uint, _this)
}

void __clove_assert_long(__clove_assert_check_e check_mode, long expected, long result, __clove_test_t* _this) {
    __CLOVE_ASSERT_INTEGER_CHECK(check_mode, expected, result, __CLOVE_GENERIC_LONG, _long, _this)
}

void __clove_assert_ulong(__clove_assert_check_e check_mode, unsigned long expected, unsigned long result, __clove_test_t* _this) {
    __CLOVE_ASSERT_INTEGER_CHECK(check_mode, expected, result, __CLOVE_GENERIC_ULONG, _ulong, _this)
}

void __clove_assert_llong(__clove_assert_check_e check_mode, long long expected, long long result, __clove_test_t* _this) {
    __CLOVE_ASSERT_INTEGER_CHECK(check_mode, expected, result, __CLOVE_GENERIC_LLONG, _llong, _this)
}

void __clove_assert_ullong(__clove_assert_check_e check_mode, unsigned long long expected, unsigned long long result, __clove_test_t* _this) {
    __CLOVE_ASSERT_INTEGER_CHECK(check_mode, expected, result, __CLOVE_GENERIC_ULLONG, _ullong, _this)
}

void __clove_assert_sizet(__clove_assert_check_e check_mode, size_t expected, size_t result, __clove_test_t* _this) {
    __CLOVE_ASSERT_INTEGER_CHECK(check_mode, expected, result, __CLOVE_GENERIC_SIZET, _sizet, _this)
}

void __clove_assert_char(__clove_assert_check_e check_mode, char expected, char result, __clove_test_t* _this) {
    __CLOVE_ASSERT_INTEGER_CHECK(check_mode, expected, result, __CLOVE_GENERIC_CHAR, _char, _this)
}

void __clove_assert_bool(__clove_assert_check_e check_mode, bool expected, bool result, __clove_test_t* _this) {
    __CLOVE_ASSERT_INTEGER_CHECK(check_mode, expected, result, __CLOVE_GENERIC_BOOL, _bool, _this)
}

void __clove_assert_null(__clove_assert_check_e check_mode, void* expected, void* result, __clove_test_t* _this) {
    __CLOVE_ASSERT_INTEGER_CHECK(check_mode, expected, result, __CLOVE_GENERIC_PTR, _ptr, _this)
}

void __clove_assert_ptr(__clove_assert_check_e check_mode, void* expected, void* result, __clove_test_t* _this) {
    __CLOVE_ASSERT_INTEGER_CHECK(check_mode, expected, result, __CLOVE_GENERIC_PTR, _ptr, _this)
}

void __clove_assert_float(__clove_assert_check_e check_mode, float expected, float result, unsigned char precision, __clove_test_t* _this) {
    bool pass_scenario = false;
    if (check_mode == __CLOVE_ASSERT_EQ) { pass_scenario = fabsf(expected - result) <= __clove_math_decimalf(precision); }
    else if (check_mode == __CLOVE_ASSERT_NE) { pass_scenario = fabsf(expected - result) > __clove_math_decimalf(precision); }
    else if (check_mode == __CLOVE_ASSERT_GT)  { pass_scenario = expected > result; }
    else if (check_mode == __CLOVE_ASSERT_GTE) { pass_scenario = expected >= result; }
    else if (check_mode == __CLOVE_ASSERT_LT)  { pass_scenario = expected < result; }
    else if (check_mode == __CLOVE_ASSERT_LTE) { pass_scenario = expected <= result; }
    if (pass_scenario) {
        _this->result = __CLOVE_TEST_RESULT_PASSED;
    }
    else {
        _this->result = __CLOVE_TEST_RESULT_FAILED;
        _this->issue.assert = check_mode;
        _this->issue.data_type = __CLOVE_GENERIC_FLOAT;
        _this->issue.expected._float = expected;
        _this->issue.actual._float = result;
        _this->issue.floating_precision = precision;
    }
}

void __clove_assert_double(__clove_assert_check_e check_mode, double expected, double result, unsigned char precision, __clove_test_t* _this) {
    bool pass_scenario = false;
    if (check_mode == __CLOVE_ASSERT_EQ) { pass_scenario = fabs(expected - result) <= __clove_math_decimald(precision); }
    else if (check_mode == __CLOVE_ASSERT_NE) { pass_scenario = fabs(expected - result) > __clove_math_decimald(precision); }
    else if (check_mode == __CLOVE_ASSERT_GT)  { pass_scenario = expected > result; }
    else if (check_mode == __CLOVE_ASSERT_GTE) { pass_scenario = expected >= result; }
    else if (check_mode == __CLOVE_ASSERT_LT)  { pass_scenario = expected < result; }
    else if (check_mode == __CLOVE_ASSERT_LTE) { pass_scenario = expected <= result; }
    if (pass_scenario) {
        _this->result = __CLOVE_TEST_RESULT_PASSED;
    }
    else {
        _this->result = __CLOVE_TEST_RESULT_FAILED;
        _this->issue.assert = check_mode;
        _this->issue.data_type = __CLOVE_GENERIC_DOUBLE;
        _this->issue.expected._double = expected;
        _this->issue.actual._double = result;
        _this->issue.floating_precision = precision;
    }
}

void __clove_assert_string(__clove_assert_check_e check_mode, const char* expected, const char* result, __clove_test_t* _this) {
    bool pass_scenario = false;
    if (check_mode == __CLOVE_ASSERT_EQ) { pass_scenario = strcmp(expected, result) == 0; }
    else if (check_mode == __CLOVE_ASSERT_NE) { pass_scenario = strcmp(expected, result) != 0; }

    if (pass_scenario) {
        _this->result = __CLOVE_TEST_RESULT_PASSED;
    }
    else {
        _this->result = __CLOVE_TEST_RESULT_FAILED;
        _this->issue.assert = check_mode;
        _this->issue.data_type = __CLOVE_GENERIC_STRING;
        _this->issue.expected._string = __clove_string_strdup(expected); //in case is allocated on the stack within the test function
        _this->issue.actual._string = __clove_string_strdup(result); //in case is allocated on the stack within the test function
    }
}
#pragma endregion // Assert Impl

#pragma region PRIVATE - Stream Impl
__clove_stream_console_t* __clove_stream_console_new(void) {
    __clove_stream_console_t* stream = __CLOVE_MEMORY_MALLOC_TYPE(__clove_stream_console_t);
    stream->base.open = __clove_stream_console_open;
    stream->base.close = __clove_stream_console_close;
    stream->base.writef = __clove_stream_console_writef;
    stream->base.seek = __clove_stream_console_seek;
    stream->base.has_ansi_support = __clove_stream_console_has_ansi_support;
    stream->base.free = __clove_stream_console_free;
    return stream;
}
bool __clove_stream_console_open(__clove_stream_t* stream) {
    __CLOVE_UNUSED_VAR(stream);
    return true;
}
void __clove_stream_console_close(__clove_stream_t* stream) { 
    __CLOVE_UNUSED_VAR(stream);
    //nothing todo
}
void __clove_stream_console_writef(__clove_stream_t* stream, const char* format, ...) {
    __CLOVE_UNUSED_VAR(stream);
    
    va_list args;
    va_start(args, format);
    __clove_console_vprintf(format, args);
    va_end(args);
}
void __clove_stream_console_seek(__clove_stream_t* stream, long offset, int origin) {
    __CLOVE_UNUSED_VAR(stream);
    __CLOVE_UNUSED_VAR(offset);
    __CLOVE_UNUSED_VAR(origin);
    //nothing todo
}

#ifdef _WIN32
#include <windows.h>
#include <stdbool.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif
bool __clove_stream_console_has_ansi_support(__clove_stream_t* stream) {
    __CLOVE_UNUSED_VAR(stream);

    DWORD outMode = 0, inMode = 0;
    HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE stdinHandle = GetStdHandle(STD_INPUT_HANDLE);

    if (stdoutHandle == INVALID_HANDLE_VALUE || stdinHandle == INVALID_HANDLE_VALUE) {
        //exit(GetLastError());
        return false;
    }

    if (!GetConsoleMode(stdoutHandle, &outMode) || !GetConsoleMode(stdinHandle, &inMode)) {
        //exit(GetLastError());
        return false;
    }

    // Enable ANSI escape codes
    outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    // Set stdin as no echo and unbuffered
    inMode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);

    if (!SetConsoleMode(stdoutHandle, outMode) || !SetConsoleMode(stdinHandle, inMode)) {
        //exit(GetLastError());
        return false;
    }
    return true;
}
#else
#include <stdbool.h>
#include <unistd.h>
bool __clove_stream_console_has_ansi_support(__clove_stream_t* stream) {
    __CLOVE_UNUSED_VAR(stream);

    if (isatty(STDOUT_FILENO)) {
        // standard output is a tty
        return true;
    }
    return false;
}
#endif //_WIN32


void __clove_stream_console_free(__clove_stream_t* stream) {
    free(stream);
}

__clove_stream_file_t* __clove_stream_file_new(const char* file_path) {
    __clove_stream_file_t* stream = __CLOVE_MEMORY_MALLOC_TYPE(__clove_stream_file_t);
    stream->base.open = __clove_stream_file_open;
    stream->base.close = __clove_stream_file_close;
    stream->base.writef = __clove_stream_file_writef;
    stream->base.seek = __clove_stream_file_seek;
    stream->base.has_ansi_support = __clove_stream_file_has_ansi_support;
    stream->base.free = __clove_stream_file_free;
    stream->file_path = __clove_string_strdup(file_path);
    stream->file = NULL;
    return stream;
}
bool __clove_stream_file_open(__clove_stream_t* stream) {
    __clove_stream_file_t* _this = (__clove_stream_file_t*)stream;
    _this->file = __clove_file_open(_this->file_path, "wb"); //binary mode so \n will stay \n (and not converted to \r\n on windows)
    return _this->file != NULL;
}
void __clove_stream_file_close(__clove_stream_t* stream) {
    __clove_stream_file_t* _this = (__clove_stream_file_t*)stream;
    __clove_file_close(_this->file);
}
void __clove_stream_file_writef(__clove_stream_t* stream, const char* format, ...) {
    __clove_stream_file_t* _this = (__clove_stream_file_t*)stream;
    va_list args;
    va_start(args, format);
    __clove_file_vprintf(_this->file, format, args);
    va_end(args);
}
void __clove_stream_file_seek(__clove_stream_t* stream, long offset, int origin) {
     __clove_stream_file_t* _this = (__clove_stream_file_t*)stream;
    fseek(_this->file, offset, origin); //TODO: wrap into __clove_file_seek method
}
bool __clove_stream_file_has_ansi_support(struct __clove_stream_t* _this) {
    __CLOVE_UNUSED_VAR(_this);
    return false;
}
void __clove_stream_file_free(__clove_stream_t* stream) {
    __clove_stream_file_t* _this = (__clove_stream_file_t*)stream;
    _this->file = NULL;
    free((char*)_this->file_path);
    free(_this);
}

//In Memory Stream
__clove_stream_memory_t* __clove_stream_memory_new(void) {
    __clove_stream_memory_t* stream = __CLOVE_MEMORY_MALLOC_TYPE(__clove_stream_memory_t);
    stream->base.open = __clove_stream_memory_open;
    stream->base.close = __clove_stream_memory_close;
    stream->base.writef = __clove_stream_memory_writef;
    stream->base.seek = __clove_stream_memory_seek;
    stream->base.has_ansi_support = __clove_stream_memory_has_ansi_support;
    stream->base.free = __clove_stream_memory_free;
    __CLOVE_VECTOR_INIT(&stream->lines, char*);
    return stream;
}
bool __clove_stream_memory_open(__clove_stream_t* stream) {
    __CLOVE_UNUSED_VAR(stream);
    return true;
}
void __clove_stream_memory_close(__clove_stream_t* stream) { 
    __CLOVE_UNUSED_VAR(stream);
    //nothing todo
}
void __clove_stream_memory_writef(__clove_stream_t* stream, const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    char* line = __CLOVE_MEMORY_CALLOC_TYPE_N(char, 1000); //TODO: Better computing real line size.
    __clove_string_vsprintf(line, 1000, format, args);
    __CLOVE_VECTOR_ADD(&((__clove_stream_memory_t*)stream)->lines, char*, line);

    va_end(args);
}
void __clove_stream_memory_seek(__clove_stream_t* stream, long offset, int origin) {
    __CLOVE_UNUSED_VAR(stream);
    __CLOVE_UNUSED_VAR(offset);
    __CLOVE_UNUSED_VAR(origin);
    //nothing todo
}

bool __clove_stream_memory_has_ansi_support(__clove_stream_t* stream) {
    __CLOVE_UNUSED_VAR(stream);
    return false;
}

void __clove_stream_memory_free(__clove_stream_t* stream) {
    __clove_vector_free(&((__clove_stream_memory_t*)stream)->lines);
    free(stream);
}

char* __clove_stream_memory_get_line(__clove_stream_memory_t* mem_stream, size_t index) {
    return (char*)__clove_vector_get(&mem_stream->lines, index);
}

char* __clove_stream_memory_as_string(__clove_stream_memory_t* mem_stream) {
    size_t buffer_size = 0;
    __CLOVE_VECTOR_FOREACH(&mem_stream->lines, char*, line, {
       buffer_size += __clove_string_length(*line);
    });

    char* buffer = __CLOVE_MEMORY_CALLOC_TYPE_N(char, buffer_size + 1);
    __CLOVE_VECTOR_FOREACH(&mem_stream->lines, char*, line, {
       __clove_string_strcat(buffer, buffer_size + 1, *line);
    });
    return buffer;
}
#pragma endregion //Stream Impl

#pragma region PRIVATE - Report Impl
void __clove_test_expr_init(__clove_test_expr_t* expr,  const char* expr_str) {
    const char* dot_begin = __clove_string_strstr(expr_str, ".");
    if (dot_begin) {
        const size_t expr_len = __clove_string_length(expr_str);
        const size_t suite_len = dot_begin - expr_str;
        const size_t test_len = expr_str + expr_len-1 - dot_begin;
        if (suite_len == 0) expr->suite_view = __clove_string_view_from_str("");
        else expr->suite_view = __clove_string_view_from_be(expr_str, dot_begin - 1);

        if (test_len == 0) expr->test_view = __clove_string_view_from_str("");
        else expr->test_view = __clove_string_view_from_str(dot_begin + 1);
    } else {
        expr->suite_view = __clove_string_view_from_str(expr_str);
        expr->test_view = __clove_string_view_from_str("*");
    }
}

bool __clove_test_expr_validate_vw(const __clove_string_view_t* match, const __clove_string_view_t* view) {
    if (__clove_string_view_strequals(match, "*")) return true;
    const size_t match_len = __clove_string_view_length(match);
    if (match_len == 0) return false;
    const char* match_start = __clove_string_view_begin(match);
    const char* match_end = __clove_string_view_end(match);

    bool start_asterisk = match_start[0] == '*';
    bool end_asterisk = match_end[0] == '*';
    if (start_asterisk && end_asterisk) {
        if (match_len <= 2) return false;
        __clove_string_view_t fixture = __clove_string_view_from_be(match_start+1, match_end-1);
        return __clove_string_view_contains(view, &fixture);
    }
    if (start_asterisk) {
        return __clove_string_view_nendswith(view, match, 1);
    }
    if (end_asterisk) {
        return __clove_string_view_ncmp(match, view, __clove_string_view_length(match)-1);
    }
    return __clove_string_view_equals(match, view);
}

bool __clove_test_expr_validate(__clove_test_expr_t* expr, const __clove_string_view_t* suite, const __clove_string_view_t* test) {
    if (!__clove_test_expr_validate_vw(&(expr->suite_view), suite)) return false;
    if (!__clove_test_expr_validate_vw(&(expr->test_view), test)) return false;
    return true;
}
#pragma endregion // Report Impl

#pragma region PRIVATE - RunTests Report Pretty Impl
#include <stdio.h>
__clove_report_pretty_t* __clove_report_run_tests_pretty_new(__clove_stream_t* stream, __clove_report_params_t* params) {
    __clove_report_pretty_t* result = __CLOVE_MEMORY_MALLOC_TYPE(__clove_report_pretty_t);
    result->base.start = __clove_report_pretty_start;
    result->base.begin_suite = __clove_report_pretty_begin_suite;
    result->base.end_suite = __clove_report_pretty_end_suite;
    result->base.end = __clove_report_pretty_end;
    result->base.end_test = __clove_report_pretty_end_test;
    result->base.free = __clove_report_pretty_free;
    result->stream = stream;
    result->params = params;
    return result;
}

void __clove_report_pretty_free(__clove_report_t* report) {
    free(report);
}

void __clove_report_pretty_start(__clove_report_t* _this, size_t suite_count, size_t test_count) {
    __clove_report_pretty_t* report = (__clove_report_pretty_t*)_this;
    report->start_time = __clove_time_now();
    report->max_test_digits = (unsigned int)snprintf(NULL, 0U, "%zu", test_count);

    bool activated = report->stream->has_ansi_support(report->stream);
    if (activated) {
        report->labels.info = "[\x1b[1;34mINFO\x1b[0m]";
        report->labels.warn = "[\x1b[33mWARN\x1b[0m]";
        report->labels.erro = "[\x1b[1;31mERRO\x1b[0m]";
        report->labels.pass = "[\x1b[1;32mPASS\x1b[0m]";
        report->labels.skip = "[\x1b[33mSKIP\x1b[0m]";
        report->labels.fail = "[\x1b[1;31mFAIL\x1b[0m]";
    }
    else {
        report->labels.info = "[INFO]";
        report->labels.warn = "[WARN]";
        report->labels.erro = "[ERRO]";
        report->labels.pass = "[PASS]";
        report->labels.skip = "[SKIP]";
        report->labels.fail = "[FAIL]";
    }

    const char* level_str = "<unknown>";
    if (report->params->report_detail == __CLOVE_REPORT_DETAIL__PASSED_FAILED_SKIPPED) {
        level_str = "Full";
    } else if (report->params->report_detail == __CLOVE_REPORT_DETAIL__FAILED) {
        level_str = "Failed only";
    } else if (report->params->report_detail == __CLOVE_REPORT_DETAIL__FAILED_SKIPPED) {
        level_str = "Failed + Skipped";
    }

    report->stream->open(report->stream);
    report->stream->writef(report->stream, "%s Executing Test Runner with detail level: '%s'\n", report->labels.info, level_str);
    report->stream->writef(report->stream, "%s Suites / Tests found: %zu / %zu\n", report->labels.info, suite_count, test_count);
}
void __clove_report_pretty_begin_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index) {
    __CLOVE_UNUSED_VAR(_this);
    __CLOVE_UNUSED_VAR(suite);
    __CLOVE_UNUSED_VAR(index);
    //nothing todo
}
void __clove_report_pretty_end_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index) {
    __CLOVE_UNUSED_VAR(_this);
    __CLOVE_UNUSED_VAR(suite);
    __CLOVE_UNUSED_VAR(index);
    //nothing todo
}
void __clove_report_pretty_end(__clove_report_t* _this, size_t test_count, size_t passed, size_t skipped, size_t failed) {
    __clove_report_pretty_t* report = (__clove_report_pretty_t*)_this;
    __clove_time_t end_time = __clove_time_now();
    __clove_time_t diff = __clove_time_sub(&end_time, &(report->start_time));
    unsigned long long millis = __clove_time_to_millis(&diff);

    report->stream->writef(report->stream, "%s Total: %zu, Passed: %zu, Failed: %zu, Skipped: %zu\n", report->labels.info, test_count, passed, failed, skipped);
    report->stream->writef(report->stream, "%s Run duration: %llu ms\n", report->labels.info, millis);
    if (passed == test_count) { report->stream->writef(report->stream, "%s Run result: SUCCESS :-)\n", report->labels.info); }
    else if (failed > 0) { report->stream->writef(report->stream, "%s Run result: FAILURE :_(\n", report->labels.erro); }
    else if (skipped > 0) { report->stream->writef(report->stream, "%s Run result: OK, but some test has been skipped!\n", report->labels.warn); }

    report->stream->close(report->stream);
}

void __clove_report_pretty_end_test(__clove_report_t* _this, __clove_suite_t* suite, __clove_test_t* test, size_t test_number) {
    __clove_report_pretty_t* report = (__clove_report_pretty_t*)_this;
    
    bool print_passed = false;
    bool print_failed = false;
    bool print_skipped = false;
    if (report->params->report_detail == __CLOVE_REPORT_DETAIL__PASSED_FAILED_SKIPPED) {
        print_passed = true;
        print_failed = true;
        print_skipped = true;
    } else if (report->params->report_detail == __CLOVE_REPORT_DETAIL__FAILED) {
        print_passed = false;
        print_failed = true;
        print_skipped = false;
    } else if (report->params->report_detail == __CLOVE_REPORT_DETAIL__FAILED_SKIPPED) {
        print_passed = false;
        print_failed = true;
        print_skipped = true;
    }

    if (print_passed && test->result == __CLOVE_TEST_RESULT_PASSED) {
        float millis = (float)(__clove_time_to_nanos(&(test->duration))) / (float)__CLOVE_TIME_TRASL_NANOS_PER_MILLIS;
        int decimal = millis > 1.f ? 0 : 3;

        char result[__CLOVE_STRING_LENGTH], strToPad[__CLOVE_TEST_ENTRY_LENGTH];
        snprintf(strToPad, __CLOVE_TEST_ENTRY_LENGTH, "%0*zu) %s.%s", report->max_test_digits, test_number, suite->name, test->name);
        __clove_report_pretty_pad_right(result, strToPad);

        report->stream->writef(report->stream, "%s %s%s (%.*f ms)\n", report->labels.info, result, report->labels.pass, decimal, millis);
    }
    else if (print_failed && test->result == __CLOVE_TEST_RESULT_FAILED) {
        char msg[__CLOVE_STRING_LENGTH] = "FAILURE but NO MESSAGE!!!";

        if (test->issue.assert == __CLOVE_ASSERT_FAIL) {
            __clove_string_sprintf(msg, sizeof(msg), "%s", "A FAIL assertion was met!");
        }
        else {
            __CLOVE_SWITCH_BEG(test->issue.data_type)
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_BOOL) {
                    const char* exp = test->issue.expected._bool ? "true" : "false";
                    const char* act = test->issue.actual._bool ? "true" : "false";
                    //__clove_string_sprintf(msg, sizeof(msg), "expected [%s] but was [%s]", exp, act);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, "%s");
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_CHAR) {
                    //const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                    const char exp = test->issue.expected._char;
                    const char act = test->issue.actual._char;
                    //__clove_string_sprintf(msg, sizeof(msg), "%sexpected [%c] but was [%c]", non, exp, act);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, "%c");
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_INT) {
                    //const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                    const int exp = test->issue.expected._int;
                    const int act = test->issue.actual._int;
                    //__clove_string_sprintf(msg, sizeof(msg), "%sexpected [%d] but was [%d]", non, exp, act);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, "%d");
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_UINT) {
                    //const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                    const unsigned int exp = test->issue.expected._uint;
                    const unsigned int act = test->issue.actual._uint;
                    //__clove_string_sprintf(msg, sizeof(msg), "%sexpected [%u] but was [%u]", non, exp, act);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, "%u");
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_LONG) {
                    //const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                    const long exp = test->issue.expected._long;
                    const long act = test->issue.actual._long;
                    //__clove_string_sprintf(msg, sizeof(msg), "%sexpected [%ld] but was [%ld]", non, exp, act);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, "%ld");
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_ULONG) {
                    //const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                    const unsigned long exp = test->issue.expected._ulong;
                    const unsigned long act = test->issue.actual._ulong;
                    //__clove_string_sprintf(msg, sizeof(msg), "%sexpected [%lu] but was [%lu]", non, exp, act);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, "%lu");
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_LLONG) {
                    //const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                    const long long exp = test->issue.expected._llong;
                    const long long act = test->issue.actual._llong;
                    //__clove_string_sprintf(msg, sizeof(msg), "%sexpected [%lld] but was [%lld]", non, exp, act);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, "%lld");
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_ULLONG) {
                    //const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                    const unsigned long long exp = test->issue.expected._ullong;
                    const unsigned long long act = test->issue.actual._ullong;
                    //__clove_string_sprintf(msg, sizeof(msg), "%sexpected [%llu] but was [%llu]", non, exp, act);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, "%llu");
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_SIZET) {
                    //const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                    const size_t exp = test->issue.expected._sizet;
                    const size_t act = test->issue.actual._sizet;
                    //__clove_string_sprintf(msg, sizeof(msg), "%sexpected [%zu] but was [%zu]", non, exp, act);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, "%zu");
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_FLOAT) {
                    const float exp = test->issue.expected._float;
                    const float act = test->issue.actual._float;
                    char format[6] = {0}; //Example: %.NNf
                    __clove_string_sprintf(format, sizeof(format), "%%.%df", test->issue.floating_precision);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, format);
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_DOUBLE) {
                    const double exp = test->issue.expected._double;
                    const double act = test->issue.actual._double;
                    char format[6] = {0}; //Example: %.NNf
                    __clove_string_sprintf(format, sizeof(format), "%%.%df", test->issue.floating_precision);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, format);
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_STRING) {
                    const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                    const char* exp = test->issue.expected._string;
                    const char* act = test->issue.actual._string;

                    const size_t max_len = 16;
                    const size_t exp_len = __clove_string_length(exp);
                    const size_t act_len = __clove_string_length(act);

                    if (exp_len <= max_len && act_len <= max_len) {
                        char* exp_escaped = __clove_string_escape(exp);
                        char* act_escaped = __clove_string_escape(act);
                        __clove_string_sprintf(msg, sizeof(msg), "%sexpected \"%s\" but was \"%s\"", non, exp_escaped, act_escaped);
                        free(exp_escaped);
                        free(act_escaped);
                    }
                    else {
                        char exp_short[16];
                        char act_short[16];
                        __clove_report_pretty_string_ellipse(exp, exp_len, act, act_len, exp_short, act_short, 16);

                        char* exp_escaped = __clove_string_escape(exp_short);
                        char* act_escaped = __clove_string_escape(act_short);
                        __clove_string_sprintf(msg, sizeof(msg), "%sexpected [%zu]\"%s\" but was [%zu]\"%s\"", non, exp_len, exp_escaped, act_len, act_escaped);
                        free(exp_escaped);
                        free(act_escaped);
                    }
                }
                __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_PTR) {
                    //const char* non = test->issue.assert == __CLOVE_ASSERT_EQ ? "" : "not ";
                    const void* exp = test->issue.expected._ptr;
                    const void* act = test->issue.actual._ptr;
                    //__clove_string_sprintf(msg, sizeof(msg), "%sexpected [%p] but was [%p]", non, exp, act);
                    __PRETTY_PRINT_FAIL_ASSERT_MSG(msg, sizeof(msg), test->issue.assert, exp, act, "%p");
                }
            __CLOVE_SWITCH_END()
        }

        //Dublication
        const char* file_path = test->file_name;
        if (report->params->tests_base_path) {
            file_path = __clove_path_relative(test->file_name, report->params->tests_base_path);
        }
        //Dublication
        char result[__CLOVE_STRING_LENGTH], strToPad[__CLOVE_TEST_ENTRY_LENGTH];
        snprintf(strToPad, __CLOVE_TEST_ENTRY_LENGTH, "%0*zu) %s.%s", report->max_test_digits, test_number, suite->name, test->name);
        __clove_report_pretty_pad_right(result, strToPad);

        report->stream->writef(report->stream, "%s %s%s %s:%d: %s\n", report->labels.erro, result, report->labels.fail, file_path, test->issue.line, msg);
    }
    else if (print_skipped && test->result == __CLOVE_TEST_RESULT_SKIPPED) {
        
        //Dublication
        const char* file_path = test->file_name;
        if (report->params->tests_base_path) {
            file_path = __clove_path_relative(test->file_name, report->params->tests_base_path);
        }
        //Dublication
        char result[__CLOVE_STRING_LENGTH], strToPad[__CLOVE_TEST_ENTRY_LENGTH];
        snprintf(strToPad, __CLOVE_TEST_ENTRY_LENGTH, "%0*zu) %s.%s", report->max_test_digits, test_number, suite->name, test->name);
        __clove_report_pretty_pad_right(result, strToPad);

        report->stream->writef(report->stream, "%s %s%s %s:%zu: %s\n", report->labels.warn, result, report->labels.skip, file_path, test->funct_line, "Missing assertion!");
    }
}

void __clove_report_pretty_string_ellipse(
    const char* exp, size_t exp_len,
    const char* act, size_t act_len,
    char* exp_short, char* act_short, size_t short_size)
{
    size_t iter_len = exp_len < act_len ? exp_len : act_len;
    if (iter_len == 0) {
        __clove_string_ellipse(exp, exp_len, 0, exp_short, short_size);
        __clove_string_ellipse(act, act_len, 0, act_short, short_size);
        return;
    }

    for (size_t i = 0; i < iter_len; ++i) {
        if (exp[i] != act[i]) {
            __clove_string_ellipse(exp, exp_len, i, exp_short, short_size);
            __clove_string_ellipse(act, act_len, i, act_short, short_size);
            return;
        }
    }

    //Scenario where the shortest one is the like the "prefix" of the longest one 
    __clove_string_ellipse(exp, exp_len, iter_len-1, exp_short, short_size);
    __clove_string_ellipse(act, act_len, iter_len-1, act_short, short_size);
}

void __clove_report_pretty_pad_right(char* result, char* strToPad) {
    int targetStrLen = __CLOVE_TEST_ENTRY_LENGTH;           // Target output length  
    const char* padding = "...................................................................................";

    int padLen = targetStrLen - (int)__clove_string_length(strToPad); // Calc Padding length
    if (padLen < 0) padLen = 0;    // Avoid negative length

    // FORMAT SPECIFIER: https://www.tutorialspoint.com/format-specifiers-in-c
    // %* => minimal length taken from the padding string
    // .* => precision, exact length of the string taken from the padding string
    __clove_string_sprintf(result, __CLOVE_STRING_LENGTH, "%s%*.*s", strToPad, padLen, padLen, padding);  // LEFT Padding 
}
#pragma endregion // RunTests Report Pretty Impl

#pragma region PRIVATE - RunTests Report Csv Impl
#include <stdio.h>
__clove_report_run_tests_csv_t* __clove_report_run_tests_csv_new(__clove_stream_t* stream, __clove_report_params_t* params) {
    __clove_report_run_tests_csv_t* result = __CLOVE_MEMORY_MALLOC_TYPE(__clove_report_run_tests_csv_t);
    result->base.start = __clove_report_run_tests_csv_start;
    result->base.begin_suite = __clove_report_run_tests_csv_begin_suite;
    result->base.end_suite = __clove_report_run_tests_csv_end_suite;
    result->base.end = __clove_report_run_tests_csv_end;
    result->base.end_test = __clove_report_run_tests_csv_end_test;
    result->base.free = __clove_report_run_tests_csv_free;
    result->stream = stream;
    result->params = params;
    return result;
}

void __clove_report_run_tests_csv_free(__clove_report_t* report) {
    free(report);
}

void __clove_report_run_tests_csv_start(__clove_report_t* _this, size_t suite_count, size_t test_count) {
    __CLOVE_UNUSED_VAR(_this);
    __CLOVE_UNUSED_VAR(suite_count);
    __CLOVE_UNUSED_VAR(test_count);

    __clove_report_run_tests_csv_t* report = (__clove_report_run_tests_csv_t*)_this;
    report->stream->open(report->stream);
    report->stream->writef(report->stream, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n", "Suite","Test","Status","Duration","File","Line","Assert","Type","Expected","Actual");
}
void __clove_report_run_tests_csv_begin_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index) {
    __CLOVE_UNUSED_VAR(_this);
    __CLOVE_UNUSED_VAR(suite);
    __CLOVE_UNUSED_VAR(index);

    //nothing todo
}
void __clove_report_run_tests_csv_end_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index) {
    __CLOVE_UNUSED_VAR(_this);
    __CLOVE_UNUSED_VAR(suite);
    __CLOVE_UNUSED_VAR(index);
    //nothing todo
}
void __clove_report_run_tests_csv_end(__clove_report_t* _this, size_t test_count, size_t passed, size_t skipped, size_t failed) {
    __CLOVE_UNUSED_VAR(test_count);
    __CLOVE_UNUSED_VAR(passed);
    __CLOVE_UNUSED_VAR(skipped);
    __CLOVE_UNUSED_VAR(failed);

    __clove_report_run_tests_csv_t* report = (__clove_report_run_tests_csv_t*)_this;
    report->stream->close(report->stream);
}

void __clove_report_run_tests_csv_end_test(__clove_report_t* _this, __clove_suite_t* suite, __clove_test_t* test, size_t test_number) {
    __CLOVE_UNUSED_VAR(test_number);
    
    __clove_report_run_tests_csv_t* report = (__clove_report_run_tests_csv_t*)_this;

    bool print_passed = false;
    bool print_failed = false;
    bool print_skipped = false;
    if (report->params->report_detail == __CLOVE_REPORT_DETAIL__PASSED_FAILED_SKIPPED) {
        print_passed = true;
        print_failed = true;
        print_skipped = true;
    } else if (report->params->report_detail == __CLOVE_REPORT_DETAIL__FAILED) {
        print_passed = false;
        print_failed = true;
        print_skipped = false;
    } else if (report->params->report_detail == __CLOVE_REPORT_DETAIL__FAILED_SKIPPED) {
        print_passed = false;
        print_failed = true;
        print_skipped = true;
    }

    ////Suite,Test,Status,Duration,File,Line,Assert,Type,Expected,Actual
    if (print_passed && test->result == __CLOVE_TEST_RESULT_PASSED) {
        report->stream->writef(report->stream, "%s,%s,%s,%llu,%s,%s,%s,%s,%s,%s\n", 
            suite->name, test->name, test->result, __clove_time_to_nanos(&(test->duration)),"","","","","","");
    } 
    else if (print_failed && test->result == __CLOVE_TEST_RESULT_FAILED) {
        const char* data_type = (test->issue.assert == __CLOVE_ASSERT_FAIL) ? "" : test->issue.data_type;
        const char* file_name = __clove_path_relative(test->file_name, report->params->tests_base_path);
      
        report->stream->writef(report->stream, "%s,%s,%s,%s,%s,%u,%s,%s,", 
            suite->name, test->name, test->result, "",file_name,test->issue.line, test->issue.assert, data_type);

        __clove_report_run_tests_csv_print_data(report, test, &test->issue.expected);
        report->stream->writef(report->stream, ",");
        __clove_report_run_tests_csv_print_data(report, test, &test->issue.actual);
        report->stream->writef(report->stream, "\n");
    }
    else if (print_skipped && test->result == __CLOVE_TEST_RESULT_SKIPPED) {
        const char* file_name = __clove_path_relative(test->file_name, report->params->tests_base_path);

        report->stream->writef(report->stream, "%s,%s,%s,%s,%s,%zu,%s,%s,%s,%s\n", 
            suite->name, test->name, test->result,"",file_name,test->funct_line,"","","","");
    }
}

void __clove_report_run_tests_csv_print_data(__clove_report_run_tests_csv_t* instance, __clove_test_t* test, __clove_generic_u* data) {
    if (test->issue.assert == __CLOVE_ASSERT_FAIL) {
        instance->stream->writef(instance->stream, "%s", "");
    } else {
        __CLOVE_SWITCH_BEG(test->issue.data_type)
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_BOOL)
                instance->stream->writef(instance->stream, "%s", data->_bool ? "true" : "false");
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_CHAR)
                instance->stream->writef(instance->stream, "%c", data->_char);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_INT)
                instance->stream->writef(instance->stream, "%d", data->_int);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_UINT)
                instance->stream->writef(instance->stream, "%u", data->_uint);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_LONG)
                instance->stream->writef(instance->stream, "%ld", data->_long);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_ULONG)
                instance->stream->writef(instance->stream, "%lu", data->_ulong);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_LLONG)
                instance->stream->writef(instance->stream, "%lld", data->_llong);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_ULLONG)
                instance->stream->writef(instance->stream, "%llu", data->_ullong);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_SIZET)
                instance->stream->writef(instance->stream, "%zu", data->_sizet);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_FLOAT)
                instance->stream->writef(instance->stream, "%.*f", test->issue.floating_precision, data->_float);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_DOUBLE)
                instance->stream->writef(instance->stream, "%.*f", test->issue.floating_precision, data->_double);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_STRING) {
                char* escaped = __clove_string_csv_escape(data->_string);
                instance->stream->writef(instance->stream, "%s", escaped);
                free(escaped);
            }
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_PTR)
                instance->stream->writef(instance->stream, "%p", data->_ptr);
        __CLOVE_SWITCH_END()
    }
}
#pragma endregion //Report RunTests Csv Impl



#pragma region PRIVATE - Report Json Impl
#include <stdio.h>
#include <stdlib.h>
__clove_report_json_t* __clove_report_run_tests_json_new(__clove_stream_t* stream, __clove_report_params_t* params) {
    __clove_report_json_t* result = __CLOVE_MEMORY_MALLOC_TYPE(__clove_report_json_t);
    result->base.start = __clove_report_json_start;
    result->base.begin_suite = __clove_report_json_begin_suite;
    result->base.end_suite = __clove_report_json_end_suite;
    result->base.end = __clove_report_json_end;
    result->base.end_test = __clove_report_json_end_test;
    result->base.free = __clove_report_json_free;
    result->stream = stream;
    result->params = params;
    result->clove_version = __CLOVE_VERSION;
    result->json_schema = "1.0";
    result->current_suite = NULL;
    result->test_count = 0;
    result->suite_tests_number = 0;

    result->suite_count = 0;
    result->is_first_suite_test = false;
    //Full report is the main usage scenario, so report will be computed during tests execution,
    //instead, for detail Failed (or Failed+Skipped) need to cache suites (and related test) and only compute 
    //the report at end of tests executions
    result->is_realtime_scenario = params->report_detail == __CLOVE_REPORT_DETAIL__PASSED_FAILED_SKIPPED;
    result->is_reporting_enabled = result->is_realtime_scenario;
    result->cached_suites = __clove_vector_null();
    return result;
}

void __clove_report_json_free(__clove_report_t* report) {
    __clove_report_json_t* instance = (__clove_report_json_t*)report;
    __clove_vector_free(&instance->cached_suites);
    free(report);
}

void __clove_report_json_start(__clove_report_t* _this, size_t suite_count, size_t test_count) {
    __clove_report_json_t* instance = (__clove_report_json_t*)_this;

    if (!instance->is_realtime_scenario) {
        __CLOVE_VECTOR_INIT_CAPACITY(&instance->cached_suites, __clove_suite_t*, suite_count);
    }

    instance->suite_count = suite_count;

    bool opened = instance->stream->open(instance->stream);
    if (!opened) { __clove_console_printf("ERROR OPENING STREAM\n"); } //TODO: ToString stream

    instance->stream->writef(instance->stream, "{\n");
    instance->stream->writef(instance->stream, "\t\"clove_version\" : \"%s\",\n", instance->clove_version);
    instance->stream->writef(instance->stream, "\t\"json_schema\" : \"%s\",\n", instance->json_schema);
    instance->stream->writef(instance->stream, "\t\"result\" : {\n");
    instance->stream->writef(instance->stream, "\t\t\"suite_count\" : %zu,\n", suite_count);
    instance->stream->writef(instance->stream, "\t\t\"test_count\" : %zu,\n", test_count);
    instance->stream->writef(instance->stream, "\t\t\"suites\" : {\n");
}


void __clove_report_json_end(__clove_report_t* _this, size_t test_count, size_t passed, size_t skipped, size_t failed) {
    __clove_report_json_t* instance = (__clove_report_json_t*)_this;
    
    if (!instance->is_reporting_enabled) {
        instance->is_reporting_enabled = true;
    
        size_t suite_count = __clove_vector_count(&instance->cached_suites);
        size_t test_number = 0;
        
        bool report_failed = false;
        bool report_skipped = false;
        if (instance->params->report_detail == __CLOVE_REPORT_DETAIL__FAILED) {
            report_failed = true;
            report_skipped = false;
        } else if (instance->params->report_detail == __CLOVE_REPORT_DETAIL__FAILED_SKIPPED) {
            report_failed = true;
            report_skipped = true;
        }

        for(size_t suite_index=0; suite_index < suite_count; ++suite_index) {
            __clove_suite_t* suite = *(__clove_suite_t**)__clove_vector_get(&instance->cached_suites, suite_index);
            bool has_failed = report_failed && suite->issue.failed_count > 0;
            bool has_skipped = report_skipped && suite->issue.skipped_count > 0;
            if (!has_failed && !has_skipped) continue; 

            instance->suite_tests_number = 0;
            if (has_failed) instance->suite_tests_number += suite->issue.failed_count;
            if (has_skipped) instance->suite_tests_number += suite->issue.skipped_count;

            __clove_report_json_begin_suite(_this, suite, suite_index);

            for(size_t test_index=0; test_index < __clove_vector_count(&suite->tests); ++test_index) { 
                __clove_test_t* test = (__clove_test_t*)__clove_vector_get(&suite->tests, test_index);
                bool failed_case = report_failed && test->result == __CLOVE_TEST_RESULT_FAILED;
                bool skipped_case = report_skipped && test->result == __CLOVE_TEST_RESULT_SKIPPED;
                if (!failed_case && !skipped_case) continue;

                __clove_report_json_end_test(_this, suite, test, ++test_number);
            }

            __clove_report_json_end_suite(_this, suite, suite_index);
        }
    }

    //Write overall results
    const char* status = "UNKNOWN";
    if (passed == test_count) status = __CLOVE_TEST_RESULT_PASSED;
    else if (failed > 0) status = __CLOVE_TEST_RESULT_FAILED;
    else if (skipped > 0) status = __CLOVE_TEST_RESULT_SKIPPED;

    instance->stream->writef(instance->stream, "\t\t},\n"); //suites
    instance->stream->writef(instance->stream, "\t\t\"test_passed\" : %zu,\n", passed);
    instance->stream->writef(instance->stream, "\t\t\"test_skipped\" : %zu,\n", skipped);
    instance->stream->writef(instance->stream, "\t\t\"test_failed\" : %zu,\n", failed);
    instance->stream->writef(instance->stream, "\t\t\"status\" : \"%s\"\n", status);
    instance->stream->writef(instance->stream, "\t}\n"); //result
    instance->stream->writef(instance->stream, "}"); //object

    instance->stream->close(instance->stream);
}

void __clove_report_json_print_data(__clove_report_json_t* instance, __clove_test_t* test, __clove_generic_u* data) {
    if (test->issue.assert == __CLOVE_ASSERT_FAIL) {
        instance->stream->writef(instance->stream, "%s", "");
    } else {
        __CLOVE_SWITCH_BEG(test->issue.data_type)
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_BOOL)
                instance->stream->writef(instance->stream, "%s", data->_bool ? "true" : "false");
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_CHAR)
                instance->stream->writef(instance->stream, "%c", data->_char);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_INT)
                instance->stream->writef(instance->stream, "%d", data->_int);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_UINT)
                instance->stream->writef(instance->stream, "%u", data->_uint);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_LONG)
                instance->stream->writef(instance->stream, "%ld", data->_long);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_ULONG)
                instance->stream->writef(instance->stream, "%lu", data->_ulong);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_LLONG)
                instance->stream->writef(instance->stream, "%lld", data->_llong);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_ULLONG)
                instance->stream->writef(instance->stream, "%llu", data->_ullong);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_SIZET)
                instance->stream->writef(instance->stream, "%zu", data->_sizet);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_FLOAT)
                instance->stream->writef(instance->stream, "%.*f", test->issue.floating_precision, data->_float);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_DOUBLE)
                instance->stream->writef(instance->stream, "%.*f", test->issue.floating_precision, data->_double);
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_STRING) {
                char* escaped = __clove_string_escape(data->_string);
                instance->stream->writef(instance->stream, "%s", escaped);
                free(escaped);
            }
            __CLOVE_SWITCH_CASE(__CLOVE_GENERIC_PTR)
                instance->stream->writef(instance->stream, "%p", data->_ptr);
        __CLOVE_SWITCH_END()
    }
}

void __clove_report_json_begin_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index) {
    __CLOVE_UNUSED_VAR(index);

    __clove_report_json_t* instance = (__clove_report_json_t*)_this;
    if (instance->is_reporting_enabled) {
        instance->is_first_suite_test = true;
        instance->current_suite = suite;
    }
}

void __clove_report_json_end_suite(__clove_report_t* _this, __clove_suite_t* suite, size_t index) {
    __CLOVE_UNUSED_VAR(index);

    __clove_report_json_t* instance = (__clove_report_json_t*)_this;
    if (!instance->is_reporting_enabled) {
        __CLOVE_VECTOR_ADD(&instance->cached_suites, __clove_suite_t*, suite);
        return;
    } 

    const char* comma = "";
    if (index < instance->suite_count-1) {
        comma = ",";
    }
    instance->stream->writef(instance->stream, "\t\t\t}%s\n", comma); //close suite    

    instance->current_suite = NULL;
}

void __clove_report_json_end_test(__clove_report_t* _this, __clove_suite_t* suite, __clove_test_t* test, size_t test_number) {
    __CLOVE_UNUSED_VAR(test_number);

    __clove_report_json_t* instance = (__clove_report_json_t*)_this;
    if (!instance->is_reporting_enabled) return;
    
    if (instance->is_first_suite_test) {
        const char* file_path = __clove_path_relative(test->file_name, instance->params->tests_base_path);
        char* escaped_file = __clove_string_strdup(file_path);
        __clove_string_replace_char(escaped_file, '\\', '/');
        
        instance->stream->writef(instance->stream, "\t\t\t\"%s\" : {\n", instance->current_suite->name);
        instance->stream->writef(instance->stream, "\t\t\t\t\"file\" : \"%s\",\n", escaped_file);
        instance->stream->writef(instance->stream, "\t\t\t\t\"tests\" : {\n");
        instance->test_count = 0;
   
        free(escaped_file);
        instance->is_first_suite_test = false;
    }

    instance->test_count++;
    instance->stream->writef(instance->stream, "\t\t\t\t\t\"%s\" : {\n", test->name);
    instance->stream->writef(instance->stream, "\t\t\t\t\t\t\"status\" : \"%s\",\n", test->result);
    instance->stream->writef(instance->stream, "\t\t\t\t\t\t\"duration\" : %llu", __clove_time_to_nanos(&(test->duration)));
    if (test->result == __CLOVE_TEST_RESULT_FAILED) {
        instance->stream->writef(instance->stream, ",\n");
        instance->stream->writef(instance->stream, "\t\t\t\t\t\t\"line\" : %u,\n", test->issue.line);
        instance->stream->writef(instance->stream, "\t\t\t\t\t\t\"assert\" : \"%s\"", test->issue.assert);        
        if (test->issue.assert != __CLOVE_ASSERT_FAIL) {
            instance->stream->writef(instance->stream, ",\n");
            instance->stream->writef(instance->stream, "\t\t\t\t\t\t\"type\" : \"%s\",\n", test->issue.data_type);
            instance->stream->writef(instance->stream, "\t\t\t\t\t\t\"expected\" : \"");
            __clove_report_json_print_data(instance, test, &(test->issue.expected));
            instance->stream->writef(instance->stream, "\",\n");
            
            instance->stream->writef(instance->stream, "\t\t\t\t\t\t\"actual\" : \"");
            __clove_report_json_print_data(instance, test, &(test->issue.actual));
            instance->stream->writef(instance->stream, "\"\n");
        } else {
            instance->stream->writef(instance->stream, "\n");
        }
    } else if (test->result == __CLOVE_TEST_RESULT_SKIPPED) {
        instance->stream->writef(instance->stream, ",\n");
        instance->stream->writef(instance->stream, "\t\t\t\t\t\t\"line\" : %zu\n", test->funct_line);
    } else {
        instance->stream->writef(instance->stream, "\n");
    }
    instance->stream->writef(instance->stream, "\t\t\t\t\t}");

    size_t suite_total_tests = suite->test_count; //realtime scenario
    if (!instance->is_realtime_scenario) suite_total_tests = instance->suite_tests_number;

    //if (instance->test_count < suite->test_count) {
    if (instance->test_count < suite_total_tests) {
        instance->stream->writef(instance->stream, ",\n");
    }
    else {
        instance->stream->writef(instance->stream, "\n");
        instance->stream->writef(instance->stream, "\t\t\t\t}\n"); //close "tests"
    }
}
#pragma endregion // Report Json Impl

#pragma region PRIVATE - Report List Test Impl
__clove_report_list_tests_pretty_t* __clove_report_list_tests_pretty_new(__clove_stream_t* stream, __clove_report_params_t* params) {
    __clove_report_list_tests_pretty_t* _this = __CLOVE_MEMORY_MALLOC_TYPE(__clove_report_list_tests_pretty_t);
    _this->base.begin = __clove_report_list_tests_pretty_begin;
    _this->base.begin_suite = __clove_report_list_tests_pretty_begin_suite;
    _this->base.begin_test = __clove_report_list_tests_pretty_begin_test;
    _this->base.end_test = __clove_report_list_tests_pretty_end_test;
    _this->base.end_suite = __clove_report_list_tests_pretty_end_suite;
    _this->base.end = __clove_report_list_tests_pretty_end;
    _this->base.free = __clove_report_list_tests_pretty_free;
    _this->stream = stream;
    _this->params = params;
    _this->suite_format = NULL;
    _this->test_format = NULL;
    _this->is_suite_first_test = false;
    _this->current_suite = NULL;
    return _this;
}
void __clove_report_list_tests_pretty_free(__clove_report_list_tests_t* _this) {
    free((__clove_report_list_tests_pretty_t*)_this);
}
void __clove_report_list_tests_pretty_begin(__clove_report_list_tests_t* _this, size_t suite_count, size_t test_count) {
    __clove_report_list_tests_pretty_t* pretty = (__clove_report_list_tests_pretty_t*)_this;
    pretty->stream->open(pretty->stream);

    bool activated = pretty->stream->has_ansi_support(pretty->stream);
    if (activated) {
        pretty->suite_format = "\x1b[1;34m%s\x1b[0m (file: %s)\n";
        pretty->test_format = "- \x1b[1;32m%s\x1b[0m [line: %zu]\n";
        pretty->stream->writef(pretty->stream, "\nListing \x1b[1;32m%zu\x1b[0m Test(s) in \x1b[1;34m%zu\x1b[0m Suite(s):\n\n", test_count, suite_count);
    } else {
        pretty->suite_format = "%s (file: %s)\n";
        pretty->test_format = "- %s [line: %zu]\n";
        pretty->stream->writef(pretty->stream, "\nListing %zu Test(s) in %zu Suite(s):\n\n", test_count, suite_count);
    }
}
void __clove_report_list_tests_pretty_begin_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index) {
     __CLOVE_UNUSED_VAR(index);

    __clove_report_list_tests_pretty_t* pretty = (__clove_report_list_tests_pretty_t*)_this;
    pretty->is_suite_first_test = true;
    pretty->current_suite = suite;
}
void __clove_report_list_tests_pretty_end_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index) {
    __CLOVE_UNUSED_VAR(_this);
    __CLOVE_UNUSED_VAR(suite);
    __CLOVE_UNUSED_VAR(index);
    //nothing todo
}
void __clove_report_list_tests_pretty_begin_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index) {
    __CLOVE_UNUSED_VAR(_this);
    __CLOVE_UNUSED_VAR(test);
    __CLOVE_UNUSED_VAR(index);
    //nothing todo
}
void __clove_report_list_tests_pretty_end_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index) {
    __CLOVE_UNUSED_VAR(index);

    __clove_report_list_tests_pretty_t* pretty = (__clove_report_list_tests_pretty_t*)_this;
    if (pretty->is_suite_first_test) {
        const char* file_path = __clove_path_relative(test->file_name, pretty->params->tests_base_path);
        pretty->stream->writef(pretty->stream, pretty->suite_format, pretty->current_suite->name, file_path); 
        pretty->is_suite_first_test = false;  
    }
    pretty->stream->writef(pretty->stream, pretty->test_format, test->name, test->funct_line);
}
void __clove_report_list_tests_pretty_end(__clove_report_list_tests_t* _this) {
    __clove_report_list_tests_pretty_t* pretty = (__clove_report_list_tests_pretty_t*)_this;
    pretty->stream->close(pretty->stream);
}

__clove_report_list_tests_csv_t* __clove_report_list_tests_csv_new(__clove_stream_t* stream, __clove_report_params_t* params) {
    __clove_report_list_tests_csv_t* _this = __CLOVE_MEMORY_MALLOC_TYPE(__clove_report_list_tests_csv_t);
    _this->base.begin = __clove_report_list_tests_csv_begin;
    _this->base.begin_suite = __clove_report_list_tests_csv_begin_suite;
    _this->base.begin_test = __clove_report_list_tests_csv_begin_test;
    _this->base.end_test = __clove_report_list_tests_csv_end_test;
    _this->base.end_suite = __clove_report_list_tests_csv_end_suite;
    _this->base.end = __clove_report_list_tests_csv_end;
    _this->base.free = __clove_report_list_tests_csv_free;
    _this->stream = stream;
    _this->params = params;
    _this->current_suite = NULL;
    return _this;
}
void __clove_report_list_tests_csv_free(__clove_report_list_tests_t* _this) {
    free((__clove_report_list_tests_csv_t*)_this);
}
void __clove_report_list_tests_csv_begin(__clove_report_list_tests_t* _this, size_t suite_count, size_t test_count) {
    __CLOVE_UNUSED_VAR(test_count);

    __clove_report_list_tests_csv_t* csv = (__clove_report_list_tests_csv_t*)_this;
    csv->stream->open(csv->stream);
    if (suite_count > 0) {
         csv->stream->writef(csv->stream, "%s,%s,%s,%s\n", "Suite", "Test", "File", "Line");
    }
}
void __clove_report_list_tests_csv_begin_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index) {
    __CLOVE_UNUSED_VAR(index);

    __clove_report_list_tests_csv_t* csv = (__clove_report_list_tests_csv_t*)_this;
    csv->current_suite = suite;
}
void __clove_report_list_tests_csv_end_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index) {
    __CLOVE_UNUSED_VAR(suite);
    __CLOVE_UNUSED_VAR(index);

    __clove_report_list_tests_csv_t* csv = (__clove_report_list_tests_csv_t*)_this;
    csv->current_suite = NULL;
}
void __clove_report_list_tests_csv_begin_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index) {
    __CLOVE_UNUSED_VAR(_this);
    __CLOVE_UNUSED_VAR(test);
    __CLOVE_UNUSED_VAR(index);
    //nothing todo
}
void __clove_report_list_tests_csv_end_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index) {
    __CLOVE_UNUSED_VAR(index);

    __clove_report_list_tests_csv_t* csv = (__clove_report_list_tests_csv_t*)_this;
    const char* file_path = __clove_path_relative(test->file_name, csv->params->tests_base_path);
    csv->stream->writef(csv->stream, "%s,%s,%s,%zu\n", csv->current_suite->name, test->name, file_path, test->funct_line);
}
void __clove_report_list_tests_csv_end(__clove_report_list_tests_t* _this) {
    __clove_report_list_tests_csv_t* csv = (__clove_report_list_tests_csv_t*)_this;
    csv->stream->close(csv->stream);
}

__clove_report_list_tests_json_t* __clove_report_list_tests_json_new(__clove_stream_t* stream, __clove_report_params_t* params) {
    __clove_report_list_tests_json_t* _this = __CLOVE_MEMORY_MALLOC_TYPE(__clove_report_list_tests_json_t);
    _this->base.begin = __clove_report_list_tests_json_begin;
    _this->base.begin_suite = __clove_report_list_tests_json_begin_suite;
    _this->base.begin_test = __clove_report_list_tests_json_begin_test;
    _this->base.end_test = __clove_report_list_tests_json_end_test;
    _this->base.end_suite = __clove_report_list_tests_json_end_suite;
    _this->base.end = __clove_report_list_tests_json_end;
    _this->base.free = __clove_report_list_tests_json_free;
    _this->stream = stream;
    _this->params = params;
    _this->clove_version = __CLOVE_VERSION;
    _this->json_schema = "1.0";
    _this->suite_count = 0;
    _this->is_suite_first_test = false;
    _this->current_suite = NULL;
    return _this;
}
void __clove_report_list_tests_json_free(__clove_report_list_tests_t* _this) {
    free((__clove_report_list_tests_json_t*)_this);
}

void __clove_report_list_tests_json_begin(__clove_report_list_tests_t* _this, size_t suite_count, size_t test_count) {
    __clove_report_list_tests_json_t* json = (__clove_report_list_tests_json_t*)_this;
    json->suite_count = suite_count;
    
    json->stream->open(json->stream);

    json->stream->writef(json->stream, "{\n");
    json->stream->writef(json->stream, "\t\"clove_version\" : \"%s\",\n", json->clove_version);
    json->stream->writef(json->stream, "\t\"json_schema\" : \"%s\",\n", json->json_schema);
    json->stream->writef(json->stream, "\t\"result\" : {\n");
    json->stream->writef(json->stream, "\t\t\"suite_count\" : %zu,\n", suite_count);
    json->stream->writef(json->stream, "\t\t\"test_count\" : %zu,\n", test_count);
    json->stream->writef(json->stream, "\t\t\"suites\" : [\n");
    
}
void __clove_report_list_tests_json_begin_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index) {
    __CLOVE_UNUSED_VAR(index);
    
    __clove_report_list_tests_json_t* json = (__clove_report_list_tests_json_t*)_this;
    json->current_suite = suite;
    json->is_suite_first_test = true;
}
void __clove_report_list_tests_json_end_suite(__clove_report_list_tests_t* _this,  __clove_suite_t* suite, size_t index) {
    __CLOVE_UNUSED_VAR(suite);

    __clove_report_list_tests_json_t* json = (__clove_report_list_tests_json_t*)_this;
    json->current_suite = NULL;

    const char* comma = "";
    if (index < json->suite_count-1) {
        comma = ",";
    }
    json->stream->writef(json->stream, "\t\t\t\t]\n");
    json->stream->writef(json->stream, "\t\t\t}%s\n", comma);
}
void __clove_report_list_tests_json_begin_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index) {
    __CLOVE_UNUSED_VAR(_this);
    __CLOVE_UNUSED_VAR(test);
    __CLOVE_UNUSED_VAR(index);
    //nothing todo
}
void __clove_report_list_tests_json_end_test(__clove_report_list_tests_t* _this,  __clove_test_t* test, size_t index) {
    __clove_report_list_tests_json_t* json = (__clove_report_list_tests_json_t*)_this;
    if (json->is_suite_first_test) {
        const char* file_path = __clove_path_relative(test->file_name, json->params->tests_base_path);
        char* escaped_file = __clove_string_strdup(file_path);
        __clove_string_replace_char(escaped_file, '\\', '/');

        json->stream->writef(json->stream, "\t\t\t{\n");
        json->stream->writef(json->stream, "\t\t\t\t\"name\" : \"%s\",\n", json->current_suite->name);
        json->stream->writef(json->stream, "\t\t\t\t\"file\" : \"%s\",\n", escaped_file);
        json->stream->writef(json->stream, "\t\t\t\t\"tests\" : [\n");

        free(escaped_file);
        json->is_suite_first_test = false;  
    }
    json->stream->writef(json->stream, "\t\t\t\t\t{\n");
    json->stream->writef(json->stream, "\t\t\t\t\t\t\"name\" : \"%s\",\n", test->name);
    json->stream->writef(json->stream, "\t\t\t\t\t\t\"line\" : %zu\n", test->funct_line);
    const char* comma = "";
    if (index < json->current_suite->test_count-1) {
        comma = ",";
    }
    json->stream->writef(json->stream, "\t\t\t\t\t}%s\n", comma);
}
void __clove_report_list_tests_json_end(__clove_report_list_tests_t* _this) {
    __clove_report_list_tests_json_t* json = (__clove_report_list_tests_json_t*)_this;

    json->stream->writef(json->stream, "\t\t]\n"); //end suites
    json->stream->writef(json->stream, "\t}\n"); //end result
    json->stream->writef(json->stream, "}\n");
    json->stream->close(json->stream);
}

//TODO: Add unit tests
int __clove_cmd_list_test_execute(__clove_suite_t* suites, size_t suite_count, size_t test_count, __clove_report_list_tests_t* report) {
    report->begin(report, suite_count, test_count);
    for (size_t i = 0; i < suite_count; ++i) {
        __clove_suite_t* each_suite = &suites[i];
        report->begin_suite(report, each_suite, i);
        for (size_t j = 0; j < each_suite->test_count; j++) {
            __clove_test_t* each_test = (__clove_test_t*)__clove_vector_get(&each_suite->tests, j);
            report->begin_test(report, each_test, j);
            each_test->dry_run = true;
            each_test->funct(each_test);
            report->end_test(report, each_test, j);
        }
        report->end_suite(report, each_suite, i);
    }
    report->end(report);
    return 0;
}
#pragma endregion


#pragma region PRIVATE - Autodiscovery Impl
bool __clove_symbols_function_validate(__clove_string_view_t* suite, __clove_string_view_t* type, __clove_string_view_t* name, __clove_symbols_context_t* context) {
    if (__clove_vector_is_empty(context->includes) && __clove_vector_is_empty(context->excludes)) return true;
    if (!__clove_string_view_strequals(type, "20")) return true;

    //Includes win over Excludes
    if (!__clove_vector_is_empty(context->includes)) {
        for(size_t i=0; i < __clove_vector_count(context->includes); ++i) {
            __clove_test_expr_t* expr = (__clove_test_expr_t*)__clove_vector_get(context->includes, i);
            if (__clove_test_expr_validate(expr, suite, name)) return true;
        }
        return false;
    }

    if (!__clove_vector_is_empty(context->excludes)) {
        for(size_t i=0; i < __clove_vector_count(context->excludes); ++i) {
            __clove_test_expr_t* expr = (__clove_test_expr_t*)__clove_vector_get(context->excludes, i);
            if (__clove_test_expr_validate(expr, suite, name)) return false;
        }
        return true;
    }
    return true;
}

void __clove_symbols_function_collect(__clove_symbols_function_t exported_funct, __clove_symbols_context_t* context) {
    /* Examples
        __clove_sym___MySuite___11_setuponce
        __clove_sym___MySuite___12_teardownonce
        __clove_sym___MySuite___13_setup
        __clove_sym___MySuite___14_teardown
        __clove_sym___MySuite___20_MyTest01
        __clove_sym___MySuite___20_MyTest02
    */
    static const char* end_suite_separator = "___";
    static int end_suite_separator_length = 3;
    //static const char* test_separator = "20_";
    //static int test_separator_length = 3;

    static __clove_suite_t last_suite_temp; //just used as temporary data storage
    static __clove_suite_t* last_suite_ptr = NULL;
    static __clove_string_view_t suite_name_view = { NULL, 0 };

    const char* test_full_name = exported_funct.name;
    const char* begin_suite_name = test_full_name + context->prefix_length;
    const char* end_suite_name = __clove_string_strstr(begin_suite_name, end_suite_separator) - 1;

    __clove_string_view_t suite_vw = __clove_string_view_from_be(begin_suite_name, end_suite_name);
    __clove_string_view_t type_vw = __clove_string_view_from_len(end_suite_name + 1 + end_suite_separator_length, 2);
    __clove_string_view_t name_vw = __clove_string_view_from_str(__clove_string_view_end(&type_vw) + 2);
    
    bool pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, context);
    if (!pass) return;

    //if suite changes, then start collecting temporary suite data, waiting for a first test to be found
    if (suite_name_view.begin == NULL || !__clove_string_view_equals(&suite_vw, &suite_name_view)) {
        suite_name_view = suite_vw;
        last_suite_temp.fixtures.setup_once = NULL;
        last_suite_temp.fixtures.teardown_once = NULL;
        last_suite_temp.fixtures.setup = NULL;
        last_suite_temp.fixtures.teardown = NULL;
        last_suite_temp.test_count = 0;
    }

    if (__clove_string_view_strequals(&type_vw, "11")) {
        last_suite_temp.fixtures.setup_once = exported_funct.fun_ptr;
    }
    else if (__clove_string_view_strequals(&type_vw, "12")) {
        last_suite_temp.fixtures.teardown_once = exported_funct.fun_ptr;
    }
    else if (__clove_string_view_strequals(&type_vw, "13")) {
        last_suite_temp.fixtures.setup = exported_funct.fun_ptr;
    }
    else if (__clove_string_view_strequals(&type_vw, "14")) {
        last_suite_temp.fixtures.teardown = exported_funct.fun_ptr;
    }
    else if (__clove_string_view_strequals(&type_vw, "20")) {
        if (last_suite_temp.test_count == 0) {
            last_suite_temp.test_count = 1; //just to record that at least one test case has been found

            last_suite_ptr = (__clove_suite_t*)__clove_vector_add_slot(&context->suites);
            last_suite_ptr->name = __clove_string_view_as_string(&suite_name_view);
            if (last_suite_temp.fixtures.setup_once)     last_suite_ptr->fixtures.setup_once    = last_suite_temp.fixtures.setup_once;
            if (last_suite_temp.fixtures.teardown_once)  last_suite_ptr->fixtures.teardown_once = last_suite_temp.fixtures.teardown_once;
            if (last_suite_temp.fixtures.setup)          last_suite_ptr->fixtures.setup         = last_suite_temp.fixtures.setup;
            if (last_suite_temp.fixtures.teardown)       last_suite_ptr->fixtures.teardown      = last_suite_temp.fixtures.teardown;
            context->suites_count++;
        }

        __clove_test_t* test = (__clove_test_t*)__clove_vector_add_slot(&last_suite_ptr->tests);
        //Switched to string allocation to make test structs indipendent from the source memory
        //test->name = test_name + test_separator_length;
        test->name = __clove_string_view_as_string(&name_vw);
        test->funct = (void (*)(__clove_test_t*))exported_funct.fun_ptr;
        last_suite_ptr->test_count++;
        context->tests_count++;
    }
}

#ifdef _WIN32
#include <windows.h>
PIMAGE_EXPORT_DIRECTORY __clove_symbols_win_get_export_table_from(HMODULE module) {
    // Start at the base of the module. The MS-DOS stub begins there.
    PBYTE base = (PBYTE)module;
    PIMAGE_DOS_HEADER mds = (PIMAGE_DOS_HEADER)module;

    PIMAGE_NT_HEADERS nt_header = (PIMAGE_NT_HEADERS)(base + mds->e_lfanew);
    if (IMAGE_NT_SIGNATURE != nt_header->Signature) {
        // Bad signature -- invalid image or module handle
        return NULL;
    }

    // Get the COFF file header.
    //PIMAGE_FILE_HEADER cfh = &nt_header->FileHeader;

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

int __clove_symbols_for_each_function_by_prefix(__clove_symbols_context_t* context, __clove_symbols_function_action action) {
    HMODULE module = GetModuleHandle(0);
    PIMAGE_EXPORT_DIRECTORY export_dir = __clove_symbols_win_get_export_table_from(module);
    if (!export_dir) {
        return 1;
    }

    //Note: Don't know why if there is no exported function results NumberOfNames = 64. NumberOfFunctions = 0 instead.
    //      So checking both counters to be sure if there is any exported function
    if (export_dir->NumberOfNames == 0 || export_dir->NumberOfFunctions == 0) {
        return 1;
    }

    PBYTE base_addr = (PBYTE)module;
    //DWORD names_count = export_dir->NumberOfNames;
    PDWORD names_ptr = (PDWORD)(base_addr + export_dir->AddressOfNames);
    DWORD ordinal_base_number = export_dir->Base;
    PWORD ordinals_ptr = (PWORD)(base_addr + export_dir->AddressOfNameOrdinals); //ordinal offset from base ordinal
    PDWORD functs_address_ptr = (PDWORD)(base_addr + export_dir->AddressOfFunctions);

    const char* prefix = context->prefix;
    size_t prefix_length = context->prefix_length;

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
            __clove_symbols_function_t funct;
            funct.name = each_name;
            funct.obj_ptr = each_funct_addr;
            action(funct, context);
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

intptr_t __clove_symbols_macos_image_slide(const char* path)
{
    for (uint32_t i = 0; i < _dyld_image_count(); i++)
    {
        if (strcmp(_dyld_get_image_name(i), path) == 0)
            return _dyld_get_image_vmaddr_slide(i);
    }
    return 0;
}

int __clove_symbols_macos_open_module_handle(const char* module_path, __clove_symbols_macos_module_t* out_module) {
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

void __clove_symbols_macos_close_module_handle(__clove_symbols_macos_module_t* module) {
    munmap(module->handle, module->size);
    module->handle = NULL;
    module->size = 0;
}
struct load_command* __clove_symbols_macos_find_command(struct mach_header_64* header, uint32_t cmd) {
    struct load_command* lc = (struct load_command*)((uint64_t)header + sizeof(struct mach_header_64));
    for (uint32_t i = 0; i < header->ncmds; i++) {
        if (lc->cmd == cmd) {
            return lc;
        }
        lc = (struct load_command*)((uint64_t)lc + lc->cmdsize);
    }
    return NULL;
}

int __clove_symbols_for_each_function_by_prefix(__clove_symbols_context_t* context, __clove_symbols_function_action action) {
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

    const char* prefix = context->prefix;
    const size_t prefix_length = context->prefix_length;
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
            //__clove_symbols_function_t funct = { each_name, each_funct_addr };
            __clove_symbols_function_t funct;
            funct.name = each_name;
            funct.obj_ptr = each_funct_addr;
            action(funct, context);
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

int __clove_symbols_lixux_dl_callback(struct dl_phdr_info* info, size_t size, void* data)
{
    __CLOVE_UNUSED_VAR(size);

    #ifdef __GNUC__
    #pragma GCC diagnostic push   
    #pragma GCC diagnostic ignored "-Wpedantic"    //Disable pedantic
    #endif
    //Preventing GCC pedantic from emitting warning: "ISO C forbids conversion of function pointer to object pointer type"
    const char* cb = (const char*)&__clove_symbols_lixux_dl_callback;
    #ifdef __GNUC__
    #pragma GCC diagnostic pop 
    #endif 

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


//uintptr_t __clove_symbols_lixux_base_addr(const char* path)
uintptr_t __clove_symbols_lixux_base_addr(void)
{
    uintptr_t base_addr;
    dl_iterate_phdr(__clove_symbols_lixux_dl_callback, &base_addr);
    return base_addr;
}

int __clove_symbols_lixux_open_module_handle(const char* module_path, __clove_symbols_lixux_module_t* out_module) {
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
    //out_module->address = __clove_symbols_lixux_base_addr(module_path);
    out_module->address = __clove_symbols_lixux_base_addr();
    return 0;
}

void __clove_symbols_lixux_close_module_handle(__clove_symbols_lixux_module_t* module) {
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
int __clove_symbols_funct_name_comparator(void* f1, void* f2) {
    __clove_symbols_function_t* funct1 = (__clove_symbols_function_t*)f1;
    __clove_symbols_function_t* funct2 = (__clove_symbols_function_t*)f2;
    return strcmp(funct1->name, funct2->name);
}

int __clove_symbols_for_each_function_by_prefix(__clove_symbols_context_t* context, __clove_symbols_function_action action) {
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
    for (size_t i = 0; i < section_count; ++i) {
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
    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(__clove_symbols_function_t));
    __clove_vector_init(&clove_functions, &params);

    const char* prefix = context->prefix;
    size_t prefix_length = context->prefix_length;

    for (size_t i = 0; i < symbol_count; ++i) {
        Elf64_Sym* sym = &symbol_table[i];
        if (ELF64_ST_TYPE(sym->st_info) == STT_FUNC) {
            size_t name_index = sym->st_name;
            char* sym_name = symbol_name_table + name_index;
            if (!strncmp(sym_name, prefix, prefix_length)) {
                __clove_symbols_function_t* each_funct = (__clove_symbols_function_t*)__clove_vector_add_slot(&clove_functions);
                each_funct->name = sym_name;
                each_funct->obj_ptr = (void*)(module.address + sym->st_value);
            }
        }
    }

    //Sort Symbols Alphanumeric ascendent
    __clove_vector_sort(&clove_functions, __clove_symbols_funct_name_comparator);

    for (size_t i = 0; i < __clove_vector_count(&clove_functions); ++i)
    {
        __clove_symbols_function_t* each_funct = (__clove_symbols_function_t*)__clove_vector_get(&clove_functions, i);
        action(*each_funct, context);
    }
    __clove_vector_free(&clove_functions);
    __clove_symbols_lixux_close_module_handle(&module);
    return 0;
}
#else 
//Not Possible. Shoud be one of the OS cases before
int __clove_symbols_for_each_function_by_prefix(__clove_symbols_context_t* context, __clove_symbols_function_action action) {
    puts("Autodiscovery not yet implemented for this OS!!!");
    return 1;
}
#endif //_WIN32 symbol table
#pragma endregion // Autodiscovery Impl

#pragma region PRIVATE - Run Impl
#define __CLOVE_RUNNER_AUTO() \
char* __clove_exec_path;\
char* __clove_exec_base_path;\
__CLOVE_ASSERT_CHECK_E_DECL() \
__CLOVE_TEST_RESULT_E_DECL() \
__CLOVE_GENERIC_TYPE_E_DECL() \
int main(int argc, char* argv[]) {\
    return __clove_runner_auto(argc, argv);\
}

int __clove_runner_auto(int argc, char* argv[]) {
    __clove_exec_path = argv[0];
    __clove_exec_base_path = __clove_path_basepath(argv[0]);

    //argc = 5;
    //const char* argv2[] = {"exec", "-i", "*.ActShortThanExpForthCharDiff", "-r", "pretty"};
    //const char* argv2[] = {"exec", "-l", "-r", "csv"};

    __clove_vector_t cmd_handlers;
    __CLOVE_VECTOR_INIT(&cmd_handlers, __clove_cmdline_handler_f);
    __CLOVE_VECTOR_ADD(&cmd_handlers, __clove_cmdline_handler_f, __clove_cmdline_handle_help);
    __CLOVE_VECTOR_ADD(&cmd_handlers, __clove_cmdline_handler_f, __clove_cmdline_handle_version);
    __CLOVE_VECTOR_ADD(&cmd_handlers, __clove_cmdline_handler_f, __clove_cmdline_handle_run_tests);
    __CLOVE_VECTOR_ADD(&cmd_handlers, __clove_cmdline_handler_f, __clove_cmdline_handle_list_tests);
    __CLOVE_VECTOR_ADD(&cmd_handlers, __clove_cmdline_handler_f, __clove_cmdline_handle_default);

    __clove_cmdline_errno_t cmd_result = __CLOVE_CMD_ERRNO_INVALID_PARAM;
    __clove_cmdline_t cmdline;
    __clove_cmdline_init(&cmdline, (const char**)argv, argc);

    __CLOVE_VECTOR_FOREACH(&cmd_handlers, __clove_cmdline_handler_f, handler, {
        __clove_cmdline_errno_t result = (*handler)(&cmdline);
        if (result != __CLOVE_CMD_ERRNO_UNMANAGED) {
            cmd_result =  result;
            break;
        }
    });

    __clove_vector_free(&cmd_handlers);
    __clove_cmdline_free(&cmdline);
    free(__clove_exec_base_path);
    return cmd_result;
}

int __clove_run_tests_with_report(__clove_report_t* report, __clove_vector_t* includes,  __clove_vector_t* excludes) {
    //0 => OK
    //1 => ERROR
    //2 => TESTS WITH FAILURES
    
    int run_result = 0;

    __clove_symbols_context_t context;
    context.includes = includes;
    context.excludes = excludes;

    __clove_vector_params_t vector_params = __clove_vector_params_defaulted(sizeof(__clove_suite_t));
    vector_params.item_ctor = __clove_vector_suite_ctor;
    vector_params.item_dtor = __clove_vector_suite_dtor;
    __clove_vector_init(&context.suites, &vector_params);
    context.prefix = "__clove_sym___";
    context.prefix_length = __clove_string_length("__clove_sym___");
    context.suites_count = 0;
    context.tests_count = 0;

    int result = __clove_symbols_for_each_function_by_prefix(&context, __clove_symbols_function_collect);
    if (result == 0) {
        //0 in case of test success/warning
        //1 in case of test failure
        run_result = __clove_exec_suites((__clove_suite_t*)(context.suites.items), context.suites_count, context.tests_count, report);
        if (run_result != 0) run_result = 2;
    }
    else {
        run_result = 1;
    }
    __clove_vector_free(&context.suites);
    return run_result;
}

int __clove_exec_suites(__clove_suite_t* suites, size_t suite_count, size_t test_count, __clove_report_t* report) {
    report->start(report, suite_count, test_count);
    size_t passed = 0;
    size_t failed = 0;
    size_t skipped = 0;

    size_t test_start_counter = 1;
    for (size_t i = 0; i < suite_count; ++i) {
        __clove_suite_t* each_suite = &suites[i];
        report->begin_suite(report, each_suite, i);
        __clove_exec_suite(each_suite, test_start_counter, &passed, &failed, &skipped, report);
        report->end_suite(report, each_suite, i);
        test_start_counter += each_suite->test_count;
    }
    report->end(report, test_count, passed, skipped, failed);
    return failed == 0 ? 0 : 1;
}

void __clove_exec_suite(__clove_suite_t* suite, size_t test_counter, size_t* passed, size_t* failed, size_t* skipped, __clove_report_t* report) {
    __clove_time_t suite_start = __clove_time_now();
    suite->fixtures.setup_once();

    for (size_t i = 0; i < suite->test_count; i++) {
        __clove_test_t* each_test = (__clove_test_t*)__clove_vector_get(&suite->tests, i);
        each_test->result = __CLOVE_TEST_RESULT_SKIPPED;

        __clove_time_t test_start = __clove_time_now();
        suite->fixtures.setup();
        each_test->funct(each_test);
        suite->fixtures.teardown();
        __clove_time_t test_end = __clove_time_now();
        each_test->duration = __clove_time_sub(&test_end, &test_start);

        __CLOVE_SWITCH_BEG(each_test->result) 
            __CLOVE_SWITCH_CASE(__CLOVE_TEST_RESULT_PASSED)  { (*passed)++;  suite->issue.passed_count++; }
            __CLOVE_SWITCH_CASE(__CLOVE_TEST_RESULT_FAILED)  { (*failed)++;  suite->issue.failed_count++; }
            __CLOVE_SWITCH_CASE(__CLOVE_TEST_RESULT_SKIPPED) { (*skipped)++; suite->issue.skipped_count++;}
        __CLOVE_SWITCH_END()

        report->end_test(report, suite, each_test, test_counter + i);
    }
    suite->fixtures.teardown_once();
    __clove_time_t suite_end = __clove_time_now();
    suite->issue.duration = __clove_time_sub(&suite_end, &suite_start);
}
#pragma endregion // Run Impl

#endif //CLOVE_IMPLEMENTATION
#pragma endregion // IMPLEMENTATION

#pragma endregion // PRIVATE

#pragma region PUBLIC
#pragma region PUBLIC - UTILS
/*
 * Provide the executable path
 */
#define CLOVE_EXEC_PATH() __clove_get_exec_path()
 /*
  * Provide the executable base path
  */
#define CLOVE_EXEC_BASE_PATH() __clove_get_exec_base_path()
#pragma endregion //UTILS

#pragma region PUBLIC - ASSERTS
#define CLOVE_FAIL()                       do { __CLOVE_ASSERT_GUARD __clove_assert_fail(_this); } while(0)
#define CLOVE_PASS()                       do { __CLOVE_ASSERT_GUARD __clove_assert_pass(_this); } while(0)
          
#define CLOVE_IS_TRUE(res)                 do { __CLOVE_ASSERT_GUARD __clove_assert_bool(__CLOVE_ASSERT_EQ, true, res, _this); } while(0)
#define CLOVE_IS_FALSE(res)                do { __CLOVE_ASSERT_GUARD __clove_assert_bool(__CLOVE_ASSERT_EQ, false, res, _this);  } while(0)
          
#define CLOVE_CHAR_EQ(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_char(__CLOVE_ASSERT_EQ, exp, res, _this); } while(0)
#define CLOVE_CHAR_NE(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_char(__CLOVE_ASSERT_NE, exp, res, _this); } while(0)
#define CLOVE_CHAR_GT(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_char(__CLOVE_ASSERT_GT, exp, res, _this); } while(0)
#define CLOVE_CHAR_GTE(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_char(__CLOVE_ASSERT_GTE, exp, res, _this); } while(0)
#define CLOVE_CHAR_LT(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_char(__CLOVE_ASSERT_LT, exp, res, _this); } while(0)
#define CLOVE_CHAR_LTE(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_char(__CLOVE_ASSERT_LTE, exp, res, _this); } while(0)
          
#define CLOVE_INT_EQ(exp, res)             do { __CLOVE_ASSERT_GUARD __clove_assert_int(__CLOVE_ASSERT_EQ, exp, res, _this); } while(0)
#define CLOVE_INT_NE(exp, res)             do { __CLOVE_ASSERT_GUARD __clove_assert_int(__CLOVE_ASSERT_NE, exp, res, _this); } while(0)
#define CLOVE_INT_GT(exp, res)             do { __CLOVE_ASSERT_GUARD __clove_assert_int(__CLOVE_ASSERT_GT, exp, res, _this); } while(0)
#define CLOVE_INT_GTE(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_int(__CLOVE_ASSERT_GTE, exp, res, _this); } while(0)
#define CLOVE_INT_LT(exp, res)             do { __CLOVE_ASSERT_GUARD __clove_assert_int(__CLOVE_ASSERT_LT, exp, res, _this); } while(0)
#define CLOVE_INT_LTE(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_int(__CLOVE_ASSERT_LTE, exp, res, _this); } while(0)
          
#define CLOVE_UINT_EQ(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_uint(__CLOVE_ASSERT_EQ, exp, res, _this); } while(0)
#define CLOVE_UINT_NE(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_uint(__CLOVE_ASSERT_NE, exp, res, _this); } while(0)
#define CLOVE_UINT_GT(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_uint(__CLOVE_ASSERT_GT, exp, res, _this); } while(0)
#define CLOVE_UINT_GTE(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_uint(__CLOVE_ASSERT_GTE, exp, res, _this); } while(0)
#define CLOVE_UINT_LT(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_uint(__CLOVE_ASSERT_LT, exp, res, _this); } while(0)
#define CLOVE_UINT_LTE(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_uint(__CLOVE_ASSERT_LTE, exp, res, _this); } while(0)
          
#define CLOVE_LONG_EQ(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_long(__CLOVE_ASSERT_EQ, exp, res, _this); } while(0)
#define CLOVE_LONG_NE(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_long(__CLOVE_ASSERT_NE, exp, res, _this); } while(0)
#define CLOVE_LONG_GT(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_long(__CLOVE_ASSERT_GT, exp, res, _this); } while(0)
#define CLOVE_LONG_GTE(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_long(__CLOVE_ASSERT_GTE, exp, res, _this); } while(0)
#define CLOVE_LONG_LT(exp, res)            do { __CLOVE_ASSERT_GUARD __clove_assert_long(__CLOVE_ASSERT_LT, exp, res, _this); } while(0)
#define CLOVE_LONG_LTE(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_long(__CLOVE_ASSERT_LTE, exp, res, _this); } while(0)
       
#define CLOVE_ULONG_EQ(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_ulong(__CLOVE_ASSERT_EQ, exp, res, _this); } while(0)
#define CLOVE_ULONG_NE(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_ulong(__CLOVE_ASSERT_NE, exp, res, _this); } while(0)
#define CLOVE_ULONG_GT(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_ulong(__CLOVE_ASSERT_GT, exp, res, _this); } while(0)
#define CLOVE_ULONG_GTE(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_ulong(__CLOVE_ASSERT_GTE, exp, res, _this); } while(0)
#define CLOVE_ULONG_LT(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_ulong(__CLOVE_ASSERT_LT, exp, res, _this); } while(0)
#define CLOVE_ULONG_LTE(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_ulong(__CLOVE_ASSERT_LTE, exp, res, _this); } while(0)
       
#define CLOVE_LLONG_EQ(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_llong(__CLOVE_ASSERT_EQ, exp, res, _this); } while(0)
#define CLOVE_LLONG_NE(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_llong(__CLOVE_ASSERT_NE, exp, res, _this); } while(0)
#define CLOVE_LLONG_GT(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_llong(__CLOVE_ASSERT_GT, exp, res, _this); } while(0)
#define CLOVE_LLONG_GTE(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_llong(__CLOVE_ASSERT_GTE, exp, res, _this); } while(0)
#define CLOVE_LLONG_LT(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_llong(__CLOVE_ASSERT_LT, exp, res, _this); } while(0)
#define CLOVE_LLONG_LTE(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_llong(__CLOVE_ASSERT_LTE, exp, res, _this); } while(0)
       
#define CLOVE_ULLONG_EQ(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_ullong(__CLOVE_ASSERT_EQ, exp, res, _this); } while(0)
#define CLOVE_ULLONG_NE(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_ullong(__CLOVE_ASSERT_NE, exp, res, _this); } while(0)
#define CLOVE_ULLONG_GT(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_ullong(__CLOVE_ASSERT_GT, exp, res, _this); } while(0)
#define CLOVE_ULLONG_GTE(exp, res)         do { __CLOVE_ASSERT_GUARD __clove_assert_ullong(__CLOVE_ASSERT_GTE, exp, res, _this); } while(0)
#define CLOVE_ULLONG_LT(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_ullong(__CLOVE_ASSERT_LT, exp, res, _this); } while(0)
#define CLOVE_ULLONG_LTE(exp, res)         do { __CLOVE_ASSERT_GUARD __clove_assert_ullong(__CLOVE_ASSERT_LTE, exp, res, _this); } while(0)
       
#define CLOVE_SIZET_EQ(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_sizet(__CLOVE_ASSERT_EQ, exp, res, _this); } while(0)
#define CLOVE_SIZET_NE(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_sizet(__CLOVE_ASSERT_NE, exp, res, _this); } while(0)
#define CLOVE_SIZET_GT(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_sizet(__CLOVE_ASSERT_GT, exp, res, _this); } while(0)
#define CLOVE_SIZET_GTE(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_sizet(__CLOVE_ASSERT_GTE, exp, res, _this); } while(0)
#define CLOVE_SIZET_LT(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_sizet(__CLOVE_ASSERT_LT, exp, res, _this); } while(0)
#define CLOVE_SIZET_LTE(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_sizet(__CLOVE_ASSERT_LTE, exp, res, _this); } while(0)
       
#define CLOVE_FLOAT_EQ(exp, res)           CLOVE_FLOAT_EQ_P(exp, res, (unsigned char)6)
#define CLOVE_FLOAT_NE(exp, res)           CLOVE_FLOAT_NE_P(exp, res, (unsigned char)6)
#define CLOVE_FLOAT_EQ_P(exp, res, prec)   do { __CLOVE_ASSERT_GUARD __clove_assert_float(__CLOVE_ASSERT_EQ, exp, res, (unsigned char)prec, _this); } while(0)
#define CLOVE_FLOAT_NE_P(exp, res, prec)   do { __CLOVE_ASSERT_GUARD __clove_assert_float(__CLOVE_ASSERT_NE, exp, res, (unsigned char)prec, _this); } while(0)
#define CLOVE_FLOAT_GT(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_float(__CLOVE_ASSERT_GT, exp, res, (unsigned char)6, _this); } while(0)
#define CLOVE_FLOAT_GTE(exp, res)          do {  __CLOVE_ASSERT_GUARD __clove_assert_float(__CLOVE_ASSERT_GTE, exp, res, (unsigned char)6, _this); } while(0)
#define CLOVE_FLOAT_LT(exp, res)           do { __CLOVE_ASSERT_GUARD __clove_assert_float(__CLOVE_ASSERT_LT, exp, res, (unsigned char)6, _this); } while(0)
#define CLOVE_FLOAT_LTE(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_float(__CLOVE_ASSERT_LTE, exp, res, (unsigned char)6, _this); } while(0)
  
#define CLOVE_DOUBLE_EQ(exp, res)          CLOVE_DOUBLE_EQ_P(exp, res, (unsigned char)15)
#define CLOVE_DOUBLE_NE(exp, res)          CLOVE_DOUBLE_NE_P(exp, res, (unsigned char)15)
#define CLOVE_DOUBLE_EQ_P(exp, res, prec)  do { __CLOVE_ASSERT_GUARD __clove_assert_double(__CLOVE_ASSERT_EQ, exp, res, prec, _this); } while(0)
#define CLOVE_DOUBLE_NE_P(exp, res, prec)  do { __CLOVE_ASSERT_GUARD __clove_assert_double(__CLOVE_ASSERT_NE, exp, res, prec, _this); } while(0)
#define CLOVE_DOUBLE_GT(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_double(__CLOVE_ASSERT_GT, exp, res, (unsigned char)15, _this); } while(0)
#define CLOVE_DOUBLE_GTE(exp, res)         do { __CLOVE_ASSERT_GUARD __clove_assert_double(__CLOVE_ASSERT_GTE, exp, res, (unsigned char)15, _this); } while(0)
#define CLOVE_DOUBLE_LT(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_double(__CLOVE_ASSERT_LT, exp, res, (unsigned char)15, _this); } while(0)
#define CLOVE_DOUBLE_LTE(exp, res)         do { __CLOVE_ASSERT_GUARD __clove_assert_double(__CLOVE_ASSERT_LTE, exp, res, (unsigned char)15, _this); } while(0)
       
#define CLOVE_NULL(res)                    do { __CLOVE_ASSERT_GUARD __clove_assert_null(__CLOVE_ASSERT_EQ, NULL, (void*)res, _this); } while(0)
#define CLOVE_NOT_NULL(res)                do { __CLOVE_ASSERT_GUARD __clove_assert_null(__CLOVE_ASSERT_NE, NULL, (void*)res, _this); } while(0)
       
#define CLOVE_PTR_EQ(p1, p2)               do { __CLOVE_ASSERT_GUARD __clove_assert_ptr(__CLOVE_ASSERT_EQ, (void*)p1, (void*)p2, _this); } while(0)
#define CLOVE_PTR_NE(p1, p2)               do { __CLOVE_ASSERT_GUARD __clove_assert_ptr(__CLOVE_ASSERT_NE, (void*)p1, (void*)p2, _this); } while(0)
#define CLOVE_PTR_GT(p1, p2)               do { __CLOVE_ASSERT_GUARD __clove_assert_ptr(__CLOVE_ASSERT_GT, (void*)p1, (void*)p2, _this); } while(0)
#define CLOVE_PTR_GTE(p1, p2)              do { __CLOVE_ASSERT_GUARD __clove_assert_ptr(__CLOVE_ASSERT_GTE, (void*)p1, (void*)p2, _this); } while(0)
#define CLOVE_PTR_LT(p1, p2)               do { __CLOVE_ASSERT_GUARD __clove_assert_ptr(__CLOVE_ASSERT_LT, (void*)p1, (void*)p2, _this); } while(0)
#define CLOVE_PTR_LTE(p1, p2)              do { __CLOVE_ASSERT_GUARD __clove_assert_ptr(__CLOVE_ASSERT_LTE, (void*)p1, (void*)p2, _this); } while(0)
       
#define CLOVE_STRING_EQ(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_string(__CLOVE_ASSERT_EQ, exp, res, _this); } while(0)
#define CLOVE_STRING_NE(exp, res)          do { __CLOVE_ASSERT_GUARD __clove_assert_string(__CLOVE_ASSERT_NE, exp, res, _this); } while(0)
#pragma endregion //ASSERTS

#pragma region PUBLIC - SUITE
  //CLOVE_SUITE_NAME must be defined by the user
#define CLOVE_SUITE_SETUP_ONCE() __CLOVE_SUITE_SETUP_ONCE_AUTO()
#define CLOVE_SUITE_TEARDOWN_ONCE() __CLOVE_SUITE_TEARDOWN_ONCE_AUTO()
#define CLOVE_SUITE_SETUP() __CLOVE_SUITE_SETUP_AUTO()
#define CLOVE_SUITE_TEARDOWN() __CLOVE_SUITE_TEARDOWN_AUTO()
#define CLOVE_TEST(title) __CLOVE_TEST_AUTO(title)
#pragma endregion // SUITE

#pragma region PUBLIC - RUNNER
#define CLOVE_RUNNER() __CLOVE_RUNNER_AUTO()
#pragma endregion // RUNNER

#pragma endregion //PUBLIC

#if defined(__GNUC__) && __GNUC__ <= 12 // GNU Unknown Pragma Fix: END
#pragma GCC diagnostic pop
#endif 

#endif //__CLOVE_H
