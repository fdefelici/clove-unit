#define CLOVE_SUITE_NAME UNIT_ReportCsvTest
#include "clove-unit.h"
#include "utils/utils.h"

static __clove_report_run_tests_csv_t* report;
static __clove_stream_file_t* stream;
static __clove_report_params_t params;

CLOVE_SUITE_SETUP() {
    char* file_path = __clove_path_rel_to_abs_exec_path("clove_report.csv");
    stream = __clove_stream_file_new(file_path);
    params.tests_base_path = "";
    report = __clove_report_run_tests_csv_new((__clove_stream_t*)stream, &params);
}

CLOVE_SUITE_TEARDOWN() {
    report->base.free((__clove_report_t*)report);
    stream->base.free((__clove_stream_t*)stream);
}

CLOVE_TEST(EmptyReport) {
    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 0, 0);
    base->end(base, 0, 0, 0, 0);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected = 
    "Suite,Test,Status,Duration,File,Line,Assert,Type,Expected,Actual\n"
    ;

    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithOnePassedTest) {
    __clove_suite_t suite;
    suite.name = "Suite1";
    suite.test_count = 1;

    __clove_test_t test11;
    test11.name = "Test11";
    test11.file_name = "test-file.c";
    test11.result = __CLOVE_TEST_RESULT_PASSED;
    test11.duration.seconds = 0;
    test11.duration.nanos_after_seconds = 100;
   
    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 1, 1);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test11, 1);
    base->end_suite(base, &suite, 0);
    base->end(base, 1, 1, 0, 0);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected = 
    "Suite,Test,Status,Duration,File,Line,Assert,Type,Expected,Actual\n"
    "Suite1,Test11,PASS,100,,,,,,\n"    
    ;
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithTwoTests) {
    __clove_suite_t suite;
    suite.name = "Suite1";
    suite.test_count = 2;

    __clove_test_t test11;
    test11.name = "Test11";
    test11.file_name = "test-file.c";
    test11.result = __CLOVE_TEST_RESULT_PASSED;
    test11.duration.seconds = 0;
    test11.duration.nanos_after_seconds = 100;

    __clove_test_t test12;
    test12.name = "Test12";
    test12.file_name = "test-file.c";
    test12.result = __CLOVE_TEST_RESULT_FAILED;
    test12.duration.seconds = 0;
    test12.duration.nanos_after_seconds = 100;
    test12.issue.line = 8;
    test12.issue.assert = __CLOVE_ASSERT_EQ;
    test12.issue.data_type = __CLOVE_GENERIC_BOOL;
    test12.issue.expected._bool = false;
    test12.issue.actual._bool = true;

    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 1, 2);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test11, 1);
    base->end_test(base, &suite, &test12, 2);
    base->end_suite(base, &suite, 0);
    base->end(base, 2, 1, 0, 1);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected = 
    "Suite,Test,Status,Duration,File,Line,Assert,Type,Expected,Actual\n"
    "Suite1,Test11,PASS,100,,,,,,\n"
    "Suite1,Test12,FAIL,,test-file.c,8,EQ,BOOL,false,true\n"
    ;
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportTwoSuitesWithOnePassedTestEach) {
    __clove_suite_t suite;
    suite.name = "Suite1";
    suite.test_count = 1;

    __clove_test_t test11;
    test11.name = "Test11";
    test11.file_name = "test-file.c";
    test11.result = __CLOVE_TEST_RESULT_PASSED;
    test11.duration.seconds = 0;
    test11.duration.nanos_after_seconds = 100;

    __clove_suite_t suite2;
    suite2.name = "Suite2";
    suite2.test_count = 1;

    __clove_test_t test21;
    test21.name = "Test21";
    test21.file_name = "test-file2.c";
    test21.result = __CLOVE_TEST_RESULT_PASSED;
    test21.duration.seconds = 0;
    test21.duration.nanos_after_seconds = 100;
   
    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 2, 2);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test11, 1);
    base->end_suite(base, &suite, 0);
    base->begin_suite(base, &suite2, 1);
    base->end_test(base, &suite2, &test21, 2);
    base->end_suite(base, &suite2, 1);
    base->end(base, 2, 2, 0, 0);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected =
    "Suite,Test,Status,Duration,File,Line,Assert,Type,Expected,Actual\n"
    "Suite1,Test11,PASS,100,,,,,,\n"
    "Suite2,Test21,PASS,100,,,,,,\n";
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithOneTestFailedWithString) {
    __clove_suite_t suite;
    suite.name = "Suite1";
    suite.test_count = 1;

    __clove_test_t test12;
    test12.name = "Test12";
    test12.file_name = "test-file.c";
    test12.result = __CLOVE_TEST_RESULT_FAILED;
    test12.duration.seconds = 0;
    test12.duration.nanos_after_seconds = 100;
    test12.issue.line = 8;
    test12.issue.assert = __CLOVE_ASSERT_EQ;
    test12.issue.data_type = __CLOVE_GENERIC_STRING;
    test12.issue.expected._string = __clove_string_strdup("Hello");
    test12.issue.actual._string = __clove_string_strdup("World");

    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 1, 1);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test12, 1);
    base->end_suite(base, &suite, 0);
    base->end(base, 1, 0, 0, 1);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected = 
    "Suite,Test,Status,Duration,File,Line,Assert,Type,Expected,Actual\n"
    "Suite1,Test12,FAIL,,test-file.c,8,EQ,STRING,Hello,World\n"
    ;
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithOneTestFailedWithFail) {
    __clove_suite_t suite;
    suite.name = "Suite1";
    suite.test_count = 1;

    __clove_test_t test12;
    test12.name = "Test12";
    test12.file_name = "test-file.c";
    test12.result = __CLOVE_TEST_RESULT_FAILED;
    test12.duration.seconds = 0;
    test12.duration.nanos_after_seconds = 100;
    test12.issue.line = 8;
    test12.issue.assert = __CLOVE_ASSERT_FAIL;
    test12.issue.data_type = NULL;
    test12.issue.expected._ptr = NULL;
    test12.issue.actual._ptr = NULL;

    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 1, 1);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test12, 1);
    base->end_suite(base, &suite, 0);
    base->end(base, 1, 0, 0, 1);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected = 
    "Suite,Test,Status,Duration,File,Line,Assert,Type,Expected,Actual\n"
    "Suite1,Test12,FAIL,,test-file.c,8,FAIL,,,\n"
    ;
    CLOVE_STRING_EQ(expected, actual);
}