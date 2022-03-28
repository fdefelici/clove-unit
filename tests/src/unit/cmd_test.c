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

CLOVE_TEST(Parse) {
    char* argv[7] = {"exec", "-r", "json", "-i", "one", "-i", "two"};
    int argc = 7;
    __clove_cmdline_t ctx = __clove_cmdline_parse(argv, argc);

    CLOVE_IS_TRUE(__clove_cmdline_has_opt(&ctx, "r"));
    CLOVE_STRING_EQ("json", __clove_cmdline_get_opt_value(&ctx, "r"));
    CLOVE_IS_TRUE(__clove_cmdline_has_opt(&ctx, "i"));

    __clove_vector_t* values = __clove_cmdline_get_opt_values(&ctx, "i");
    CLOVE_INT_EQ(2, (int)__clove_vector_count(values));
    CLOVE_STRING_EQ("one", *(char**)__clove_vector_get(values, 0));
    CLOVE_STRING_EQ("two", *(char**)__clove_vector_get(values, 1));
} 

CLOVE_TEST(ParseAndAdd) {
    char* argv[3] = {"exec", "-i", "one"};
    int argc = 3;
    __clove_cmdline_t ctx = __clove_cmdline_parse(argv, argc);

    __clove_cmdline_add_opt(&ctx, "r", "json");
    CLOVE_STRING_EQ("json", __clove_cmdline_get_opt_value(&ctx, "r"));
    CLOVE_STRING_EQ("one", __clove_cmdline_get_opt_value(&ctx, "i"));
} 


