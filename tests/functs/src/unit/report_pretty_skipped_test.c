#define CLOVE_SUITE_NAME UNIT_ReportPrettyDetailSkippedTest
#include "clove-unit.h"
#include "utils/domain_utils.h"

static __clove_report_pretty_t* report;
static __clove_stream_memory_t* stream;
static __clove_report_params_t params;

CLOVE_SUITE_SETUP() {
    stream = __clove_stream_memory_new();
    params.tests_base_path = "abs";
    params.report_detail = __CLOVE_REPORT_DETAIL__FAILED_SKIPPED;
    report = __clove_report_pretty_new((__clove_stream_t*)stream, &params);
}

CLOVE_SUITE_TEARDOWN() {
    report->base.free((__clove_report_t*)report);
    stream->base.free((__clove_stream_t*)stream);
}

CLOVE_TEST(EmptyReport) {
    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 0, 0);
    base->end(base, 0, 0, 0, 0);

    const char* expected = 
    "[INFO] Executing Test Runner with detail level: 'Failed + Skipped'\n"
    "[INFO] Suites / Tests found: 0 / 0\n"
    "[INFO] Total: 0, Passed: 0, Failed: 0, Skipped: 0\n"
    "[INFO] Run duration: 0 ms\n"
    "[INFO] Run result: SUCCESS :-)\n"
    ;

    char* actual = __clove_stream_memory_as_string(stream);
    
    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithOnePassedTest) {
    __clove_suite_t suite = create_suite("Suite1", 1); 
    __clove_test_t test11 = create_test("Test11");
   
    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 1, 1);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test11, 1);
    base->end_suite(base, &suite, 0);
    base->end(base, 1, 1, 0, 0);

    const char* expected = 
    "[INFO] Executing Test Runner with detail level: 'Failed + Skipped'\n"
    "[INFO] Suites / Tests found: 1 / 1\n"
    "[INFO] Total: 1, Passed: 1, Failed: 0, Skipped: 0\n"
    "[INFO] Run duration: 0 ms\n"
    "[INFO] Run result: SUCCESS :-)\n"
    ;

    char* actual = __clove_stream_memory_as_string(stream);

    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportOneSuiteWithTwoTests) {
    __clove_suite_t suite = create_suite("Suite1", 2);
    __clove_test_t test11 = create_test("Test11");
    __clove_test_t test12 = create_test_fail("Test12");
    __clove_test_t test13 = create_test_skip("Test13");

    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 1, 3);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test11, 1);
    base->end_test(base, &suite, &test12, 2);
    base->end_test(base, &suite, &test13, 3);
    base->end_suite(base, &suite, 0);
    base->end(base, 3, 1, 1, 1);


    const char* expected = 
    "[INFO] Executing Test Runner with detail level: 'Failed + Skipped'\n"
    "[INFO] Suites / Tests found: 1 / 3\n"
    "[ERRO] 2) Suite1.Test12............................................[FAIL] test_file.c:8: expected [false] but was [true]\n"
    "[WARN] 3) Suite1.Test13............................................[SKIP]\n"
    "[INFO] Total: 3, Passed: 1, Failed: 1, Skipped: 1\n"
    "[INFO] Run duration: 0 ms\n"
    "[ERRO] Run result: FAILURE :_(\n"
    ;

    char* actual = __clove_stream_memory_as_string(stream);

    CLOVE_STRING_EQ(expected, actual);
}
