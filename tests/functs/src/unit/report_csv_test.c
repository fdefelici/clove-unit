#define CLOVE_SUITE_NAME UNIT_ReportCsvTest
#include "clove-unit.h"
#include "utils/utils.h"
#include "utils/domain_utils.h"

static __clove_report_run_tests_csv_t* report;
static __clove_stream_file_t* stream;
static __clove_report_params_t params;

static __clove_vector_t suites;

CLOVE_SUITE_SETUP() {
    char* file_path = __clove_path_rel_to_abs_exec_path("clove_report.csv");
    stream = __clove_stream_file_new(file_path);
    params.tests_base_path = "abs";
    params.report_detail = __CLOVE_REPORT_DETAIL__PASSED_FAILED_SKIPPED;
    report = __clove_report_run_tests_csv_new((__clove_stream_t*)stream, &params);

    __CLOVE_VECTOR_INIT(&suites, __clove_suite_t);
}

CLOVE_SUITE_TEARDOWN() {
    __CLOVE_VECTOR_FREE(&suites);

    report->base.free((__clove_report_t*)report);
    stream->base.free((__clove_stream_t*)stream);
}

CLOVE_TEST(EmptyReport) {
    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, &suites, 0);
    base->end(base, 0, 0, 0, 0);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected = 
    "Suite,Test,Status,Duration,File,Line,Assert,Type,Expected,Actual\n"
    ;

    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithOnePassedTest) {
    __clove_suite_t suite = create_suite("Suite1");
    __clove_test_t test11 = create_test("Test11");
    
    suite_add_test(&suite, &test11);
    __CLOVE_VECTOR_ADD(&suites, __clove_suite_t, suite);
    
    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, &suites, 1);
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

CLOVE_TEST(ReportOneSuiteWithOneSkippedTest) {
    __clove_suite_t suite = create_suite("Suite1");
    __clove_test_t test11 = create_test_skip("Test11");
    
    suite_add_test(&suite, &test11);
    __CLOVE_VECTOR_ADD(&suites, __clove_suite_t, suite);

    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, &suites, 1);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test11, 1);
    base->end_suite(base, &suite, 0);
    base->end(base, 1, 0, 1, 0);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected = 
    "Suite,Test,Status,Duration,File,Line,Assert,Type,Expected,Actual\n"
    "Suite1,Test11,SKIP,,test_file.c,4,,,,\n"    
    ;
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithTwoTests) {
    __clove_suite_t suite = create_suite("Suite1");
    __clove_test_t test11 = create_test("Test11");
    __clove_test_t test12 = create_test_fail("Test12");

    suite_add_test(&suite, &test11);
    suite_add_test(&suite, &test12);
    __CLOVE_VECTOR_ADD(&suites, __clove_suite_t, suite);

    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, &suites, 2);
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
    "Suite1,Test12,FAIL,,test_file.c,8,EQ,BOOL,false,true\n"
    ;
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportTwoSuitesWithOnePassedTestEach) {
    __clove_suite_t suite = create_suite("Suite1");
    __clove_test_t test11 = create_test("Test11");
    suite_add_test(&suite, &test11);

    __clove_suite_t suite2 = create_suite("Suite2");
    __clove_test_t test21 = create_test("Test21");
    suite_add_test(&suite2, &test21);

    __CLOVE_VECTOR_ADD(&suites, __clove_suite_t, suite);
    __CLOVE_VECTOR_ADD(&suites, __clove_suite_t, suite2);
    
    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, &suites, 2);
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
    __clove_suite_t suite  = create_suite("Suite1");
   
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

    suite_add_test(&suite, &test12);
    __CLOVE_VECTOR_ADD(&suites, __clove_suite_t, suite);

    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, &suites, 1);
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
    __clove_suite_t suite = create_suite("Suite1");

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

    suite_add_test(&suite, &test12);
    __CLOVE_VECTOR_ADD(&suites, __clove_suite_t, suite);

    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, &suites, 1);
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