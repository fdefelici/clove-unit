#define CLOVE_SUITE_NAME UNIT_ReportJsonTest
#include "clove-unit.h"
#include <stdlib.h>

char* read_file(const char* path) {
    FILE* file = __clove_file_open(path, "rb");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* result = (char*)malloc(file_size + 1);
    result[file_size] = '\0';
    size_t bytes_read = fread(result, file_size, 1, file);
    fclose(file);
    return result;
}

__clove_report_json_t* report;
__clove_stream_file_t* stream;

CLOVE_SUITE_SETUP() {
    char* file_path = __clove_path_rel_to_abs_exec_path("clove_report.json");
    stream = __clove_stream_file_new(file_path);
    report = __clove_report_json_new((__clove_stream_t*)stream, "test_version");
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
    "\t\"clove_version\" : \"test_version\",\n"
    "\t\"api_version\" : 1,\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 0,\n"
    "\t\t\"test_count\" : 0,\n"
    "\t\t\"suites\" : {\n"
    "\t\t},\n"
    "\t\t\"test_passed\" : 0,\n"
    "\t\t\"test_skipped\" : 0,\n"
    "\t\t\"test_failed\" : 0,\n"
    "\t\t\"status\" : 1\n"
    "\t}\n"
    "}";

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
    base->test_executed(base, &suite, &test11, 1);
    base->end(base, 1, 1, 0, 0);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected =
    "{\n"
    "\t\"clove_version\" : \"test_version\",\n"
    "\t\"api_version\" : 1,\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 1,\n"
    "\t\t\"test_count\" : 1,\n"
    "\t\t\"suites\" : {\n"
    "\t\t\t\"Suite1\" : {\n"
    "\t\t\t\t\"_data\" : {\n"
    "\t\t\t\t\t\"file\" : \"test-file.c\"\n"
    "\t\t\t\t},\n"
    "\t\t\t\t\"Test11\" : {\n"
    "\t\t\t\t\t\"status\" : 1,\n"
    "\t\t\t\t\t\"duration\" : 100\n"
    "\t\t\t\t}\n"
    "\t\t\t}\n"
    "\t\t},\n"
    "\t\t\"test_passed\" : 1,\n"
    "\t\t\"test_skipped\" : 0,\n"
    "\t\t\"test_failed\" : 0,\n"
    "\t\t\"status\" : 1\n"
    "\t}\n"
    "}";

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
    base->start(base, 2, 6);
    base->test_executed(base, &suite, &test11, 1);
    base->test_executed(base, &suite, &test12, 2);
    base->end(base, 6, 3, 2, 1);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected = 
    "{\n"
    "\t\"clove_version\" : \"test_version\",\n"
    "\t\"api_version\" : 1,\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 2,\n"
    "\t\t\"test_count\" : 6,\n"
    "\t\t\"suites\" : {\n"
    "\t\t\t\"Suite1\" : {\n"
    "\t\t\t\t\"_data\" : {\n"
    "\t\t\t\t\t\"file\" : \"test-file.c\"\n"
    "\t\t\t\t},\n"
    "\t\t\t\t\"Test11\" : {\n"
    "\t\t\t\t\t\"status\" : 1,\n"
    "\t\t\t\t\t\"duration\" : 100\n"
    "\t\t\t\t},\n"
    "\t\t\t\t\"Test12\" : {\n"
    "\t\t\t\t\t\"status\" : 2,\n"
    "\t\t\t\t\t\"duration\" : 100,\n"
    "\t\t\t\t\t\"line\" : 8,\n"
    "\t\t\t\t\t\"assert\" : 1,\n"
    "\t\t\t\t\t\"type\" : 1,\n"
    "\t\t\t\t\t\"expected\" : \"false\",\n"
    "\t\t\t\t\t\"actual\" : \"true\"\n"
    "\t\t\t\t}\n"
    "\t\t\t}\n"
    "\t\t},\n"
    "\t\t\"test_passed\" : 3,\n"
    "\t\t\"test_skipped\" : 2,\n"
    "\t\t\"test_failed\" : 1,\n"
    "\t\t\"status\" : 2\n"
    "\t}\n"
    "}";
    
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
    base->test_executed(base, &suite, &test11, 1);
    base->test_executed(base, &suite2, &test21, 2);
    base->end(base, 2, 2, 0, 0);

    const char* file_path = stream->file_path;
    const char* actual = read_file(file_path);

    const char* expected =
    "{\n"
    "\t\"clove_version\" : \"test_version\",\n"
    "\t\"api_version\" : 1,\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 2,\n"
    "\t\t\"test_count\" : 2,\n"
    "\t\t\"suites\" : {\n"
    "\t\t\t\"Suite1\" : {\n"
    "\t\t\t\t\"_data\" : {\n"
    "\t\t\t\t\t\"file\" : \"test-file.c\"\n"
    "\t\t\t\t},\n"
    "\t\t\t\t\"Test11\" : {\n"
    "\t\t\t\t\t\"status\" : 1,\n"
    "\t\t\t\t\t\"duration\" : 100\n"
    "\t\t\t\t}\n"
    "\t\t\t},\n"
    "\t\t\t\"Suite2\" : {\n"
    "\t\t\t\t\"_data\" : {\n"
    "\t\t\t\t\t\"file\" : \"test-file2.c\"\n"
    "\t\t\t\t},\n"
    "\t\t\t\t\"Test21\" : {\n"
    "\t\t\t\t\t\"status\" : 1,\n"
    "\t\t\t\t\t\"duration\" : 100\n"
    "\t\t\t\t}\n"
    "\t\t\t}\n"
    "\t\t},\n"
    "\t\t\"test_passed\" : 2,\n"
    "\t\t\"test_skipped\" : 0,\n"
    "\t\t\"test_failed\" : 0,\n"
    "\t\t\"status\" : 1\n"
    "\t}\n"
    "}";

    CLOVE_STRING_EQ(expected, actual);
}
