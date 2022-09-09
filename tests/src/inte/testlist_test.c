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
  <exec> --list-test [-i inclusion][-e exclusion]

  Return as output list of test with this format:
  <SuiteName> <TestName> <TestFuncLineNumber>
*/

CLOVE_TEST(ListAllTestsTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    
    const char* out_expected = "Prj01Suite01 Test01 4\n"
                               "Prj01Suite01 Test02 8\n"
                               "Prj01Suite02 Test21 4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithInclusionTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -i *.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = "Prj01Suite01 Test01 4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}

CLOVE_TEST(ListTestWithExclusionTest) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --list-tests -e *.Test01";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    
    const char* out_expected = "Prj01Suite01 Test02 8\n"
                               "Prj01Suite02 Test21 4\n";
    CLOVE_STRING_EQ(out_expected, cmd_out);
}
