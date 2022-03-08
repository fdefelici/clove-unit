#define CLOVE_SUITE_NAME CommandLineTest
#include "clove-unit.h"

CLOVE_TEST(TestNoOpt) {
    char* argv[1];
    int argc = 1;
    __clove_cmdline_t cmd = __clove_cmdline_create(argv, argc);
    
    char* arg = NULL;
    bool found = __clove_cmdline_next_opt(&cmd, &arg);
    CLOVE_IS_FALSE(found);

    CLOVE_INT_EQ(1, cmd.arg_index);
    CLOVE_NULL(arg);
} 

CLOVE_TEST(TestOneOpt) {
    char* argv[2] = {"", "-v"};
    int argc = 2;
    __clove_cmdline_t ctx = __clove_cmdline_create(argv, argc);

    char* arg = NULL;
    bool found = __clove_cmdline_next_opt(&ctx, &arg);
    CLOVE_IS_TRUE(found);

    CLOVE_INT_EQ(2, ctx.arg_index);
    CLOVE_STRING_EQ("v", arg);
} 
