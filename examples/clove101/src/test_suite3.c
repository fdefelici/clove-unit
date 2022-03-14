#define CLOVE_SUITE_NAME MySuite03
#include "clove-unit.h"

CLOVE_TEST(FailingTest) {
    int a = 1;
    int b = 2;
    CLOVE_INT_EQ(a, b);
}

/*
CLOVE_TEST(CheckNotNull) {
    int* null_ptr = NULL;
    CLOVE_NOT_NULL(null_ptr);
}


CLOVE_TEST(CheckNull) {
    int myvalue = 1;
    CLOVE_NULL(&myvalue);
}
*/

CLOVE_TEST(StringFailTest) {
    CLOVE_STRING_EQ("Hello", "World");
}

CLOVE_TEST(SkippedTest) {
    
}

