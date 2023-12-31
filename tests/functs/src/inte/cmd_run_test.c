#define CLOVE_SUITE_NAME INTE_RunTest
#include <clove-unit.h>
#include "utils/utils.h"

#define _SEP_ __CLOVE_PATH_SEPARATOR_STR
#define _TBP_ RES_PRJ01_TEST_PATH _SEP_
#define _TBPU_ RES_PRJ01_TEST_PATH_UNIX "/"

static char* cmd_out;

CLOVE_SUITE_SETUP() {
    cmd_out = NULL;
}

CLOVE_SUITE_TEARDOWN() {
    if (cmd_out) free(cmd_out);
}
 
CLOVE_TEST(JsonReportWithOptOonFile) {
    char* base_path = __clove_path_basepath(RES_PRJ01_EXEC_PATH);
    char* report_path = __clove_path_concat(__CLOVE_PATH_SEPARATOR, base_path, "cmd_json_report.json");
    //file_delete(report_path);

    const char* cmd = cmd_fmt("\"%s\" -r json -o \"%s\"", RES_PRJ01_EXEC_PATH, report_path);
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); 

    CLOVE_IS_TRUE(file_exists(report_path));

    file_delete(report_path);
    free(base_path);
    free(report_path);
}

CLOVE_TEST(JsonReportWithOptOuputOnFile) {
    char* base_path = __clove_path_basepath(RES_PRJ01_EXEC_PATH);
    char* report_path = __clove_path_concat(__CLOVE_PATH_SEPARATOR, base_path, "cmd_json_report.json");
    //file_delete(report_path);

    const char* cmd = cmd_fmt("\"%s\" -r json --output \"%s\"", RES_PRJ01_EXEC_PATH, report_path);
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); 

    CLOVE_IS_TRUE(file_exists(report_path));

    file_delete(report_path);
    free(base_path);
    free(report_path);
}

CLOVE_TEST(JsonReportWithOptOuputOnStdoutAsDefault) {
    const char* cmd = cmd_fmt("\"%s\" -r json", RES_PRJ01_EXEC_PATH);
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); 
    
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"suite_count\" : 2"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"test_count\" : 3"));
}

CLOVE_TEST(JsonReportWithOptOuputOnStdout) {
    const char* cmd = cmd_fmt("\"%s\" -r json --output stdout", RES_PRJ01_EXEC_PATH);
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); 
    
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"suite_count\" : 2"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"test_count\" : 3"));
}

CLOVE_TEST(JsonReportWithOptReportOnStdoutDefault) {
    const char* cmd = cmd_fmt("\"%s\" --report json", RES_PRJ01_EXEC_PATH);
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); 
    
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"suite_count\" : 2"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"test_count\" : 3"));
}

CLOVE_TEST(JsonReportWithBasePathOpt) {
    const char* cmd;
    int cmd_code; 
    
    //Without Base Path options
    cmd = RES_PRJ01_EXEC_PATH" -r json";
    cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); 
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"suite_count\" : 2"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"file\" : \""_TBPU_"src/prj01_test1.c\""));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"file\" : \""_TBPU_"src/prj01_test2.c\""));

    //With base path option: -b
    cmd = RES_PRJ01_EXEC_PATH" -r json -b "_TBP_;
    cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); 
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"suite_count\" : 2"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"file\" : \"src/prj01_test1.c\""));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"file\" : \"src/prj01_test2.c\""));

    //With base path option: --base-path
    cmd = RES_PRJ01_EXEC_PATH" -r json --base-path "_TBP_;
    cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); 
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"suite_count\" : 2"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"file\" : \"src/prj01_test1.c\""));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "\"file\" : \"src/prj01_test2.c\""));
}

CLOVE_TEST(PrettyReportWithOptXTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -r pretty -x"; //x enable exec error in case of test failure
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_GENERIC, cmd_code); //project contains 1 test with failure
}

CLOVE_TEST(PrettyReportWithOptErrorOnTestFailTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -r pretty --error-on-test-fail"; //x enable exec error in case of test failure
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_GENERIC, cmd_code); //project contains 1 test with failure
}

CLOVE_TEST(PrettyReportIncludeOneTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -r pretty -i Prj01Suite01.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 1 / 1"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "1) Prj01Suite01.Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 1, Passed: 1, Failed: 0, Skipped: 0"));
}

CLOVE_TEST(PrettyReportIncludeTwoTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -r pretty -i Prj01Suite01.Test01 -i Prj01Suite02.Test21";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 2 / 2"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "1) Prj01Suite01.Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "2) Prj01Suite02.Test21"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 2, Passed: 2, Failed: 0, Skipped: 0"));
}

CLOVE_TEST(PrettyReportWithOptReport) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --report pretty";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
}

