#define CLOVE_SUITE_NAME UNIT_ReportConsoleTest
#include "clove-unit.h"

CLOVE_TEST(SameLengthFirstCharDiff) {
    char exp[] = "H123456789012345111111111111111111111111";
    char act[] = "h123456789012345111111111111111111111111";
    char exp_short[16];
    char act_short[16];
    __clove_report_pretty_string_ellipse(exp, sizeof(exp)-1, act, sizeof(act)-1, exp_short, act_short, 16);

    CLOVE_STRING_EQ("H12345678901...", exp_short);
    CLOVE_STRING_EQ("h12345678901...", act_short);
}


CLOVE_TEST(ActShortThanExpFirstCharDiff) {
    char exp[] = "H123456789012345111111111111111111111111";
    char act[] = "h123456";
    char exp_short[16];
    char act_short[16];
    __clove_report_pretty_string_ellipse(exp, sizeof(exp)-1, act, sizeof(act)-1, exp_short, act_short, 16);

    CLOVE_STRING_EQ("H12345678901...", exp_short);
    CLOVE_STRING_EQ("h123456", act_short);
}
    
CLOVE_TEST(ActShortThanExpSecondCharDiff) {
    char exp[] = "0H23456789012345111111111111111111111111";
    char act[] = "0h23456";
    char exp_short[16];
    char act_short[16];
    __clove_report_pretty_string_ellipse(exp, sizeof(exp)-1, act, sizeof(act)-1, exp_short, act_short, 16);
   
    CLOVE_STRING_EQ("0H2345678901...", exp_short);
    CLOVE_STRING_EQ("0h23456", act_short);
}


CLOVE_TEST(ActShortThanExpForthCharDiff) {
    char exp[] = "012H456789012345111111111111111111111111";
    char act[] = "012h456";
    char exp_short[16];
    char act_short[16];
    __clove_report_pretty_string_ellipse(exp, sizeof(exp)-1, act, sizeof(act)-1, exp_short, act_short, 16);
   
    CLOVE_STRING_EQ("012H45678901...", exp_short);
    CLOVE_STRING_EQ("012h456", act_short);
}


CLOVE_TEST(ActShortThanExpLeftRightEllipsing) {
    char exp[] = "0123H56789012345111111111111111111111111";
    char act[] = "0123h56";
    char exp_short[16];
    char act_short[16];
    __clove_report_pretty_string_ellipse(exp, sizeof(exp)-1, act, sizeof(act)-1, exp_short, act_short, 16);
   
    CLOVE_STRING_EQ("...123H56789...", exp_short);
    CLOVE_STRING_EQ("0123h56", act_short);
}

CLOVE_TEST(ActShortThanExpWithEqualityNoEllipse) {
    char exp[] = "012";
    char act[] = "01";
    char exp_short[16];
    char act_short[16];
    __clove_report_pretty_string_ellipse(exp, sizeof(exp)-1, act, sizeof(act)-1, exp_short, act_short, 16);
   
    CLOVE_STRING_EQ("012", exp_short);
    CLOVE_STRING_EQ("01", act_short);
}

CLOVE_TEST(ActShortThanExpWithEqualityWithEllipse) {
    char exp[] = "012345678901234567890";
    char act[] = "01";
    char exp_short[16];
    char act_short[16];
    __clove_report_pretty_string_ellipse(exp, __clove_string_length(exp), 
                                         act, __clove_string_length(act), 
                                         exp_short, act_short, 16);
   
    CLOVE_STRING_EQ("012345678901...", exp_short);
    CLOVE_STRING_EQ("01", act_short);
}


CLOVE_TEST(ActEmpty) {
    char exp[] = "H123456789012345111111111111111111111111";
    char act[] = "";
    char exp_short[16];
    char act_short[16];
    __clove_report_pretty_string_ellipse(exp, sizeof(exp)-1, act, sizeof(act)-1, exp_short, act_short, 16);
   
    CLOVE_STRING_EQ("H12345678901...", exp_short);
    CLOVE_STRING_EQ("", act_short);
}

