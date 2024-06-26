#define CLOVE_SUITE_NAME UNIT_StringAssertsTest
#include "clove-unit.h"
#include <stdlib.h>

CLOVE_TEST(StringStaticallyAllocated) {
    CLOVE_STRING_EQ("hello", "hello");
}

CLOVE_TEST(StringAllocatedOnStack) {
    char string1[] = "MyStackString";
    char string2[] = "MyStackString";
    CLOVE_STRING_EQ(string1, string2);
}

CLOVE_TEST(LongString) {
    char string[256];
    for(int i=0; i < sizeof(string) - 1; ++i) {
        string[i] = 'a';
    }
    string[sizeof(string)-1] = '\0';

    CLOVE_STRING_NE(string, "baaaaaaaaaa");
}

CLOVE_TEST(StringEscaping) {
    const char* to_escape = "\"Hello\nWorld\"";
    const char* expected = "\\\"Hello\\nWorld\\\"";
    char* actual = __clove_string_escape(to_escape);
    CLOVE_STRING_EQ(expected, actual);
    __clove_memory_free(actual);
}

CLOVE_TEST(StringStartsWith) {
    CLOVE_IS_TRUE(__clove_string_startswith("-v", "-"));
    CLOVE_IS_FALSE(__clove_string_startswith("-v", ""));
    CLOVE_IS_FALSE(__clove_string_startswith("-v", NULL));
}

CLOVE_TEST(StringCsvEscape) {
    CLOVE_STRING_EQ("\"Hello,World\"", __clove_string_csv_escape("Hello,World"));
    CLOVE_STRING_EQ("\"\"", __clove_string_csv_escape("\""));
    CLOVE_STRING_EQ("\\n\\t", __clove_string_csv_escape("\n\t"));
}

CLOVE_TEST(StringIsEqualsAny) {
    CLOVE_IS_TRUE(__clove_string_equal_any("mystring", 1, "mystring"));
    CLOVE_IS_FALSE(__clove_string_equal_any("mystring", 1, "one"));
    CLOVE_IS_TRUE(__clove_string_equal_any("mystring", 3, "one", "two", "mystring"));
    CLOVE_IS_FALSE(__clove_string_equal_any("mystring", 2, "one", "two"));
}

CLOVE_TEST(StringLastIndexOf) {
    CLOVE_INT_EQ(-1, __clove_string_last_indexof("hello_o", 'a'));
    CLOVE_INT_EQ( 6, __clove_string_last_indexof("hello_o", 'o'));
    CLOVE_INT_EQ( 3, __clove_string_last_indexof("hello_o", 'l'));
    CLOVE_INT_EQ( 1, __clove_string_last_indexof("hello_o", 'e'));
}