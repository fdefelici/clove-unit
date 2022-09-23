#define CLOVE_SUITE_NAME INTE_CmdVersionTest
#include <clove-unit.h>
#include "utils/utils.h"

char* cmd_out;

CLOVE_SUITE_SETUP() {
    cmd_out = NULL;
}

CLOVE_SUITE_TEARDOWN() {
    if (cmd_out) free(cmd_out);
}
 
CLOVE_TEST(RunVersionWithOptV) {
    const char* cmd = RES_PRJ01_EXEC_PATH" -v";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_STRING_EQ(__CLOVE_VERSION, cmd_out);
}

CLOVE_TEST(RunVersionWithOptVersion) {
    const char* cmd = RES_PRJ01_EXEC_PATH" --version";
    int cmd_code = exec_cmd(cmd, &cmd_out);
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, cmd_code);
    CLOVE_STRING_EQ(__CLOVE_VERSION, cmd_out);
}


