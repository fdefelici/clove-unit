/* 
 * clove
 * v1.0.4
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
#define __CLOVE_TEST_ENTRY_LENGTH 50

// wrapper for Microsoft API
#ifndef _WIN32
char *strtok_s(char *str, const char *delimiters, char **context) {
    return strtok_r(str, delimiters, context);
}  

int strncpy_s(char *strDest, size_t numberOfElements, const char *strSource, size_t count) {
    return strncpy(strDest, strSource, count) == NULL;
}

int strcpy_s(char *dest, size_t dest_size, const char *src) {
    return strcpy(dest, src) == NULL;
}
#endif

typedef struct __clove_test_t {
    char name[__CLOVE_TEST_ENTRY_LENGTH];
    void (*funct)(struct __clove_test_t *);
    unsigned int result;
    char file_name[__CLOVE_STRING_LENGTH];
    unsigned int line;
    char err_msg[__CLOVE_STRING_LENGTH];
} __clove_test;


#define __CLOVE_TEST_PASSED 1
#define __CLOVE_TEST_FAILED 2
#define __CLOVE_TEST_SKIPPED 3

#define __CLOVE_ASSERT_CHECK_EQUALITY 1
#define __CLOVE_ASSERT_CHECK_DIFFERENCE 2
#define __CLOVE_ASSERT_CHECK_TRUE 3
#define __CLOVE_ASSERT_CHECK_FALSE 4

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
    if (check_mode == __CLOVE_ASSERT_CHECK_EQUALITY) { pass_scenario = fabsf(expected - result) <= 0.000001f; }
    else if (check_mode == __CLOVE_ASSERT_CHECK_DIFFERENCE) { pass_scenario = fabsf(expected - result) > 0.000001f; }
    
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

static void __clove_exec(__clove_test *tests, int numOfTests) {
    printf("%s Executing Test Runner in 'Fail Safe Verbose' mode\n", __CLOVE_INFO);
    printf("%s Tests found: %d \n", __CLOVE_INFO, numOfTests);
    unsigned int passed = 0;
    unsigned int failed = 0;
    unsigned int skipped = 0;
    
    for(int i=0; i<numOfTests; i++) {
        __clove_test* each = &tests[i];
        each->result = __CLOVE_TEST_SKIPPED;
        //if (each.setup) each.setup();
        
        each->funct(each);
        //if (each.teardown) each.teardown();

        char result[__CLOVE_STRING_LENGTH], strToPad[__CLOVE_TEST_ENTRY_LENGTH + 10];
        snprintf(strToPad, __CLOVE_TEST_ENTRY_LENGTH + 10,  "%d) %s", i+1, each->name);
        __clove_pad_right(result, strToPad);

        switch(each->result) {
            case __CLOVE_TEST_PASSED: {
                passed++;
                printf("%s %s%s\n", __CLOVE_INFO, result, __CLOVE_PASSED);
                break;
            }
            case __CLOVE_TEST_FAILED: {
                failed++;
                printf("%s %s%s => %s@%d: %s\n", __CLOVE_ERRO, result, __CLOVE_FAILED, each->file_name, each->line, each->err_msg);
                break;
            }
            case __CLOVE_TEST_SKIPPED: {
                skipped++;
                printf("%s %s%s\n", __CLOVE_WARN, result, __CLOVE_SKIPPED);
                break;
            }
            default:
                break;
        }
    }
    printf("%s Total: %d, Passed: %d, Failed: %d, Skipped: %d\n", __CLOVE_INFO, numOfTests, passed, failed, skipped);
    if (passed == numOfTests) { printf("%s Test result => SUCCESS :-)\n", __CLOVE_INFO); }
    else if (failed > 0) { printf("%s Test result => FAILURE :_(\n", __CLOVE_ERRO); }
    else if (skipped > 0) { printf("%s Test result => OK, but some test has been skipped!\n", __CLOVE_WARN); }
    return;
}

#define __CLOVE_TEST_GUARD if (_this->result==__CLOVE_TEST_FAILED) {return;} strcpy_s(_this->file_name, __CLOVE_STRING_LENGTH, __clove_rel_src(__FILE__)); _this->line=__LINE__;

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

// MAIN
// - single # will create a string from the given argument
// - double ## will create a new token by concatenating the arguments
#define CLOVE_RUNNER(...) \
char* __clove_exec_path;\
char* __clove_exec_base_path;\
int main(int argc, char* argv[]) {\
    __clove_setup_ansi_console();\
    __clove_exec_path = argv[0]; \
    __clove_exec_base_path = __clove_basepath(argv[0]); \
    void (*func_ptr[])(__clove_test*) = {__VA_ARGS__};\
    int testSize = sizeof(func_ptr) / sizeof(func_ptr[0]);\
    __clove_test* tests = (__clove_test*)calloc(testSize, sizeof(__clove_test));\
    char functs_as_str[] = #__VA_ARGS__;\
    for(int i=0; i < testSize; i++) {\
        char *token;\
        char *context;\
        if (i==0) { token = strtok_s(functs_as_str, ", ", &context); }\
        else { token = strtok_s(NULL, ", ", &context); }\
        strcpy_s(tests[i].name, sizeof(tests[i].name), token);\
        tests[i].funct = (*func_ptr[i]);\
    }\
    __clove_exec(tests, testSize); \
    free(tests); \
    free(__clove_exec_base_path); \
    return 0;\
}

// ASSERTIONS

/* 
 * Define a new test named 'title'
 */
#define CLOVE_TEST(title) static void title(__clove_test *_this) 
#define CLOVE_INT_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_int(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_INT_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_int(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#define CLOVE_IS_TRUE(res) __CLOVE_TEST_GUARD __clove_check_bool(__CLOVE_ASSERT_CHECK_TRUE, res, _this);
#define CLOVE_IS_FALSE(res) __CLOVE_TEST_GUARD __clove_check_bool(__CLOVE_ASSERT_CHECK_FALSE, res, _this);
#define CLOVE_NULL(res) __CLOVE_TEST_GUARD __clove_check_null(__CLOVE_ASSERT_CHECK_EQUALITY, res, _this);
#define CLOVE_NOT_NULL(res) __CLOVE_TEST_GUARD __clove_check_null(__CLOVE_ASSERT_CHECK_DIFFERENCE, res, _this);
#define CLOVE_PTR_EQ(p1, p2) __CLOVE_TEST_GUARD __clove_check_ptr(__CLOVE_ASSERT_CHECK_EQUALITY, p1, p2, _this);
#define CLOVE_PTR_NE(p1, p2) __CLOVE_TEST_GUARD __clove_check_ptr(__CLOVE_ASSERT_CHECK_DIFFERENCE, p1, p2, _this);
#define CLOVE_FLOAT_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_float(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_FLOAT_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_float(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#define CLOVE_STRING_EQ(exp, res) __CLOVE_TEST_GUARD __clove_check_string(__CLOVE_ASSERT_CHECK_EQUALITY, exp, res, _this);
#define CLOVE_STRING_NE(exp, res) __CLOVE_TEST_GUARD __clove_check_string(__CLOVE_ASSERT_CHECK_DIFFERENCE, exp, res, _this);
#pragma endregion

#endif //CLOVE_H
