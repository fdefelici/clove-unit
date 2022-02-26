#define CLOVE_SUITE_NAME AssertsFailTest
#include "clove.h"

CLOVE_TEST(IntEq) {
    CLOVE_INT_EQ(1, 1);
    CLOVE_INT_EQ(2, 1);
}

CLOVE_TEST(IntNe) {
    CLOVE_INT_NE(2, 1);
    CLOVE_INT_NE(1, 1);
}

CLOVE_TEST(IsTrue) {
    CLOVE_IS_TRUE(true);
    CLOVE_IS_TRUE(false);
}

CLOVE_TEST(IsFalse) {
    CLOVE_IS_FALSE(false);
    CLOVE_IS_FALSE(true);
}