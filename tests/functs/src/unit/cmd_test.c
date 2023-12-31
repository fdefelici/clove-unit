#define CLOVE_SUITE_NAME UNIT_CommandLineTest
#include "clove-unit.h"

CLOVE_TEST(InitWithNoArgs) {
    const char* argv[1] = {"exec"};
    int argc = 1;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    CLOVE_PASS();
    __clove_cmdline_free(&cmd);
} 

CLOVE_TEST(InitWithOneOptNoValue) {
    const char* argv[2] = {"exec", "-o"};
    int argc = 2;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    CLOVE_IS_TRUE(__clove_cmdline_has_opt(&cmd, "o"));
    __clove_cmdline_free(&cmd);
} 

CLOVE_TEST(InitWithMultipleOpt) {
    const char* argv[7] = {"exec", "-r", "json", "-i", "one", "-i", "two"};
    int argc = 7;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    CLOVE_IS_TRUE(__clove_cmdline_has_opt(&cmd, "r"));
    CLOVE_STRING_EQ("json", __clove_cmdline_get_opt_value(&cmd, "r"));
    CLOVE_IS_TRUE(__clove_cmdline_has_opt(&cmd, "i"));

    const __clove_vector_t* values = __clove_cmdline_get_opt_values(&cmd, "i");
    CLOVE_INT_EQ(2, (int)__clove_vector_count(values));
    CLOVE_STRING_EQ("one", *(char**)__clove_vector_get(values, 0));
    CLOVE_STRING_EQ("two", *(char**)__clove_vector_get(values, 1));

    __clove_cmdline_free(&cmd);
} 

CLOVE_TEST(InitAndAdd) {
    const char* argv[3] = {"exec", "-i", "one"};
    int argc = 3;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    __clove_cmdline_add_opt(&cmd, "r", "json");
    CLOVE_STRING_EQ("json", __clove_cmdline_get_opt_value(&cmd, "r"));
    CLOVE_STRING_EQ("one", __clove_cmdline_get_opt_value(&cmd, "i"));

    __clove_cmdline_free(&cmd);
}

CLOVE_TEST(OptDashWithTwoLetter) {
    const char* argv[2] = {"exec", "-ii"}; //single dash option accept only 1 char option name
    int argc = 2;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    CLOVE_IS_FALSE(__clove_cmdline_has_opt(&cmd, "ii"));
    __clove_cmdline_free(&cmd);
}

CLOVE_TEST(OptDoubleDash) {
    const char* argv[2] = {"exec", "--ii"};
    int argc = 2;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    CLOVE_IS_TRUE(__clove_cmdline_has_opt(&cmd, "ii"));
    __clove_cmdline_free(&cmd);
}

CLOVE_TEST(OptDoubleDashSeparatedByDash) {
    const char* argv[2] = {"exec", "--list-tests"};
    int argc = 2;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    CLOVE_IS_TRUE(__clove_cmdline_has_opt(&cmd, "list-tests"));
    __clove_cmdline_free(&cmd);
}

CLOVE_TEST(InvalidOptJustDash) {
    const char* argv[2] = {"exec", "-"};
    int argc = 2;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    CLOVE_PASS();
    __clove_cmdline_free(&cmd);
}

CLOVE_TEST(HasOneOpt) {
    const char* argv[4] = {"exec", "-o", "--output", "-a"};
    int argc = 4;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    CLOVE_IS_TRUE(__clove_cmdline_has_any_opt(&cmd, "o", "output"));
    CLOVE_IS_TRUE(__clove_cmdline_has_any_opt(&cmd, "a", "unexistent"));
    CLOVE_IS_FALSE(__clove_cmdline_has_any_opt(&cmd, "unexistent", "unexistent"));

    __clove_cmdline_free(&cmd);
}

CLOVE_TEST(GetOneOptValue) {
    const char* argv[7] = {"exec", "-o", "value1", "--output", "value2", "-a", "value3"};
    int argc = 7;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    CLOVE_STRING_EQ("value1", __clove_cmdline_get_any_opt_value(&cmd, "o", "output"));
    CLOVE_STRING_EQ("value2", __clove_cmdline_get_any_opt_value(&cmd, "output", "o"));
    CLOVE_STRING_EQ("value3", __clove_cmdline_get_any_opt_value(&cmd, "a", "unexistent"));
    CLOVE_STRING_EQ("value3", __clove_cmdline_get_any_opt_value(&cmd, "unexistent", "a"));
    CLOVE_NULL( __clove_cmdline_get_any_opt_value(&cmd, "unexistent", "unexistent"));

    __clove_cmdline_free(&cmd);
}

CLOVE_TEST(InitWithTwoOptWithoutValue) {
    const char* argv[3] = {"exec", "-r", "-i"};
    int argc = 3;
    __clove_cmdline_t cmd;
    __clove_cmdline_init(&cmd, argv, argc);

    CLOVE_IS_TRUE(__clove_cmdline_has_opt(&cmd, "r"));
    CLOVE_NULL(__clove_cmdline_get_opt_value(&cmd, "r"));
    CLOVE_IS_TRUE(__clove_cmdline_has_opt(&cmd, "i"));
    CLOVE_NULL(__clove_cmdline_get_opt_value(&cmd, "i"));

    __clove_cmdline_free(&cmd);
}