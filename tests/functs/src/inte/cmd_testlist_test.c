#define CLOVE_SUITE_NAME INTE_CmdTestListTest
#include <clove-unit.h>
#include "utils/utils.h"

static char* cmd_out;

CLOVE_SUITE_SETUP() {
    cmd_out = NULL;
}

CLOVE_SUITE_TEARDOWN() {
    if (cmd_out) free(cmd_out);
}

/*
  List Tests command line api is invoked as follow:
  <exec> --list-test [-i,--include expr][-e,--exclude expr][-b, --base-path path]

  Return as output list of test with this format:
  <SuiteName> <TestName> <SuiteFilePath> <TestFuncLineNumber>
*/
#define _SEP_ __CLOVE_PATH_SEPARATOR_STR
#define _TBP_ RES_PRJ01_TEST_PATH _SEP_
#define _TBPU_ RES_PRJ01_TEST_PATH_UNIX "/"

CLOVE_TEST(ListAllTestsWithOptLOptRcsv) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l -r csv";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
        
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test01,"_TBP_"src"_SEP_"prj01_test1.c,4\n"
    "Prj01Suite01,Test02,"_TBP_"src"_SEP_"prj01_test1.c,8\n"
    "Prj01Suite02,Test21,"_TBP_"src"_SEP_"prj01_test2.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListAllTestsWithOptListTestsOptRcsv) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -r csv";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test01,"_TBP_"src"_SEP_"prj01_test1.c,4\n"
    "Prj01Suite01,Test02,"_TBP_"src"_SEP_"prj01_test1.c,8\n"
    "Prj01Suite02,Test21,"_TBP_"src"_SEP_"prj01_test2.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithInclusionOptIOptRcsvTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -r csv -i *.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test01,"_TBP_"src"_SEP_"prj01_test1.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithInclusionOptIncludeOptRcsvTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -r csv --include *.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test01,"_TBP_"src"_SEP_"prj01_test1.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithExclusionOptEOptRcsvTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -r csv -e *.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test02,"_TBP_"src"_SEP_"prj01_test1.c,8\n"
    "Prj01Suite02,Test21,"_TBP_"src"_SEP_"prj01_test2.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithExclusionOptExcludeOptRcsvTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -r csv --exclude *.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test02,"_TBP_"src"_SEP_"prj01_test1.c,8\n"
    "Prj01Suite02,Test21,"_TBP_"src"_SEP_"prj01_test2.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListAllTestsWithOptLOptRcsvOptB) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l -r csv -b "_TBP_;
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
        
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test01,src"_SEP_"prj01_test1.c,4\n"
    "Prj01Suite01,Test02,src"_SEP_"prj01_test1.c,8\n"
    "Prj01Suite02,Test21,src"_SEP_"prj01_test2.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListAllTestsWithOptLOptRcsvOptBasepath) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l -r csv --base-path "_TBP_;
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
        
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test01,src"_SEP_"prj01_test1.c,4\n"
    "Prj01Suite01,Test02,src"_SEP_"prj01_test1.c,8\n"
    "Prj01Suite02,Test21,src"_SEP_"prj01_test2.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}


CLOVE_TEST(ListTestsWithtDefaultReport) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = "\nListing 3 Test(s) in 2 Suite(s):\n\n"
                               "Prj01Suite01 (file: "_TBP_"src"_SEP_"prj01_test1.c)\n"
                               "- Test01 [line: 4]\n"
                               "- Test02 [line: 8]\n"
                               "Prj01Suite02 (file: "_TBP_"src"_SEP_"prj01_test2.c)\n"
                               "- Test21 [line: 4]\n";
   CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestsWithOptRpretty) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l -r pretty";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = "\nListing 3 Test(s) in 2 Suite(s):\n\n"
                               "Prj01Suite01 (file: "_TBP_"src"_SEP_"prj01_test1.c)\n"
                               "- Test01 [line: 4]\n"
                               "- Test02 [line: 8]\n"
                               "Prj01Suite02 (file: "_TBP_"src"_SEP_"prj01_test2.c)\n"
                               "- Test21 [line: 4]\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestsWithOptRprettyOptB) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l -r pretty -b "_TBP_;
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = "\nListing 3 Test(s) in 2 Suite(s):\n\n"
                               "Prj01Suite01 (file: src"_SEP_"prj01_test1.c)\n"
                               "- Test01 [line: 4]\n"
                               "- Test02 [line: 8]\n"
                               "Prj01Suite02 (file: src"_SEP_"prj01_test2.c)\n"
                               "- Test21 [line: 4]\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithOptRjson) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l -r json";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = 
    "{\n"
    "\t\"clove_version\" : \""__CLOVE_VERSION"\",\n"
    "\t\"json_schema\" : \"1.0\",\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 2,\n"
    "\t\t\"test_count\" : 3,\n"
    "\t\t\"suites\" : [\n"
    "\t\t\t{\n"
    "\t\t\t\t\"name\" : \"Prj01Suite01\",\n"
    "\t\t\t\t\"file\" : \""_TBPU_"src/prj01_test1.c\",\n"
    "\t\t\t\t\"tests\" : [\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test01\",\n"
    "\t\t\t\t\t\t\"line\" : 4\n"
    "\t\t\t\t\t},\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test02\",\n"
    "\t\t\t\t\t\t\"line\" : 8\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t]\n"
    "\t\t\t},\n"
    "\t\t\t{\n"
    "\t\t\t\t\"name\" : \"Prj01Suite02\",\n"
    "\t\t\t\t\"file\" : \""_TBPU_"src/prj01_test2.c\",\n"
    "\t\t\t\t\"tests\" : [\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test21\",\n"
    "\t\t\t\t\t\t\"line\" : 4\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t]\n"
    "\t\t\t}\n"
    "\t\t]\n"
    "\t}\n"
    "}\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithOptRjsonOptB) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l -r json -b "_TBP_;
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = 
    "{\n"
    "\t\"clove_version\" : \""__CLOVE_VERSION"\",\n"
    "\t\"json_schema\" : \"1.0\",\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 2,\n"
    "\t\t\"test_count\" : 3,\n"
    "\t\t\"suites\" : [\n"
    "\t\t\t{\n"
    "\t\t\t\t\"name\" : \"Prj01Suite01\",\n"
    "\t\t\t\t\"file\" : \"src/prj01_test1.c\",\n"
    "\t\t\t\t\"tests\" : [\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test01\",\n"
    "\t\t\t\t\t\t\"line\" : 4\n"
    "\t\t\t\t\t},\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test02\",\n"
    "\t\t\t\t\t\t\"line\" : 8\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t]\n"
    "\t\t\t},\n"
    "\t\t\t{\n"
    "\t\t\t\t\"name\" : \"Prj01Suite02\",\n"
    "\t\t\t\t\"file\" : \"src/prj01_test2.c\",\n"
    "\t\t\t\t\"tests\" : [\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test21\",\n"
    "\t\t\t\t\t\t\"line\" : 4\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t]\n"
    "\t\t\t}\n"
    "\t\t]\n"
    "\t}\n"
    "}\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}