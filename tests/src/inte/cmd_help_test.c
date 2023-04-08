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