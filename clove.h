/* 
 * clove-unit
 * v2.0.1
 * Unit Testing library for C
 * https://github.com/fdefelici/clove-unit
 * 
 */

#ifndef CLOVE_H
#define CLOVE_H

#pragma region PRIVATE APIs
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
} __clove_vector_t;

#define __CLOVE_VECTOR_DEFAULT_PARAMS { .item_size = 0, .initial_capacity = 10, .item_ctor = NULL, .item_dtor = NULL }

static void __clove_vector_init(__clove_vector_t* vector, __clove_vector_params_t* params) {
    vector->capacity = params->initial_capacity;
    vector->count = 0;
    vector->item_size = params->item_size;
    vector->items = (unsigned char*)malloc( vector->item_size * vector->capacity);
    vector->item_ctor = params->item_ctor;
    vector->item_dtor = params->item_dtor;
}

static size_t __clove_vector_count(__clove_vector_t* vector) {    
    return vector->count;
}

static void* __clove_vector_next(__clove_vector_t* vector) {
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
    if (vector->count == 0) return NULL;
    if (index < 0) return NULL;
    if (index >= vector->count) return NULL;
    size_t byte_index = index * vector->item_size;
    return (void*)&(vector->items[byte_index]);
}

static void __clove_vector_free(__clove_vector_t* vector) {    
    if (vector->item_dtor) {
        for(int i=0; i< vector->count; ++i) {
            void* item = __clove_vector_get(vector, i);
            vector->item_dtor(item);
        }
    }
    free(vector->items);
    vector->capacity = 0;
    vector->count = 0;
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
#endif

typedef struct __clove_test_t {
    //char name[__CLOVE_TEST_ENTRY_LENGTH];
    char* name;
    void (*funct)(struct __clove_test_t *);
    unsigned int result;
    char file_name[__CLOVE_STRING_LENGTH];
    unsigned int line;
    char err_msg[__CLOVE_STRING_LENGTH];
} __clove_test;

typedef struct __clove_suite {
    char* name;
    //__clove_test* tests;
    __clove_vector_t tests;
    int test_count;
    struct {
        void (*setup_once)();
        void (*teardown_once)();
        void (*setup)();
        void (*teardown)();
    } fixtures;
} __clove_suite_t;


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


static void __clove_fail(char *msg, __clove_test *_this) {
    _this->result = __CLOVE_TEST_FAILED;
    strcpy_s(_this->err_msg, __CLOVE_STRING_LENGTH, msg);
}

static void __clove_pass(char *msg, __clove_test *_this) {
    _this->result = __CLOVE_TEST_PASSED;
    strcpy_s(_this->err_msg, __CLOVE_STRING_LENGTH, msg);
}

static void __clove_check_int(const unsigned int check_mode, int expected, int result, __clove_test *_this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%d] but was [%d]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_uint(const unsigned int check_mode, unsigned int expected, unsigned int result, __clove_test *_this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%u] but was [%u]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_long(const unsigned int check_mode, long expected, long result, __clove_test *_this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%ld] but was [%ld]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_ulong(const unsigned int check_mode, unsigned long expected, unsigned long result, __clove_test *_this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%lu] but was [%lu]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_llong(const unsigned int check_mode, long long expected, long long result, __clove_test *_this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%lld] but was [%lld]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_ullong(const unsigned int check_mode, unsigned long long expected, unsigned long long result, __clove_test *_this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%llu] but was [%llu]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_char(const unsigned int check_mode, char expected, char result, __clove_test *_this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = expected == result; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = expected != result; }
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%c] but was [%c]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_bool(const unsigned int check_mode, int result, __clove_test *_this) {
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
    } else if (check_mode == __CLOVE_ASSERT_CHECK_FALSE) { 
        pass_scenario = 0 == result; // 0 is FALSE
        strcpy_s(expectedBool, 6, "false"); 
        strcpy_s(resultBool, 6, "true");
     }
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%s] but was [%s]", expectedBool, resultBool);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_null(const unsigned int check_mode, void *result, __clove_test *_this) {
    int pass_scenario;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) pass_scenario = result == NULL;
    else pass_scenario = result != NULL;
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%s] but was [%p]", "NULL", result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_ptr(const unsigned int check_mode, void *expected, void *result, __clove_test* _this) {
    int pass_scenario;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) pass_scenario = expected == result;
    else pass_scenario = expected != result;

    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%p] but was [%p]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_float(const unsigned int check_mode, float expected, float result, __clove_test* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = fabsf(expected - result) <= __CLOVE_FLOATING_PRECISION; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = fabsf(expected - result) > __CLOVE_FLOATING_PRECISION; }
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%f] but was [%f]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_double(const unsigned int check_mode, double expected, double result, __clove_test* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = (float)fabs(expected - result) <= __CLOVE_FLOATING_PRECISION; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = (float)fabs(expected - result) > __CLOVE_FLOATING_PRECISION; }
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%f] but was [%f]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_check_string(const unsigned int check_mode, const char* expected, const char* result, __clove_test* _this) {
    int pass_scenario = 0;
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = strcmp(expected, result) == 0; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = strcmp(expected, result) != 0; }
    
    if (pass_scenario) {
        __clove_pass("", _this);
    } else {
        char msg[__CLOVE_STRING_LENGTH];
        sprintf(msg, "expected [%s] but was [%s]", expected, result);
        __clove_fail(msg, _this);
    }
}

