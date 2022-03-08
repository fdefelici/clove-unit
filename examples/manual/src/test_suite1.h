#define CLOVE_ENABLE_MANUAL
#include "clove-unit.h"

CLOVE_TEST(FirstTest) {
    int a = 1;
    int b = 1;
    CLOVE_INT_EQ(a, b);
}

CLOVE_TEST(SecondTest) {
    int a = 1;
    int b = 2;

    CLOVE_INT_NE(a, b);
}

CLOVE_TEST(FailingTest) {
    int a = 1;
    int b = 2;
    CLOVE_INT_EQ(a, b);
}

CLOVE_SUITE(TestSuite1) {
    CLOVE_SUITE_TESTS(
        FirstTest, 
        SecondTest, 
        FailingTest
    );
}
