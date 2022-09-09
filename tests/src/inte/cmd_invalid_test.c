#define CLOVE_SUITE_NAME InvalidCmdTest
#include <clove-unit.h>
#include "utils/utils.h"

char* cmd_out;

CLOVE_SUITE_SETUP() {
    cmd_out = NULL;
}

CLOVE_SUITE_TEARDOWN() {
    if (cmd_out) free(cmd_out);
}

//Note: Generic error just because PRJ01 has one test with failure
CLOVE_TEST(InvalidDash) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_GENERIC, cmd_code);
}

CLOVE_TEST(InvalidDashOneLetter) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -j";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_GENERIC, cmd_code);
}

CLOVE_TEST(InvalidDashTwoLetters) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -jj";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_GENERIC, cmd_code);
}

CLOVE_TEST(InvalidDoubleDash) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_GENERIC, cmd_code);
} 

CLOVE_TEST(InvalidDoubleDashParam) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --hello";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_GENERIC, cmd_code);
} 