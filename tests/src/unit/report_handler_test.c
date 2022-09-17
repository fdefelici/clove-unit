#define CLOVE_SUITE_NAME UNIT_ReportHandlerTest
#include "clove-unit.h"
#include "utils/utils.h"

CLOVE_TEST(Simple) {
    char* file_path = __clove_path_rel_to_abs_exec_path("myfile.json");
    #define argc 5
    char* argv[argc] = {"exec", "-r", "json", "-o", file_path};

    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    //__clove_cmdline_errno_t result = __clove_cmdline_handle_report(&cmd);
    //printf(file_path);
    CLOVE_IS_TRUE(file_exists(file_path));
    
    CLOVE_INT_EQ(__CLOVE_CMD_ERRNO_OK, result);

}

