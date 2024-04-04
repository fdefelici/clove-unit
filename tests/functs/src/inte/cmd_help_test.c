#define CLOVE_SUITE_NAME INTE_CmdHelpTest
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
  Help command is invoked as follow:
  <exec> -h
  <exec> --help
*/
CLOVE_TEST(RunHelpWithOptH) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -h";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "<executable> [options]\n"));
}

CLOVE_TEST(RunHelpWithOptHelp) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --help";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "<executable> [options]\n"));
}

CLOVE_TEST(RunHelpFullCheck) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --help";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    //CLOVE_IS_TRUE(__clove_string_contains(cmd_out, "<executable> [options]\n"));
    const char* expected = 
    "CLove-Unit v"__CLOVE_VERSION"\n"
    "usage:\n"
    "   <executable> [options]\n"
    "where options are:\n"
    "   <no-options>                       Run all tests producing a 'pretty' print report (default behavior).\n"
    "   -b, --base-path                    Base path for test sources. Allow to shorten test file paths when running/listing tests.\n"
    "   -d, --run-detail <level>           Control Run Tests report detail level: '1' (failed), '2' (failed+skipped), '3' (passed+failed+skipped). Default is '3'.\n"
    "   -e, --exclude <expr>               Suite/Test expression to be excluded. Works when running/listing tests.\n"
    "   -h, --help                         Display usage information.\n"
    "   -i, --include <expr>               Suite/Test expression to be included. Works when running/listing tests.\n"
    "   -l, --list-tests                   List all/matching test cases in 'pretty' format (default).\n"
    "   -o, --output <stream>              Specify output stream for a report: 'stdout' (default) or <file path>.\n"
    "   -r, --report <format>              Specify a report format when running tests: 'pretty', 'csv', 'json'.\n"
    "   -t, --run-tests                    Execute all/matching test cases (same as <no-options>).\n"
    "   -v, --version                      Show CLove-Unit version.\n"
    "   -x, --error-on-test-fail           Run Tests process will end with error in case of test failure. Default is to end the process successfully.\n"
    "\n"
    "For detailed usage please look at the README in https://github.com/fdefelici/clove-unit.\n"
    ;
    CLOVE_STRING_EQ(expected, cmd_out);
}