CLOVE_TEST(PrettyReportWithBasePathOpt) {
    const char* cmd;
    int cmd_code;
    
    //Without basepath option
    cmd = RES_PRJ01_EXEC_PATH" -r pretty"; 
    cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); //project contains 1 test with failure
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "[FAIL] "_TBP_"src"_SEP_"prj01_test1.c:9"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 3, Passed: 2, Failed: 1, Skipped: 0"));

    //With basepath otion -b
    cmd = RES_PRJ01_EXEC_PATH" -r pretty -b "_TBP_; 
    cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); //project contains 1 test with failure
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "[FAIL] src"_SEP_"prj01_test1.c:9"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 3, Passed: 2, Failed: 1, Skipped: 0"));

    //With basepath otion --base-path
    cmd = RES_PRJ01_EXEC_PATH" -r pretty --base-path "_TBP_; 
    cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); //project contains 1 test with failure
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "[FAIL] src"_SEP_"prj01_test1.c:9"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 3, Passed: 2, Failed: 1, Skipped: 0"));
}

CLOVE_TEST(DefaultReportIncludeOneTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -i Prj01Suite01.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 1 / 1"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "1) Prj01Suite01.Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 1, Passed: 1, Failed: 0, Skipped: 0"));
}

CLOVE_TEST(DefaultReportExcludeOneTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -e Prj01Suite01.Test02";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 2 / 2"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "1) Prj01Suite01.Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "2) Prj01Suite02.Test21"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 2, Passed: 2, Failed: 0, Skipped: 0"));
}

CLOVE_TEST(DefaultReportIncludeOverExcludeOneTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -i Prj01Suite01.Test01 -e Prj01Suite01.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 1 / 1"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "1) Prj01Suite01.Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 1, Passed: 1, Failed: 0, Skipped: 0"));
}

CLOVE_TEST(DefaultReportWithOptOonFile) {
    char* base_path = __clove_path_basepath(RES_PRJ01_EXEC_PATH);
    char* report_path = __clove_path_concat(__CLOVE_PATH_SEPARATOR, base_path, "cmd_console_report.txt");
    //file_delete(report_path);

    const char* cmd = cmd_fmt("\"%s\" -o \"%s\"", RES_PRJ01_EXEC_PATH, report_path);
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code); 

    CLOVE_IS_TRUE(file_exists(report_path));

    file_delete(report_path);
    free(base_path);
    free(report_path);
}

CLOVE_TEST(DefaultReportWithOptT) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -t";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 2 / 3"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 3, Passed: 2, Failed: 1, Skipped: 0"));
}

CLOVE_TEST(DefaultReportWithOptRunTests) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --run-tests";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Suite / Tests found: 2 / 3"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Total: 3, Passed: 2, Failed: 1, Skipped: 0"));
}

CLOVE_TEST(ReportWithOptRcsvWithFailure) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -r csv";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);

    __clove_vector_t lines;
    str_split(cmd_out, '\n', &lines);

    CLOVE_ULLONG_EQ(4, __clove_vector_count(&lines));

    const char* line1 = *(char**)__clove_vector_get(&lines, 0);
    const char* line2 = *(char**)__clove_vector_get(&lines, 1);
    const char* line3 = *(char**)__clove_vector_get(&lines, 2);
    const char* line4 = *(char**)__clove_vector_get(&lines, 3);

    //line1
    CLOVE_STRING_EQ("Suite,Test,Status,Duration,File,Line,Assert,Type,Expected,Actual", line1);
    //line2
    CLOVE_IS_TRUE(__clove_string_startswith(line2, "Prj01Suite01,Test01,PASS,"));
    //line3
    CLOVE_STRING_EQ("Prj01Suite01,Test02,FAIL,,"_TBP_"src"_SEP_"prj01_test1.c,9,FAIL,,,", line3);
    //line4
    CLOVE_IS_TRUE(__clove_string_startswith(line4, "Prj01Suite02,Test21,PASS,"));
}

CLOVE_TEST(CsvReportWithBasepathOpt) {
    const char* cmd;
    int cmd_code;
    __clove_vector_t lines;
    const char* line3;

    //Without basepath option
    cmd = RES_PRJ01_EXEC_PATH" -r csv";
    cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    str_split(cmd_out, '\n', &lines);
    CLOVE_ULLONG_EQ(4, __clove_vector_count(&lines));
    line3 = *(char**)__clove_vector_get(&lines, 2);
    CLOVE_STRING_EQ("Prj01Suite01,Test02,FAIL,,"_TBP_"src"_SEP_"prj01_test1.c,9,FAIL,,,", line3);

    //With basepath option -b
    cmd = RES_PRJ01_EXEC_PATH" -r csv -b "_TBP_;
    cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    str_split(cmd_out, '\n', &lines);
    CLOVE_ULLONG_EQ(4, __clove_vector_count(&lines));
    line3 = *(char**)__clove_vector_get(&lines, 2);
    CLOVE_STRING_EQ("Prj01Suite01,Test02,FAIL,,src"_SEP_"prj01_test1.c,9,FAIL,,,", line3);

    //With basepath option --base-path
    cmd = RES_PRJ01_EXEC_PATH" -r csv --base-path "_TBP_;
    cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    str_split(cmd_out, '\n', &lines);
    CLOVE_ULLONG_EQ(4, __clove_vector_count(&lines));
    line3 = *(char**)__clove_vector_get(&lines, 2);
    CLOVE_STRING_EQ("Prj01Suite01,Test02,FAIL,,src"_SEP_"prj01_test1.c,9,FAIL,,,", line3);
}