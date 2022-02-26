#define CLOVE_SUITE_NAME ReportJsonTest
#include "clove.h"

CLOVE_TEST(ReportOneSuiteWithTwoTests) {
    __clove_report_json_t* report = __clove_report_json();


    __clove_suite_t suite;
    suite.name = "Suite1";
    suite.test_count = 2;

    __clove_test_t test11;
    test11.name = "Test11";
    strcpy_s(test11.file_name, __CLOVE_STRING_LENGTH, "test-file.c");
    test11.result = __CLOVE_TEST_PASSED;

    __clove_test_t test12;
    test12.name = "Test12";
    strcpy_s(test12.file_name, __CLOVE_STRING_LENGTH, "test-file.c");
    test12.result = __CLOVE_TEST_FAILED;
    test12.line = 8;
    strcpy_s(test12.err_msg, __CLOVE_STRING_LENGTH, "error msg");


    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 2, 6);
    base->test_executed(base, &suite, &test11, 1);
    base->test_executed(base, &suite, &test12, 2);
    base->end(base, 6, 3, 2, 1);

    const char* file_path = ((__clove_report_json_t*)report)->file_path;
    FILE* file = fopen(file_path, "rb");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* result = (char*)malloc(file_size + 1);
    result[file_size] = '\0';
    fread(result, file_size, 1, file);

    puts(result);

    fclose(file);

    const char* expected =
    "{\n"
    "\t\"suite_count\" : 2,\n"
    "\t\"test_count\" : 6,\n"
    "\t\"suites\" : {\n"
    "\t},\n"
    "\t\"test_passed\" : 3,\n"
    "\t\"test_skipped\" : 2,\n"
    "\t\"test_failed\" : 1,\n"
    "\t\"status\" : 2\n"
    "}";

    //puts("");
    //puts(expected);
    //CLOVE_STRING_EQ(expected, result);
}


CLOVE_TEST(ReportTwoSuitesWithOneTestEach) {
    __clove_report_json_t* report = __clove_report_json();


    __clove_suite_t suite;
    suite.name = "Suite1";
    suite.test_count = 1;

    __clove_test_t test11;
    test11.name = "Test11";
    strcpy_s(test11.file_name, __CLOVE_STRING_LENGTH, "test-suite1.c");
    test11.result = __CLOVE_TEST_PASSED;


    __clove_suite_t suite2;
    suite2.name = "Suite2";
    suite2.test_count = 1;

    __clove_test_t test12;
    test12.name = "Test12";
    strcpy_s(test12.file_name, __CLOVE_STRING_LENGTH, "test-suite2.c");
    test12.result = __CLOVE_TEST_FAILED;
    test12.line = 8;
    strcpy_s(test12.err_msg, __CLOVE_STRING_LENGTH, "error msg");


    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 2, 6);
    base->test_executed(base, &suite, &test11, 1);
    base->test_executed(base, &suite2, &test12, 2);
    base->end(base, 6, 3, 2, 1);

    const char* file_path = ((__clove_report_json_t*)report)->file_path;
    FILE* file = fopen(file_path, "rb+");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* result = (char*)malloc(file_size + 1);
    result[file_size] = '\0';
    fread(result, file_size, 1, file);

    puts(result);

    fclose(file);
}
