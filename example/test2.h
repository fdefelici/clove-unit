#include "clove.h"

CLOVE_TEST(BooleanTest) {
    int TRUE = 1;
    int FALSE = 0;
    CLOVE_IS_TRUE(TRUE);
}

CLOVE_TEST(IntTest) {
    CLOVE_INT_EQ(2-1, 1);
}

CLOVE_TEST(FloatTest) {
    CLOVE_FLOAT_NE(1.0f, 1.0001f);
    CLOVE_FLOAT_EQ(1.0f, 1.000001f); //Two float are equals if their diff is <= 0.000001f 
}
