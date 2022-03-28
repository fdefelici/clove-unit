#define CLOVE_SUITE_NAME StringViewTest
#include <clove-unit.h>

CLOVE_TEST(CreationFromOffset) {
    const char* str = "Hello World"; 
    __clove_string_view_t vw;
    
    vw = __clove_string_view_from_offs(str, 0, 4);
    CLOVE_STRING_EQ("Hello World", vw.begin);
    CLOVE_INT_EQ(5, (int)vw.length);

    vw = __clove_string_view_from_offs(str, 6, 10);
    CLOVE_STRING_EQ("World", vw.begin);
    CLOVE_INT_EQ(5, (int)vw.length);
}

CLOVE_TEST(CreationFromBeginEnd) {
    const char* str = "Hello World"; 
    __clove_string_view_t vw;
    
    vw = __clove_string_view_from_be(str, str+4);
    CLOVE_STRING_EQ("Hello World", vw.begin);
    CLOVE_INT_EQ(5, (int)vw.length);

    vw = __clove_string_view_from_be(str+6, str+10);
    CLOVE_STRING_EQ("World", vw.begin);
    CLOVE_INT_EQ(5, (int)vw.length);
}

CLOVE_TEST(CreationFromLen) {
    const char* str = "Hello World"; 
    __clove_string_view_t vw;
    
    vw = __clove_string_view_from_len(str, 5);
    CLOVE_STRING_EQ("Hello World", vw.begin);
    CLOVE_INT_EQ(5, (int)vw.length);

    vw = __clove_string_view_from_len(str+6, 5);
    CLOVE_STRING_EQ("World", vw.begin);
    CLOVE_INT_EQ(5, (int)vw.length);
}

CLOVE_TEST(CreationFromString) {
    const char* str = "Hello World"; 
    __clove_string_view_t vw;
    
    vw = __clove_string_view_from_str(str);
    CLOVE_STRING_EQ("Hello World", vw.begin);
    CLOVE_INT_EQ(11, (int)vw.length);

    vw = __clove_string_view_from_str(str+6);
    CLOVE_STRING_EQ("World", vw.begin);
    CLOVE_INT_EQ(5, (int)vw.length);
}

CLOVE_TEST(Length) {
    const char* str = "Hello World"; 
    __clove_string_view_t vw = __clove_string_view_from_offs(str, 0, 4);

    CLOVE_INT_EQ(5, (int)__clove_string_view_length(&vw));
}

CLOVE_TEST(BeginAndEnd) {
    const char* str = "Hello World"; 
    __clove_string_view_t vw = __clove_string_view_from_offs(str, 0, 4);

    CLOVE_STRING_EQ("Hello World", __clove_string_view_begin(&vw));
    CLOVE_STRING_EQ("o World", __clove_string_view_end(&vw));
}

CLOVE_TEST(EqualsWithString) {
    const char* str = "Hello World"; 
    __clove_string_view_t vw = __clove_string_view_from_offs(str, 0, 4);

    CLOVE_IS_TRUE(__clove_string_view_strequals(&vw, "Hello"));
    CLOVE_IS_FALSE(__clove_string_view_strequals(&vw, "Hello1"));
    CLOVE_IS_FALSE(__clove_string_view_strequals(&vw, "World"));
}



CLOVE_TEST(Equals) {
    const char* str1 = "Hello World"; 
    __clove_string_view_t vw1 = __clove_string_view_from_str(str1+6);

    const char* str2 = "World"; 
    __clove_string_view_t vw2 = __clove_string_view_from_str(str2);

    CLOVE_IS_TRUE(__clove_string_view_equals(&vw1, &vw1));
}

CLOVE_TEST(AsString) {
    const char* str = "Hello World"; 
    __clove_string_view_t vw1 = __clove_string_view_from_len(str, 5);

    char* result = __clove_string_view_as_string(&vw1);
    CLOVE_STRING_EQ("Hello", result);
    free(result);
}

CLOVE_TEST(EndWithUsingOffsetOnSuffix) {
    const char* str = "Hello World"; 
    __clove_string_view_t vw1 = __clove_string_view_from_str(str);
    __clove_string_view_t suffix;
    bool result ;
    suffix =  __clove_string_view_from_str("1World");
    result = __clove_string_view_nendswith(&vw1, &suffix, 1);
    CLOVE_IS_TRUE(result);

    suffix =  __clove_string_view_from_str("World");
    result = __clove_string_view_nendswith(&vw1, &suffix, 3);
    CLOVE_IS_TRUE(result);

    suffix =  __clove_string_view_from_str("1World");
    result = __clove_string_view_nendswith(&vw1, &suffix, 0);
    CLOVE_IS_FALSE(result);
}

CLOVE_TEST(Contains) {
    const char* str = "Hello World"; 
    __clove_string_view_t vw1 = __clove_string_view_from_str(str);
    __clove_string_view_t suffix;
    bool result ;
    suffix =  __clove_string_view_from_offs("1World", 2, 1);
    result = __clove_string_view_contains(&vw1, &suffix);
    CLOVE_IS_TRUE(result);
}

