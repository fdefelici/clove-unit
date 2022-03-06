#define CLOVE_SUITE_NAME ReportConsoleTest
#include "clove.h"

CLOVE_TEST(SameLengthFirstCharDiff) {
    char exp[] = "H123456789012345111111111111111111111111";
    char act[] = "h123456789012345111111111111111111111111";
    char exp_short[16];
    char act_short[16];
    __clove_report_console_string_ellipse(exp, sizeof(exp), act, sizeof(act), exp_short, act_short, 16);

    CLOVE_STRING_EQ("H12345678901...", exp_short);
    CLOVE_STRING_EQ("h12345678901...", act_short);
}


CLOVE_TEST(ActShortThanExpFirstCharDiff) {
    char exp[] = "H123456789012345111111111111111111111111";
    char act[] = "h123456";
    char exp_short[16];
    char act_short[16];
    __clove_report_console_string_ellipse(exp, sizeof(exp), act, sizeof(act), exp_short, act_short, 16);

    CLOVE_STRING_EQ("H12345678901...", exp_short);
    CLOVE_STRING_EQ("h123456", act_short);
}
    
CLOVE_TEST(ActShortThanExpSecondCharDiff) {
    char exp[] = "0H23456789012345111111111111111111111111";
    char act[] = "0h23456";
    char exp_short[16];
    char act_short[16];
    __clove_report_console_string_ellipse(exp, sizeof(exp), act, sizeof(act), exp_short, act_short, 16);
   
    CLOVE_STRING_EQ("0H2345678901...", exp_short);
    CLOVE_STRING_EQ("0h23456", act_short);
}


CLOVE_TEST(ActShortThanExpForthCharDiff) {
    char exp[] = "012H456789012345111111111111111111111111";
    char act[] = "012h456";
    char exp_short[16];
    char act_short[16];
    __clove_report_console_string_ellipse(exp, sizeof(exp), act, sizeof(act), exp_short, act_short, 16);
   
    CLOVE_STRING_EQ("012H45678901...", exp_short);
    CLOVE_STRING_EQ("012h456", act_short);
}


CLOVE_TEST(ActShortThanExpLeftRightEllipsing) {
    char exp[] = "0123H56789012345111111111111111111111111";
    char act[] = "0123h56";
    char exp_short[16];
    char act_short[16];
    __clove_report_console_string_ellipse(exp, sizeof(exp), act, sizeof(act), exp_short, act_short, 16);
   
    CLOVE_STRING_EQ("...123H56789...", exp_short);
    CLOVE_STRING_EQ("0123h56", act_short);
}

CLOVE_TEST(ActEmpty) {
    char exp[] = "H123456789012345111111111111111111111111";
    char act[] = "";
    char exp_short[16];
    char act_short[16];
    __clove_report_console_string_ellipse(exp, sizeof(exp), act, sizeof(act), exp_short, act_short, 16);
   
    CLOVE_STRING_EQ("H12345678901...", exp_short);
    CLOVE_STRING_EQ("", act_short);
}

CLOVE_TEST(StringEllipseEmpty) {
    char string[] = "";
    size_t str_len = sizeof(string);
    size_t pos = 0;
    char out[16];
    size_t out_len = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_len);

    CLOVE_STRING_EQ("", out);
}

CLOVE_TEST(StringEllipseShortString) {
    char string[] = "123456789012345";
    size_t str_len = sizeof(string);
    size_t pos = 0;
    char out[16];
    size_t out_len = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_len);

    CLOVE_STRING_EQ("123456789012345", out);
}

CLOVE_TEST(StringEllipseAtPos0) {
    char string[] = "123456789012345111111";
    size_t str_len = sizeof(string);
    size_t pos = 0;
    char out[16];
    size_t out_len = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_len);

    CLOVE_STRING_EQ("123456789012...", out);
}

CLOVE_TEST(StringEllipseAtPos3) {
    char string[] = "123456789012345111111";
    size_t str_len = sizeof(string);
    size_t pos = 1;
    char out[16];
    size_t out_len = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_len);

    CLOVE_STRING_EQ("123456789012...", out);
}

CLOVE_TEST(StringEllipseAtPos4) {
    char string[] = "123456789012345111111";
    size_t str_len = sizeof(string);
    size_t pos = 4;
    char out[16];
    size_t out_len = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_len);

    CLOVE_STRING_EQ("...234567890...", out);
}

CLOVE_TEST(StringEllipseAtPos21) {
    char string[] = "123456789012345111111";
    size_t str_len = sizeof(string);
    size_t pos = 21;
    char out[16];
    size_t out_len = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_len);
    CLOVE_STRING_EQ("...012345111111", out);
}

CLOVE_TEST(StringEllipseLeftAndRight) {
    char string[] = "123456789012345111111";
    size_t str_len = sizeof(string);
    size_t pos = 4;
    char out[16];
    size_t out_len = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_len);
    CLOVE_STRING_EQ("...234567890...", out);
}