/* 
 * clove-unit
 * v2.0.1
 * Unit Testing library for C
 * https://github.com/fdefelici/clove-unit
 * 
 */

#ifndef CLOVE_H
#define CLOVE_H

#pragma region INTERNALS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
    __clove_test* tests;
    int test_count;
    void (*setup_funct)();
    void (*teardown_funct)();
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
    for(int i=0; i<suite->test_count; i++) {
        __clove_test* each_test = &suite->tests[i];
        each_test->result = __CLOVE_TEST_SKIPPED;
        
        suite->setup_funct();
        each_test->funct(each_test);
        suite->teardown_funct();

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
    #pragma endregion
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
#pragma endregion

#pragma region PUBLIC APIs
/*
 * Provide the executable path
 */
#define CLOVE_EXEC_PATH __clove_exec_base_path
/*
 * Provide the executable base path
 */
#define CLOVE_EXEC_BASE_PATH __clove_exec_base_path

/*
 * Test Execution.
 * Take as input a list of suites.
 */
#define CLOVE_RUNNER(...) \
char* __clove_exec_path;\
char* __clove_exec_base_path;\
int main(int argc, char* argv[]) {\
    __clove_setup_ansi_console();\
    __clove_exec_path = argv[0]; \
    __clove_exec_base_path = __clove_basepath(argv[0]); \
    static void (*suite_ptr[])(__clove_suite_t*) = {__VA_ARGS__};\
    int suite_count = sizeof(suite_ptr) / sizeof(suite_ptr[0]);\
    __clove_suite_t* suites = (__clove_suite_t*)calloc(suite_count, sizeof(__clove_suite_t));\
    int test_count = 0;\
    for(int i=0; i < suite_count; ++i) {\
        suite_ptr[i](&suites[i]);\
        test_count += suites[i].test_count;\
    }\
    __clove_exec_suites(suites, suite_count, test_count);\
    free(suites); \
    free(__clove_exec_base_path); \
    return 0;\
}
// - single # will create a string from the given argument
// - double ## will create a new token by concatenating the arguments


static void __clove_empty_funct() { }

// ASSERTIONS
/*
 * Suite declaration. (basically a standard method forward declaration)
 * Useful to work with compilation unit instead of just using header file to implement test and test suite
 */
#define CLOVE_SUITE_DECL(title) void title(__clove_suite_t *_this_suite);

/*
 * Suite Implementation
 */
#define CLOVE_SUITE(title) \
void title(__clove_suite_t *_this_suite) { \
    static char name[] = #title;\
    _this_suite->name = name; \
    _this_suite->setup_funct = __clove_empty_funct; \
    _this_suite->teardown_funct = __clove_empty_funct;
#define CLOVE_SUITE_SETUP(funct) _this_suite->setup_funct = funct;
#define CLOVE_SUITE_TEARDOWN(funct) _this_suite->teardown_funct = funct;
#define CLOVE_SUITE_TESTS(...) \
    static void (*func_ptr[])(__clove_test*) = {__VA_ARGS__};\
    static char functs_as_str[] = #__VA_ARGS__;\
    int test_count = sizeof(func_ptr) / sizeof(func_ptr[0]);\
    _this_suite->name = name;\
    _this_suite->test_count = test_count;\
    _this_suite->tests = (__clove_test*)calloc(test_count, sizeof(__clove_test));\
    char *context = NULL;\
    for(int i=0; i < test_count; ++i) {\
        char *token;\
        if (i==0) { token = strtok_s(functs_as_str, ", ", &context); }\
        else { token = strtok_s(NULL, ", ", &context); }\
        _this_suite->tests[i].name = token;\
        _this_suite->tests[i].funct = (*func_ptr[i]);\
    }\
}

/* 
 * Define a new test named 'title'
 */
#define CLOVE_TEST(title) static void title(__clove_test *_this) 
#define CLOVE_PASS() __CLOVE_TEST_GUARD __clove_pass("", _this);
#define CLOVE_FAIL() __CLOVE_TEST_GUARD __clove_fail("Test meet FAIL assertion!", _this);
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
#pragma endregion

#endif //CLOVE_H
