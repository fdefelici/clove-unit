#define CLOVE_SUITE_NAME StringAssertsTest
#include "clove.h"

CLOVE_TEST(StringStaticallyAllocated) {
    CLOVE_STRING_EQ("hello", "hello");
}

CLOVE_TEST(StringAllocatedOnStack) {
    char string1[] = "MyStackString";
    char string2[] = "MyStackString";
    CLOVE_STRING_EQ(string1, string2); //TODO: good test is to verify during reporting phase
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
