#define CLOVE_SUITE_NAME INTE_InvalidCmdTest
#include <clove-unit.h>
#include "utils/utils.h"

static char* cmd_out;

CLOVE_SUITE_SETUP() {
    cmd_out = NULL;
}

CLOVE_SUITE_TEARDOWN() {
    if (cmd_out) __clove_memory_free(cmd_out);
}

//Note: Important for this unit test is to demostrate that with invalid params the process ends gracefully
CLOVE_TEST(InvalidDash) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
}

CLOVE_TEST(InvalidDashOneLetter) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -j";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
}

CLOVE_TEST(InvalidDashTwoLetters) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -jj";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
}

CLOVE_TEST(InvalidDoubleDash) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
} 

CLOVE_TEST(InvalidDoubleDashParam) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --hello";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
} 