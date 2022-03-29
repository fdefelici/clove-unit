#define CLOVE_SUITE_NAME AssertsTest
#include "clove-unit.h"

CLOVE_TEST(IsTrue) {
    CLOVE_IS_TRUE(true);
    //CLOVE_IS_TRUE(false);
}

CLOVE_TEST(IsFalse) {
    CLOVE_IS_FALSE(false);
    //CLOVE_IS_FALSE(true);
}

CLOVE_TEST(IntEq) {
    CLOVE_INT_EQ(1, 1);
    //CLOVE_INT_EQ(2, 1);
}

CLOVE_TEST(IntNe) {
    CLOVE_INT_NE(2, 1);
    //CLOVE_INT_NE(1, 1);
}

CLOVE_TEST(CharEq) {
    CLOVE_CHAR_EQ('a', 'a');
    //CLOVE_CHAR_EQ('a', 'b');
}

CLOVE_TEST(CharNe) {
    CLOVE_CHAR_NE('a', 'b');
    //CLOVE_CHAR_NE('a', 'a');
}

CLOVE_TEST(UIntEq) {
    CLOVE_UINT_EQ(1, 1);
    //CLOVE_UINT_EQ(1, 2);
}

CLOVE_TEST(UintNe) {
    CLOVE_UINT_NE(1, 2);
    //CLOVE_UINT_NE(1, 1);
}

CLOVE_TEST(LongEq) {
    CLOVE_LONG_EQ(1, 1);
    //CLOVE_LONG_EQ(1, 2);
}

CLOVE_TEST(LongNe) {
    CLOVE_LONG_NE(1, 2);
    //CLOVE_LONG_NE(1, 1);
}

CLOVE_TEST(ULongEq) {
    CLOVE_ULONG_EQ(1, 1);
    //CLOVE_ULONG_EQ(1, 2);
}

CLOVE_TEST(ULongNe) {
    CLOVE_ULONG_NE(1, 2);
    //CLOVE_ULONG_NE(1, 1);
}

CLOVE_TEST(LLongEq) {
    CLOVE_LLONG_EQ(1, 1);
    //CLOVE_LLONG_EQ(1, 2);
}

CLOVE_TEST(LLongNe) {
    CLOVE_LLONG_NE(1, 2);
    //CLOVE_LLONG_NE(1, 1);
}

CLOVE_TEST(ULLongEq) {
    CLOVE_ULLONG_EQ(1, 1);
    //CLOVE_ULLONG_EQ(1, 2);
}

CLOVE_TEST(ULLongNe) {
    CLOVE_ULLONG_NE(1, 2);
    //CLOVE_ULLONG_NE(1, 1);
}

CLOVE_TEST(FloatEq) {
    CLOVE_FLOAT_EQ(1, 1);
    //CLOVE_FLOAT_EQ(1, 2);
}

CLOVE_TEST(FloatNe) {
    CLOVE_FLOAT_NE(1, 2);
    //CLOVE_FLOAT_NE(1, 1);
}

CLOVE_TEST(DoubleEq) {
    CLOVE_DOUBLE_EQ(1, 1);
    //CLOVE_DOUBLE_EQ(1, 2);
}

CLOVE_TEST(DoubleNe) {
    CLOVE_DOUBLE_NE(1, 2);
    //CLOVE_DOUBLE_NE(1, 1);
}

CLOVE_TEST(StringEq) {
    CLOVE_STRING_EQ("a", "a");
    //CLOVE_STRING_EQ("a", "b");
}

CLOVE_TEST(StringNe) {
    CLOVE_STRING_NE("a", "b");
    //CLOVE_STRING_NE("a", "a");
}

CLOVE_TEST(PtrEq) {
    const int v1 = 1;
    const int v2 = 1;
    CLOVE_PTR_EQ(&v1, &v1);
    //CLOVE_PTR_EQ(&v1, &v2);
}

CLOVE_TEST(PtrNe) {
    const int v1 = 1;
    const int v2 = 1;
    CLOVE_PTR_NE(&v1, &v2);
    //CLOVE_PTR_NE(&v1, &v1);
}

CLOVE_TEST(IsNull) {
    const int* null = NULL;
    const int not_null = 1;
    CLOVE_NULL(null);
    //CLOVE_NULL(&not_null);
}

CLOVE_TEST(IsNotNull) {
    const int* null = NULL;
    const int not_null = 1;
    CLOVE_NOT_NULL(&not_null);
    //CLOVE_NOT_NULL(null);
}

CLOVE_TEST(AssertPassFail) {
    CLOVE_PASS();
    //CLOVE_FAIL();
}
