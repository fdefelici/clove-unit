#define CLOVE_SUITE_NAME UNIT_ReportJsonFullTest
#include "clove-unit.h"
#include "utils/utils.h"
#include "utils/domain_utils.h"

static __clove_report_json_t* report;
static __clove_stream_file_t* stream;
static __clove_report_params_t params;


CLOVE_SUITE_SETUP() {
    char* file_path = __clove_path_rel_to_abs_exec_path("clove_report.json");
    stream = __clove_stream_file_new(file_path);
    params.tests_base_path = "abs";
    params.report_detail = __CLOVE_REPORT_DETAIL__PASSED_FAILED_SKIPPED;
    report = __clove_report_run_tests_json_new((__clove_stream_t*)stream, &params);
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
    "{\n"
    "\t\"clove_version\" : \""__CLOVE_VERSION"\",\n"
    "\t\"json_schema\" : \"1.0\",\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 0,\n"
    "\t\t\"test_count\" : 0,\n"
    "\t\t\"suites\" : {\n"
    "\t\t},\n"
    "\t\t\"test_passed\" : 0,\n"
    "\t\t\"test_skipped\" : 0,\n"
    "\t\t\"test_failed\" : 0,\n"
    "\t\t\"status\" : \"PASS\"\n"
    "\t}\n"
    "}";

    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithOnePassedTest) {
    __clove_suite_t suite = create_suite("Suite1", 0);
    __clove_test_t test11 = create_test("Test11");
    suite_add_test(&suite, &test11);
    
    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 1, 1);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test11, 1);
    base->end_suite(base, &suite, 0);
    base->end(base, 1, 1, 0, 0);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected =
    "{\n"
    "\t\"clove_version\" : \""__CLOVE_VERSION"\",\n"
    "\t\"json_schema\" : \"1.0\",\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 1,\n"
    "\t\t\"test_count\" : 1,\n"
    "\t\t\"suites\" : {\n"
    "\t\t\t\"Suite1\" : {\n"
    "\t\t\t\t\"file\" : \"test_file.c\",\n"
    "\t\t\t\t\"tests\" : {\n"
    "\t\t\t\t\t\"Test11\" : {\n"
    "\t\t\t\t\t\t\"status\" : \"PASS\",\n"
    "\t\t\t\t\t\t\"duration\" : 100\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t}\n"
    "\t\t\t}\n"
    "\t\t},\n"
    "\t\t\"test_passed\" : 1,\n"
    "\t\t\"test_skipped\" : 0,\n"
    "\t\t\"test_failed\" : 0,\n"
    "\t\t\"status\" : \"PASS\"\n"
    "\t}\n"
    "}";
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithOneSkippedTest) {
    __clove_suite_t suite = create_suite("Suite1", 0);
    __clove_test_t test11 = create_test_skip("Test11");
    suite_add_test(&suite, &test11);
    
    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 1, 1);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test11, 1);
    base->end_suite(base, &suite, 0);
    base->end(base, 1, 0, 1, 0);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected =
    "{\n"
    "\t\"clove_version\" : \""__CLOVE_VERSION"\",\n"
    "\t\"json_schema\" : \"1.0\",\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 1,\n"
    "\t\t\"test_count\" : 1,\n"
    "\t\t\"suites\" : {\n"
    "\t\t\t\"Suite1\" : {\n"
    "\t\t\t\t\"file\" : \"test_file.c\",\n"
    "\t\t\t\t\"tests\" : {\n"
    "\t\t\t\t\t\"Test11\" : {\n"
    "\t\t\t\t\t\t\"status\" : \"SKIP\",\n"
    "\t\t\t\t\t\t\"duration\" : 50,\n"
    "\t\t\t\t\t\t\"line\" : 4\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t}\n"
    "\t\t\t}\n"
    "\t\t},\n"
    "\t\t\"test_passed\" : 0,\n"
    "\t\t\"test_skipped\" : 1,\n"
    "\t\t\"test_failed\" : 0,\n"
    "\t\t\"status\" : \"SKIP\"\n"
    "\t}\n"
    "}";
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithTwoTests) {
    __clove_suite_t suite = create_suite("Suite1", 0);
    __clove_test_t test11 = create_test("Test11");
    __clove_test_t test12 = create_test_fail("Test12");
    suite_add_test(&suite, &test11);
    suite_add_test(&suite, &test12);

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
    "{\n"
    "\t\"clove_version\" : \""__CLOVE_VERSION"\",\n"
    "\t\"json_schema\" : \"1.0\",\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 1,\n"
    "\t\t\"test_count\" : 2,\n"
    "\t\t\"suites\" : {\n"
    "\t\t\t\"Suite1\" : {\n"
    "\t\t\t\t\"file\" : \"test_file.c\",\n"
    "\t\t\t\t\"tests\" : {\n"
    "\t\t\t\t\t\"Test11\" : {\n"
    "\t\t\t\t\t\t\"status\" : \"PASS\",\n"
    "\t\t\t\t\t\t\"duration\" : 100\n"
    "\t\t\t\t\t},\n"
    "\t\t\t\t\t\"Test12\" : {\n"
    "\t\t\t\t\t\t\"status\" : \"FAIL\",\n"
    "\t\t\t\t\t\t\"duration\" : 100,\n"
    "\t\t\t\t\t\t\"line\" : 8,\n"
    "\t\t\t\t\t\t\"assert\" : \"EQ\",\n"
    "\t\t\t\t\t\t\"type\" : \"BOOL\",\n"
    "\t\t\t\t\t\t\"expected\" : \"false\",\n"
    "\t\t\t\t\t\t\"actual\" : \"true\"\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t}\n"
    "\t\t\t}\n"
    "\t\t},\n"
    "\t\t\"test_passed\" : 1,\n"
    "\t\t\"test_skipped\" : 0,\n"
    "\t\t\"test_failed\" : 1,\n"
    "\t\t\"status\" : \"FAIL\"\n"
    "\t}\n"
    "}";
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportTwoSuitesWithOnePassedTestEach) {
    __clove_suite_t suite = create_suite("Suite1", 0);
    __clove_test_t test11 = create_test("Test11");
    suite_add_test(&suite, &test11);
    
    __clove_suite_t suite2 = create_suite("Suite2", 0);
    __clove_test_t test21 = create_test("Test21");
    suite_add_test(&suite2, &test21);
    
   
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
    "{\n"
    "\t\"clove_version\" : \""__CLOVE_VERSION"\",\n"
    "\t\"json_schema\" : \"1.0\",\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 2,\n"
    "\t\t\"test_count\" : 2,\n"
    "\t\t\"suites\" : {\n"
    "\t\t\t\"Suite1\" : {\n"
    "\t\t\t\t\"file\" : \"test_file.c\",\n"
    "\t\t\t\t\"tests\" : {\n"
    "\t\t\t\t\t\"Test11\" : {\n"
    "\t\t\t\t\t\t\"status\" : \"PASS\",\n"
    "\t\t\t\t\t\t\"duration\" : 100\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t}\n"
    "\t\t\t},\n"
    "\t\t\t\"Suite2\" : {\n"
    "\t\t\t\t\"file\" : \"test_file.c\",\n"
    "\t\t\t\t\"tests\" : {\n"
    "\t\t\t\t\t\"Test21\" : {\n"
    "\t\t\t\t\t\t\"status\" : \"PASS\",\n"
    "\t\t\t\t\t\t\"duration\" : 100\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t}\n"
    "\t\t\t}\n"
    "\t\t},\n"
    "\t\t\"test_passed\" : 2,\n"
    "\t\t\"test_skipped\" : 0,\n"
    "\t\t\"test_failed\" : 0,\n"
    "\t\t\"status\" : \"PASS\"\n"
    "\t}\n"
    "}";
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithOneFailAssertTest) {
    __clove_suite_t suite = create_suite("Suite1", 0);
    
    __clove_test_t test11;
    test11.name = "Test11";
    test11.file_name = "test_file.c";
    test11.result = __CLOVE_TEST_RESULT_FAILED;
    test11.issue.line = 8;
    test11.issue.assert = __CLOVE_ASSERT_FAIL;
    test11.duration.seconds = 0;
    test11.duration.nanos_after_seconds = 100;
    
    suite_add_test(&suite, &test11);

    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 1, 1);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test11, 1);
    base->end_suite(base, &suite, 0);
    base->end(base, 1, 0, 0, 1);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected =
    "{\n"
    "\t\"clove_version\" : \""__CLOVE_VERSION"\",\n"
    "\t\"json_schema\" : \"1.0\",\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 1,\n"
    "\t\t\"test_count\" : 1,\n"
    "\t\t\"suites\" : {\n"
    "\t\t\t\"Suite1\" : {\n"
    "\t\t\t\t\"file\" : \"test_file.c\",\n"
    "\t\t\t\t\"tests\" : {\n"
    "\t\t\t\t\t\"Test11\" : {\n"
    "\t\t\t\t\t\t\"status\" : \"FAIL\",\n"
    "\t\t\t\t\t\t\"duration\" : 100,\n"
    "\t\t\t\t\t\t\"line\" : 8,\n"
    "\t\t\t\t\t\t\"assert\" : \"FAIL\"\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t}\n"
    "\t\t\t}\n"
    "\t\t},\n"
    "\t\t\"test_passed\" : 0,\n"
    "\t\t\"test_skipped\" : 0,\n"
    "\t\t\"test_failed\" : 1,\n"
    "\t\t\"status\" : \"FAIL\"\n"
    "\t}\n"
    "}";
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithOneFailSizetAssertTest) {
    __clove_suite_t suite = create_suite("Suite1", 0);
    
    __clove_test_t test11;
    test11.name = "Test11";
    test11.file_name = "test_file.c";
    test11.result = __CLOVE_TEST_RESULT_FAILED;
    test11.issue.line = 8;
    test11.issue.assert = __CLOVE_ASSERT_EQ;
    test11.issue.data_type = __CLOVE_GENERIC_SIZET;
    test11.issue.expected._sizet = 1;
    test11.issue.actual._sizet = 2;
    test11.duration.seconds = 0;
    test11.duration.nanos_after_seconds = 100;

    suite_add_test(&suite, &test11);
   
    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 1, 1);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test11, 1);
    base->end_suite(base, &suite, 0);
    base->end(base, 1, 0, 0, 1);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected =
    "{\n"
    "\t\"clove_version\" : \""__CLOVE_VERSION"\",\n"
    "\t\"json_schema\" : \"1.0\",\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 1,\n"
    "\t\t\"test_count\" : 1,\n"
    "\t\t\"suites\" : {\n"
    "\t\t\t\"Suite1\" : {\n"
    "\t\t\t\t\"file\" : \"test_file.c\",\n"
    "\t\t\t\t\"tests\" : {\n"
    "\t\t\t\t\t\"Test11\" : {\n"
    "\t\t\t\t\t\t\"status\" : \"FAIL\",\n"
    "\t\t\t\t\t\t\"duration\" : 100,\n"
    "\t\t\t\t\t\t\"line\" : 8,\n"
    "\t\t\t\t\t\t\"assert\" : \"EQ\",\n"
    "\t\t\t\t\t\t\"type\" : \"SIZET\",\n"
    "\t\t\t\t\t\t\"expected\" : \"1\",\n"
    "\t\t\t\t\t\t\"actual\" : \"2\"\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t}\n"
    "\t\t\t}\n"
    "\t\t},\n"
    "\t\t\"test_passed\" : 0,\n"
    "\t\t\"test_skipped\" : 0,\n"
    "\t\t\"test_failed\" : 1,\n"
    "\t\t\"status\" : \"FAIL\"\n"
    "\t}\n"
    "}";
    CLOVE_STRING_EQ(expected, actual);
}