#define CLOVE_SUITE_NAME MySecondSuite
#include "clove.h"

CLOVE_TEST(FailingTest) {
    int a = 1;
    int b = 2;
    CLOVE_INT_EQ(a, b);
}


CLOVE_TEST(ChecNotNull) {
    int* null_ptr = NULL;
    CLOVE_NOT_NULL(null_ptr);
}


CLOVE_TEST(CheckNull) {
    int myvalue = 1;
    CLOVE_NULL(&myvalue);
}

CLOVE_TEST(SkippedTest) {
    
}

