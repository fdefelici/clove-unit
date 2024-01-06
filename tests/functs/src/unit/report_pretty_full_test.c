#define CLOVE_SUITE_NAME UNIT_ReportPrettyTest
#include "clove-unit.h"
#include "utils/domain_utils.h"

static __clove_report_pretty_t* report;
static __clove_stream_memory_t* stream;
static __clove_report_params_t params;

CLOVE_SUITE_SETUP() {
    stream = __clove_stream_memory_new();
    params.tests_base_path = "abs";
    params.report_detail = __CLOVE_REPORT_DETAIL__PASSED_FAILED_SKIPPED;
    report = __clove_report_run_tests_pretty_new((__clove_stream_t*)stream, &params);
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
    "[INFO] Executing Test Runner with detail level: 'Full'\n"
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
    "[INFO] Executing Test Runner with detail level: 'Full'\n"
    "[INFO] Suites / Tests found: 1 / 1\n"
    "[INFO] 1) Suite1.Test11............................................[PASS] (0.000 ms)\n"
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

    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 1, 2);
    base->begin_suite(base, &suite, 0);
    base->end_test(base, &suite, &test11, 1);
    base->end_test(base, &suite, &test12, 2);
    base->end_suite(base, &suite, 0);
    base->end(base, 2, 1, 0, 1);

    const char* expected = 
    "[INFO] Executing Test Runner with detail level: 'Full'\n"
    "[INFO] Suites / Tests found: 1 / 2\n"
    "[INFO] 1) Suite1.Test11............................................[PASS] (0.000 ms)\n"
    "[ERRO] 2) Suite1.Test12............................................[FAIL] test_file.c:8: expected [false] but was [true]\n"
    "[INFO] Total: 2, Passed: 1, Failed: 1, Skipped: 0\n"
    "[INFO] Run duration: 0 ms\n"
    "[ERRO] Run result: FAILURE :_(\n"
    ;

    char* actual = __clove_stream_memory_as_string(stream);

    CLOVE_STRING_EQ(expected, actual);
}

CLOVE_TEST(ReportTwoSuitesWithTenTests) {
    __clove_suite_t suite1 = create_suite("Suite 1", 1);
    __clove_test_t test11 = create_test("Test11");

    __clove_suite_t suite2 = create_suite("Suite 2", 9);    
    __clove_test_t test21 = create_test("Test21");
    __clove_test_t test22 = create_test("Test22");
    __clove_test_t test23 = create_test("Test23");
    __clove_test_t test24 = create_test("Test24");
    __clove_test_t test25 = create_test("Test25");
    __clove_test_t test26 = create_test("Test26");
    __clove_test_t test27 = create_test("Test27");
    __clove_test_t test28 = create_test("Test28");
    __clove_test_t test29 = create_test("Test29");

    __clove_report_t* base = (__clove_report_t*)report;
    base->start(base, 2, 10);
    base->begin_suite(base, &suite1, 0);
    base->end_test(base, &suite1, &test11, 1);
    base->end_suite(base, &suite1, 0);
    base->begin_suite(base, &suite2, 1);
    base->end_test(base, &suite2, &test21, 2);
    base->end_test(base, &suite2, &test22, 3);
    base->end_test(base, &suite2, &test23, 4);
    base->end_test(base, &suite2, &test24, 5);
    base->end_test(base, &suite2, &test25, 6);
    base->end_test(base, &suite2, &test26, 7);
    base->end_test(base, &suite2, &test27, 8);
    base->end_test(base, &suite2, &test28, 9);
    base->end_test(base, &suite2, &test29, 10);
    base->end_suite(base, &suite2, 1);
    base->end(base, 2, 10, 0, 0);

    const char* expected = 
    "[INFO] Executing Test Runner with detail level: 'Full'\n"
    "[INFO] Suites / Tests found: 2 / 10\n"
    "[INFO] 01) Suite 1.Test11..........................................[PASS] (0.000 ms)\n"
    "[INFO] 02) Suite 2.Test21..........................................[PASS] (0.000 ms)\n"
    "[INFO] 03) Suite 2.Test22..........................................[PASS] (0.000 ms)\n"
    "[INFO] 04) Suite 2.Test23..........................................[PASS] (0.000 ms)\n"
    "[INFO] 05) Suite 2.Test24..........................................[PASS] (0.000 ms)\n"
    "[INFO] 06) Suite 2.Test25..........................................[PASS] (0.000 ms)\n"
    "[INFO] 07) Suite 2.Test26..........................................[PASS] (0.000 ms)\n"
    "[INFO] 08) Suite 2.Test27..........................................[PASS] (0.000 ms)\n"
    "[INFO] 09) Suite 2.Test28..........................................[PASS] (0.000 ms)\n"
    "[INFO] 10) Suite 2.Test29..........................................[PASS] (0.000 ms)\n"
    "[INFO] Total: 2, Passed: 10, Failed: 0, Skipped: 0\n"
    "[INFO] Run duration: 0 ms\n"
    ;

    char* actual = __clove_stream_memory_as_string(stream);

    CLOVE_STRING_EQ(expected, actual);
}
