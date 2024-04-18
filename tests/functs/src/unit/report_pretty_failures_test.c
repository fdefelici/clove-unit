#define CLOVE_SUITE_NAME UNIT_ReportPrettyFailuresModeTest
#include "clove-unit.h"
#include "utils/domain_utils.h"

static __clove_report_pretty_t* report;
static __clove_stream_memory_t* stream;
static __clove_report_params_t params;
static __clove_vector_t suites;

CLOVE_SUITE_SETUP() {
    stream = __clove_stream_memory_new();
    params.tests_base_path = "abs";
    params.report_detail = __CLOVE_REPORT_DETAIL__FAILED;
    report = __clove_report_run_tests_pretty_new((__clove_stream_t*)stream, &params);
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
    base->end(base, 0, 0, 0);

    const char* expected = 
    "[INFO] Executing Test Runner with detail level: 'Failed only'\n"
    "[INFO] Suites / Tests found: 0 / 0\n"
    "[INFO] Total: 0, Passed: 0, Failed: 0, Skipped: 0\n"
    "[INFO] Run duration: 0 ms\n"
    "[INFO] Run result: SUCCESS :-)\n"
    ;

    char* actual = __clove_stream_memory_as_string(stream);
    
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
    base->end(base, 1, 0, 0);

    const char* expected = 
    "[INFO] Executing Test Runner with detail level: 'Failed only'\n"
    "[INFO] Suites / Tests found: 1 / 1\n"
    "[INFO] Total: 1, Passed: 1, Failed: 0, Skipped: 0\n"
    "[INFO] Run duration: 0 ms\n"
    "[INFO] Run result: SUCCESS :-)\n"
    ;

    char* actual = __clove_stream_memory_as_string(stream);

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
    base->end(base, 1, 0, 1);

    const char* expected = 
    "[INFO] Executing Test Runner with detail level: 'Failed only'\n"
    "[INFO] Suites / Tests found: 1 / 2\n"
    "[ERRO] 2) Suite1.Test12.........................................[FAIL] test_file.c:8: expected [false] but was [true]\n"
    "[INFO] Total: 2, Passed: 1, Failed: 1, Skipped: 0\n"
    "[INFO] Run duration: 0 ms\n"
    "[ERRO] Run result: FAILURE :_(\n"
    ;

    char* actual = __clove_stream_memory_as_string(stream);

    CLOVE_STRING_EQ(expected, actual);
}
