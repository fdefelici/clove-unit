#define CLOVE_SUITE_NAME INTE_CmdTestListTest
#include <clove-unit.h>
#include "utils/utils.h"

char* cmd_out;

CLOVE_SUITE_SETUP() {
    cmd_out = NULL;
}

CLOVE_SUITE_TEARDOWN() {
    if (cmd_out) free(cmd_out);
}

/*
  List Tests command line api is invoked as follow:
  <exec> --list-test [-i,--include expr][-e,--exclude expr]

  Return as output list of test with this format:
  <SuiteName> <TestName> <TestFuncLineNumber>
*/
#define _SEP_ __CLOVE_PATH_SEPARATOR_STR
CLOVE_TEST(ListAllTestsWithOptLOptRcsv) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l -r csv";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test01,src"_SEP_"prj01_test1.c,4\n"
    "Prj01Suite01,Test02,src"_SEP_"prj01_test1.c,8\n"
    "Prj01Suite02,Test21,src"_SEP_"prj01_test2.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListAllTestsWithOptListTestsOptRcsv) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -r csv";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test01,src"_SEP_"prj01_test1.c,4\n"
    "Prj01Suite01,Test02,src"_SEP_"prj01_test1.c,8\n"
    "Prj01Suite02,Test21,src"_SEP_"prj01_test2.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithInclusionOptIOptRcsvTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -r csv -i *.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test01,src"_SEP_"prj01_test1.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithInclusionOptIncludeOptRcsvTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -r csv --include *.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test01,src"_SEP_"prj01_test1.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithExclusionOptEOptRcsvTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -r csv -e *.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test02,src"_SEP_"prj01_test1.c,8\n"
    "Prj01Suite02,Test21,src"_SEP_"prj01_test2.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithExclusionOptExcludeOptRcsvTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -r csv --exclude *.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = 
    "Suite,Test,File,Line\n"
    "Prj01Suite01,Test02,src"_SEP_"prj01_test1.c,8\n"
    "Prj01Suite02,Test21,src"_SEP_"prj01_test2.c,4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}


CLOVE_TEST(ListTestsWithtDefaultReport) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = "\nListing 3 Test(s) in 2 Suite(s):\n\n"
                               "Prj01Suite01 (file: src"_SEP_"prj01_test1.c)\n"
                               "- Test01 [line: 4]\n"
                               "- Test02 [line: 8]\n"
                               "Prj01Suite02 (file: src"_SEP_"prj01_test2.c)\n"
                               "- Test21 [line: 4]\n";
     /* Cannot use it because of Ansi encoding 
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Prj01Suite01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Test02"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Prj01Suite02"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Test21"));
    */
   CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestsWithOptRpretty) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l -r pretty";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = "\nListing 3 Test(s) in 2 Suite(s):\n\n"
                               "Prj01Suite01 (file: src"_SEP_"prj01_test1.c)\n"
                               "- Test01 [line: 4]\n"
                               "- Test02 [line: 8]\n"
                               "Prj01Suite02 (file: src"_SEP_"prj01_test2.c)\n"
                               "- Test21 [line: 4]\n";
    /* Cannot use it because of Ansi encoding 
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Prj01Suite01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Test01"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Test02"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Prj01Suite02"));
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "Test21"));

    */
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithOptRjson) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -l -r json";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = 
    "{\n"
    "\t\"suite_count\" : 2,\n"
    "\t\"test_count\" : 3,\n"
    "\t\"suites\" : [\n"
    "\t\t{\n"
    "\t\t\t\"name\" : \"Prj01Suite01\",\n"
    "\t\t\t\"file\" : \"src/prj01_test1.c\",\n"
    "\t\t\t\"tests\" : [\n"
    "\t\t\t\t{\n"
    "\t\t\t\t\t\"name\" : \"Test01\",\n"
    "\t\t\t\t\t\"line\" : 4\n"
    "\t\t\t\t},\n"
    "\t\t\t\t{\n"
    "\t\t\t\t\t\"name\" : \"Test02\",\n"
    "\t\t\t\t\t\"line\" : 8\n"
    "\t\t\t\t}\n"
    "\t\t\t]\n"
    "\t\t},\n"
     "\t\t{\n"
    "\t\t\t\"name\" : \"Prj01Suite02\",\n"
    "\t\t\t\"file\" : \"src/prj01_test2.c\",\n"
    "\t\t\t\"tests\" : [\n"
    "\t\t\t\t{\n"
    "\t\t\t\t\t\"name\" : \"Test21\",\n"
    "\t\t\t\t\t\"line\" : 4\n"
    "\t\t\t\t}\n"
    "\t\t\t]\n"
    "\t\t}\n"
    "\t]\n"
    "}\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}