CLOVE_TEST(PrettyEllipseWithBigStrings) {
    const char* exp = 
    "{\n"
    "\t\"clove_version\" : \""__CLOVE_VERSION"\",\n"
    "\t\"json_schema\" : \"1.0\",\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 2,\n"
    "\t\t\"test_count\" : 3,\n"
    "\t\t\"suites\" : [\n"
    "\t\t\t{\n"
    "\t\t\t\t\"name\" : \"Prj01Suite01\",\n"
    "\t\t\t\t\"file\" : \"src/prj01_test1.c\",\n"
    "\t\t\t\t\"tests\" : [\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test01\",\n"
    "\t\t\t\t\t\t\"line\" : 4\n"
    "\t\t\t\t\t},\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test02\",\n"
    "\t\t\t\t\t\t\"line\" : 8\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t]\n"
    "\t\t\t},\n"
    "\t\t\t{\n"
    "\t\t\t\t\"name\" : \"Prj01Suite02\",\n"
    "\t\t\t\t\"file\" : \"src/prj01_test2.c\",\n"
    "\t\t\t\t\"tests\" : [\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test21\",\n"
    "\t\t\t\t\t\t\"line\" : 4\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t]\n"
    "\t\t\t}\n"
    "\t\t]\n"
    "\t}\n"
    "}\n";

    const char* act = 
    "{\n"
    "\t\"clove_version\" : \""__CLOVE_VERSION"\",\n"
    "\t\"json_schema\" : \"1.0\",\n"
    "\t\"result\" : {\n"
    "\t\t\"suite_count\" : 2,\n"
    "\t\t\"test_count\" : 3,\n"
    "\t\t\"suites\" : [\n"
    "\t\t\t{\n"
    "\t\t\t\t\"name\" : \"Prj01Suite01\",\n"
    "\t\t\t\t\"file\" : \"src/prj01_test1.c\",\n"
    "\t\t\t\t\"tests\" : [\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test01\",\n"
    "\t\t\t\t\t\t\"line\" : 4\n"
    "\t\t\t\t\t},\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test02\",\n"
    "\t\t\t\t\t\t\"line\" : 8\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t]\n"
    "\t\t\t},\n"
    "\t\t\t{\n"
    "\t\t\t\t\"name\" : \"Prj01Suite02\",\n"
    "\t\t\t\t\"file\" : \"src/prj01_test2.c\",\n"
    "\t\t\t\t\"tests\" : [\n"
    "\t\t\t\t\t{\n"
    "\t\t\t\t\t\t\"name\" : \"Test21\",\n"
    "\t\t\t\t\t\t\"line\" : 4\n"
    "\t\t\t\t\t}\n"
    "\t\t\t\t]\n"
    "\t\t\t}\n"
    "\t\t]\n"
    "\t}\n"; 
    //miss final }

    size_t exp_len = __clove_string_length(exp);
    size_t act_len = __clove_string_length(act);

    char exp_short[16];
    char act_short[16];
    __clove_report_pretty_string_ellipse(exp, exp_len, act, act_len, exp_short, act_short, 16);

    CLOVE_STRING_EQ("...\t}\n\t\t]\n\t}\n}\n", exp_short);
    CLOVE_STRING_EQ("...\t\t\t}\n\t\t]\n\t}\n", act_short);
}

CLOVE_TEST(StringEllipseEmpty) {
    char string[] = "";
    size_t str_len = sizeof(string);
    size_t pos = 0;
    char out[16];
    size_t out_size = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_size);

    CLOVE_STRING_EQ("", out);
}

CLOVE_TEST(StringEllipseShortString) {
    char string[] = "123456789012345";
    size_t str_len = sizeof(string)-1;
    size_t pos = 0;
    char out[16];
    size_t out_size = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_size);

    CLOVE_STRING_EQ("123456789012345", out);
}

CLOVE_TEST(StringEllipseAtPos0) {
    char string[] = "123456789012345111111";
    size_t str_len = sizeof(string)-1;
    size_t pos = 0;
    char out[16];
    size_t out_size = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_size);

    CLOVE_STRING_EQ("123456789012...", out);
}

CLOVE_TEST(StringEllipseAtPos3) {
    char string[] = "123456789012345111111";
    size_t str_len = sizeof(string)-1;
    size_t pos = 1;
    char out[16];
    size_t out_size = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_size);

    CLOVE_STRING_EQ("123456789012...", out);
}

CLOVE_TEST(StringEllipseAtPos4) {
    char string[] = "123456789012345111111";
    size_t str_len = sizeof(string)-1;
    size_t pos = 4;
    char out[16];
    size_t out_size = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_size);

    CLOVE_STRING_EQ("...234567890...", out);
}

CLOVE_TEST(StringEllipseAtPos21) {
    //               01234567890123456789 
    char string[] = "123456789012345111112";
    size_t str_len = sizeof(string)-1;
    size_t pos = 20;
    char out[16];
    size_t out_size = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_size);

    printf("%c\n", string[pos]);
    CLOVE_STRING_EQ("...012345111112", out);
}

CLOVE_TEST(StringEllipseLeftAndRight) {
    char string[] = "123456789012345111111";
    size_t str_len = sizeof(string)-1;
    size_t pos = 4;
    char out[16];
    size_t out_size = sizeof(out);
    __clove_string_ellipse(string, str_len, pos, out, out_size);
    CLOVE_STRING_EQ("...234567890...", out);
}