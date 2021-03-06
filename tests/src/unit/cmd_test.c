#define CLOVE_SUITE_NAME CommandLineTest
#include "clove-unit.h"

CLOVE_TEST(Init) {
    char* argv[7] = {"exec", "-r", "json", "-i", "one", "-i", "two"};
    int argc = 7;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    CLOVE_IS_TRUE(__clove_cmdline_has_opt(&cmd, "r"));
    CLOVE_STRING_EQ("json", __clove_cmdline_get_opt_value(&cmd, "r"));
    CLOVE_IS_TRUE(__clove_cmdline_has_opt(&cmd, "i"));

    __clove_vector_t* values = __clove_cmdline_get_opt_values(&cmd, "i");
    CLOVE_INT_EQ(2, (int)__clove_vector_count(values));
    CLOVE_STRING_EQ("one", *(char**)__clove_vector_get(values, 0));
    CLOVE_STRING_EQ("two", *(char**)__clove_vector_get(values, 1));

    __clove_cmdline_free(&cmd);
} 

CLOVE_TEST(InitAndAdd) {
    char* argv[3] = {"exec", "-i", "one"};
    int argc = 3;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    __clove_cmdline_add_opt(&cmd, "r", "json");
    CLOVE_STRING_EQ("json", __clove_cmdline_get_opt_value(&cmd, "r"));
    CLOVE_STRING_EQ("one", __clove_cmdline_get_opt_value(&cmd, "i"));

    __clove_cmdline_free(&cmd);
} 


