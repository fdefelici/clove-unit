#define CLOVE_SUITE_NAME StringAssertsTest
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
    free(actual);
}

CLOVE_TEST(StringStartsWith) {
    const char* str1 = "-v";
    const char* prefix = "-";
    bool result = __clove_string_startswith(str1, prefix);
    CLOVE_IS_TRUE(result);

    const char* str2 = "";
    result = __clove_string_startswith(str2, prefix);
    CLOVE_IS_FALSE(result);

    const char* str3 = NULL;
    result = __clove_string_startswith(str3, prefix);
    CLOVE_IS_FALSE(result);
}