static void __clove_pad_right(char *result, char *strToPad) {
    int targetStrLen = __CLOVE_TEST_ENTRY_LENGTH;           // Target output length  
    const char *padding="...................................................................................";

    int padLen = targetStrLen - strlen(strToPad); // Calc Padding length
    if(padLen < 0) padLen = 0;    // Avoid negative length

    // FORMAT SPECIFIER: https://www.tutorialspoint.com/format-specifiers-in-c
    // %* => minimal length taken from the padding string
    // .* => precision, exact length of the string taken from the padding string
    sprintf(result, "%s%*.*s", strToPad, padLen, padLen, padding);  // LEFT Padding 
}

static const char* __clove_rel_src(const char* path) {
    //https://stackoverflow.com/questions/9834067/difference-between-char-and-const-char
    const char* subpath = strstr(path, "\\src");
    if (subpath == NULL) subpath = strstr(path, "\\tests");
    if (subpath == NULL) return path;
    return subpath+1;
}

static void __clove_exec_suite(__clove_suite_t *suite, int test_counter, unsigned int* passed, unsigned int* failed, unsigned int* skipped) {
    suite->fixtures.setup_once();

    for(int i=0; i<suite->test_count; i++) {
        __clove_test* each_test = (__clove_test*)__clove_vector_get(&suite->tests, i);
        each_test->result = __CLOVE_TEST_SKIPPED;
        
        suite->fixtures.setup();
        each_test->funct(each_test);
        suite->fixtures.teardown();

        char result[__CLOVE_STRING_LENGTH], strToPad[__CLOVE_TEST_ENTRY_LENGTH];
        snprintf(strToPad, __CLOVE_TEST_ENTRY_LENGTH, "%d) %s.%s", test_counter+i, suite->name, each_test->name);
        __clove_pad_right(result, strToPad);

        switch(each_test->result) {
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
    for(int i=0; i < suite_count; ++i) {
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
    int size = strlen(str);
    for(int i=0; i<size; ++i) {
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
        bytes_count = strlen(path);
    } else {
        bytes_count = last_addr - path;
    }
    int size = bytes_count + 1; //considera il  null terminator

    char* base_path = (char*)malloc(sizeof(char) * size);
    strncpy_s(base_path, size, path, bytes_count);
    return base_path;
}

static void __clove_empty_funct() { }

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
        __clove_suite_t* suite = (__clove_suite_t*)__clove_vector_next(&suites); \
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
    static void (*func_ptr[])(__clove_test*) = {__VA_ARGS__};\
    static char functs_as_str[] = #__VA_ARGS__;\
    int test_count = sizeof(func_ptr) / sizeof(func_ptr[0]);\
    _this_suite->name = name;\
    _this_suite->test_count = test_count;\
    __clove_vector_params_t vector_params = __CLOVE_VECTOR_DEFAULT_PARAMS; \
    vector_params.item_size = sizeof(__clove_test); \
    vector_params.initial_capacity = test_count; \
    vector_params.item_ctor = __clove_vector_test_ctor; \
    __clove_vector_init(&_this_suite->tests, &vector_params); \
    char *context = NULL;\
    for(int i=0; i < test_count; ++i) {\
        char *token;\
        if (i==0) { token = strtok_s(functs_as_str, ", ", &context); }\
        else { token = strtok_s(NULL, ", ", &context); }\
        __clove_test* test = __clove_vector_next(&_this_suite->tests); \
        test->name = token;\
        test->funct = (*func_ptr[i]);\
    }\
}

#define __CLOVE_TEST_MANUAL(title) static void title(__clove_test *_this) 

#pragma endregion //Manual


#pragma region Private APIs - Autodiscovery

#include <windows.h>

static PIMAGE_EXPORT_DIRECTORY __clove_symbols_get_export_table_from (HMODULE module)
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

typedef struct __clove_symbols_context_t {
    __clove_suite_t* last_suite;
    __clove_vector_t suites;
    int suites_count;
    int tests_count;
    int prefix_length;
} __clove_symbols_context_t;

typedef struct {
    char* name;
    void* pointer;
} __clove_symbols_function_t;

typedef void (*__clove_symbols_function_action)(__clove_symbols_function_t, void* context);


static void __clove_vector_test_ctor(__clove_test* test) {
    memset(test, 0, sizeof(__clove_test));
}

static void __clove_vector_suite_ctor(__clove_suite_t* suite) {
    suite->name = NULL;
    suite->fixtures.setup_once = __clove_empty_funct;
    suite->fixtures.teardown_once = __clove_empty_funct;
    suite->fixtures.setup = __clove_empty_funct;
    suite->fixtures.teardown = __clove_empty_funct;
    
    __clove_vector_params_t params = __CLOVE_VECTOR_DEFAULT_PARAMS;
    params.item_size = sizeof(__clove_test);
    params.item_ctor = __clove_vector_test_ctor;
    __clove_vector_init(&(suite->tests), &params);
    suite->test_count = 0;
}

static void __clove_vector_suite_dtor(__clove_suite_t* suite) {
    free(suite->name);
    __clove_vector_free(&suite->tests);
}

static void __clove_vector_suite_ctor_manual(__clove_suite_t* suite) {
    suite->name = NULL;
    suite->fixtures.setup_once = __clove_empty_funct;
    suite->fixtures.teardown_once = __clove_empty_funct;
    suite->fixtures.setup = __clove_empty_funct;
    suite->fixtures.teardown = __clove_empty_funct;
    
    /*
    __clove_vector_params_t params = __CLOVE_VECTOR_DEFAULT_PARAMS;
    params.item_size = sizeof(__clove_test);
    params.item_ctor = __clove_vector_test_ctor;
    __clove_vector_init(&(suite->tests), &params);
    */
    suite->test_count = 0;
}

static void __clove_vector_suite_dtor_manual(__clove_suite_t* suite) {
    //free(suite->name);
    __clove_vector_free(&suite->tests);
}

static int __clove_symbols_for_each_function_by_prefix(const char* prefix, __clove_symbols_function_action action, void* action_context) {    
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

    int prefix_length = strlen(prefix);

    //Takes advantage of symbols are lexically sorted. 
    //And also that prefix starting with "__" will come first. So no need to start a binary search
    unsigned char match_ongoing = 0;
    for(int i=0; i < export_dir->NumberOfNames; ++i )
    {           
        char* each_name = (char*)(base_addr + names_ptr[i]);
        unsigned long each_ordinal = ordinal_base_number + ordinals_ptr[i];
        unsigned long* each_funct_addr = (PDWORD)(base_addr + functs_address_ptr[each_ordinal - ordinal_base_number]); 
        printf("%lu) %s [0x%p]\n", each_ordinal, each_name, each_funct_addr);
        
        if (strncmp(prefix, each_name, prefix_length) == 0) {
            if (!match_ongoing) match_ongoing = 1;
            __clove_symbols_function_t funct = { each_name, each_funct_addr};
            action(funct, action_context);
        } else {
            //At the first failure, if match was ongoing then there are no more symbol with that prefix
            if (match_ongoing) break;
        }
    }
    return 0;
}

static void __clove_symbols_function_collect(__clove_symbols_function_t exported_funct, __clove_symbols_context_t* context) {
    static char* end_suite_separator = "___";
    static int end_suite_separator_length = 3;
    static char* test_separator = "20_";
    static int test_separator_length = 3;


    char* test_full_name = exported_funct.name;

    char* begin_suite_name = test_full_name + context->prefix_length;
    char* end_suite_name = strstr(begin_suite_name, end_suite_separator);
    int size = end_suite_name - begin_suite_name;

    char* suite_name = (char*)malloc(size+1); //size+1 for null terminator
    strncpy_s(suite_name, size+1, begin_suite_name, size);
    suite_name[size] = '\0'; //maybe could be avoided?!

    //if suite changes, then set as next suite to collect the tests
    if (context->last_suite == NULL || strcmp(suite_name, context->last_suite->name) != 0 ) {  
        context->last_suite = (__clove_suite_t*)__clove_vector_next(&context->suites);
        context->last_suite->name = suite_name;
        context->suites_count++;
    }

    char* test_name = end_suite_name + end_suite_separator_length;
    if (test_name[0] == '1' && test_name[1] == '1') {
        context->last_suite->fixtures.setup_once = (void (*)())exported_funct.pointer;
    } else if (test_name[0] == '1' && test_name[1] == '2') {
        context->last_suite->fixtures.teardown_once = (void (*)())exported_funct.pointer;
    } else if (test_name[0] == '1' && test_name[1] == '3') { 
        context->last_suite->fixtures.setup = (void (*)())exported_funct.pointer;
    } else if (test_name[0] == '1' && test_name[1] == '4') { 
        context->last_suite->fixtures.teardown = (void (*)())exported_funct.pointer;
    } else if (test_name[0] == '2' && test_name[1] == '0') { 
        __clove_test* test = (__clove_test*)__clove_vector_next(&context->last_suite->tests);
        test->name = test_name + test_separator_length;
        test->funct = (void (*)(struct __clove_test_t *))exported_funct.pointer;
        context->last_suite->test_count++;
        context->tests_count++;
    }
}


#define __CLOVE_RUNNER_AUTO() \
char* __clove_exec_path;\
char* __clove_exec_base_path;\
int main(int argc, char* argv[]) {\
    __clove_setup_ansi_console();\
    __clove_exec_path = argv[0]; \
    __clove_exec_base_path = __clove_basepath(argv[0]); \
    __clove_symbols_context_t __context; \
    __clove_vector_params_t vector_params = __CLOVE_VECTOR_DEFAULT_PARAMS; \
    vector_params.item_size = sizeof(__clove_suite_t); \
    vector_params.item_ctor = __clove_vector_suite_ctor; \
    vector_params.item_dtor = __clove_vector_suite_dtor; \
    __clove_vector_init(&__context.suites, &vector_params); \
    __context.suites_count = 0; \
    __context.last_suite = NULL; \
    __context.prefix_length = strlen("__clove_sym___"); \
    __context.tests_count = 0; \
    __clove_symbols_for_each_function_by_prefix("__clove_sym___", __clove_symbols_function_collect, &__context); \
    __clove_exec_suites((__clove_suite_t*)(__context.suites.items), __context.suites_count, __context.tests_count); \
    free(__clove_exec_base_path); \
    __clove_vector_free(&__context.suites); \
    return 0;\
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
#define __CLOVE_TEST_AUTO(title) __CLOVE_SUITE_METHOD_AUTO_1( CLOVE_SUITE_NAME, 20_ ## title, __clove_test *_this)
#pragma endregion //Autodiscovery
#pragma endregion // Private APIs

#pragma region PUBLIC APIs
#pragma region Public APIs - UTILS
/*
 * Provide the executable path
 */
#define CLOVE_EXEC_PATH __clove_exec_base_path
/*
 * Provide the executable base path
 */
#define CLOVE_EXEC_BASE_PATH __clove_exec_base_path
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
#define CLOVE_NULL(res) __CLOVE_TEST_GUARD __clove_check_null(__CLOVE_ASSERT_CHECK_EQUALITY, res, _this);
#define CLOVE_NOT_NULL(res) __CLOVE_TEST_GUARD __clove_check_null(__CLOVE_ASSERT_CHECK_DIFFERENCE, res, _this);
#define CLOVE_PTR_EQ(p1, p2) __CLOVE_TEST_GUARD __clove_check_ptr(__CLOVE_ASSERT_CHECK_EQUALITY, p1, p2, _this);
#define CLOVE_PTR_NE(p1, p2) __CLOVE_TEST_GUARD __clove_check_ptr(__CLOVE_ASSERT_CHECK_DIFFERENCE, p1, p2, _this);
#define CLOVE_STRING_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_string(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_STRING_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_string(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#pragma endregion //ASSERTIONS


#if defined(CLOVE_ENABLE_AUTODISCOVERY) || defined(CLOVE_SUITE_NAME)
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
