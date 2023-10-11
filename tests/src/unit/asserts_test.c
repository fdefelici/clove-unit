#define CLOVE_SUITE_NAME UNIT_AssertsTest
#include <stdlib.h>
#include <time.h>
#include "clove-unit.h"

CLOVE_TEST(BoolComparison) {
    CLOVE_IS_TRUE(true);
    CLOVE_IS_FALSE(false);
}

CLOVE_TEST(CharComparison) {
    CLOVE_CHAR_EQ('a', 'a');
    CLOVE_CHAR_NE('a', 'b');

    CLOVE_CHAR_GT('b', 'a');
    CLOVE_CHAR_GTE('b', 'a');
    CLOVE_CHAR_GTE('b', 'b');

    CLOVE_CHAR_LT('a', 'b');
    CLOVE_CHAR_LTE('a', 'b');
    CLOVE_CHAR_LTE('b', 'b');
}

CLOVE_TEST(IntComparison) {
    CLOVE_INT_EQ(1, 1);
    CLOVE_INT_NE(2, 1);

    CLOVE_INT_GT(2, 1);
    CLOVE_INT_GTE(2, 1);
    CLOVE_INT_GTE(2, 2);
    
    CLOVE_INT_LT(1, 2);
    CLOVE_INT_LTE(1, 2);
    CLOVE_INT_LTE(2, 2);
}

CLOVE_TEST(UIntComparison) {
    CLOVE_UINT_EQ(1, 1);
    CLOVE_UINT_NE(1, 2);
    
    CLOVE_UINT_GT(2, 1);
    CLOVE_UINT_GTE(2, 1);
    CLOVE_UINT_GTE(2, 2);

    CLOVE_UINT_LT(1, 2);
    CLOVE_UINT_LTE(1, 2);
    CLOVE_UINT_LTE(2, 2);
}

CLOVE_TEST(LongComparison) {
    CLOVE_LONG_EQ(1, 1);
    CLOVE_LONG_NE(1, 2);

    CLOVE_LONG_GT(2, 1);
    CLOVE_LONG_GTE(2, 1);
    CLOVE_LONG_GTE(1, 1);
    
    CLOVE_LONG_LT(1, 2);
    CLOVE_LONG_LTE(1, 2);
    CLOVE_LONG_LTE(2, 2);
}

CLOVE_TEST(ULongComparison) {
    CLOVE_ULONG_EQ(1, 1);
    CLOVE_ULONG_NE(1, 2);

    CLOVE_ULONG_GT(2, 1);
    CLOVE_ULONG_GTE(2, 1);
    CLOVE_ULONG_GTE(1, 1);
    
    CLOVE_ULONG_LT(1, 2);
    CLOVE_ULONG_LTE(1, 2);
    CLOVE_ULONG_LTE(2, 2);
}

CLOVE_TEST(LLongComparison) {
    CLOVE_LLONG_EQ(1, 1);
    CLOVE_LLONG_NE(1, 2);

    CLOVE_LLONG_GT(2, 1);
    CLOVE_LLONG_GTE(2, 1);
    CLOVE_LLONG_GTE(1, 1);
    
    CLOVE_LLONG_LT(1, 2);
    CLOVE_LLONG_LTE(1, 2);
    CLOVE_LLONG_LTE(2, 2);    
}

CLOVE_TEST(ULLongComparison) {
    CLOVE_ULLONG_EQ(1, 1);
    CLOVE_ULLONG_NE(1, 2);

    CLOVE_ULLONG_GT(2, 1);
    CLOVE_ULLONG_GTE(2, 1);
    CLOVE_ULLONG_GTE(1, 1);
    
    CLOVE_ULLONG_LT(1, 2);
    CLOVE_ULLONG_LTE(1, 2);
    CLOVE_ULLONG_LTE(2, 2);
}

CLOVE_TEST(SizetComparison) {
    CLOVE_SIZET_EQ(1, 1);
    CLOVE_SIZET_NE(2, 1);

    CLOVE_SIZET_GT(2, 1);
    CLOVE_SIZET_GTE(2, 1);
    CLOVE_SIZET_GTE(1, 1);
    
    CLOVE_SIZET_LT(1, 2);
    CLOVE_SIZET_LTE(1, 2);
    CLOVE_SIZET_LTE(2, 2);
}

CLOVE_TEST(FloatComparison) {
    srand((unsigned)time(NULL) ^ (unsigned)clock());
    volatile float v1 = ((float)rand() / RAND_MAX) * (2.0f - 1.1f) + 1.1f;
    volatile float v2 = ((float)rand() / RAND_MAX) * (3.1f - 2.1f) + 2.1f;

    CLOVE_FLOAT_EQ(v1, v1);
    CLOVE_FLOAT_NE(v1, v2);

    CLOVE_FLOAT_GT(v2, v1);
    CLOVE_FLOAT_GTE(v2, v1);
    CLOVE_FLOAT_GTE(v2, v2);

    CLOVE_FLOAT_LT(v1, v2);
    CLOVE_FLOAT_LTE(v1, v2);
    CLOVE_FLOAT_LTE(v2, v2);
}

CLOVE_TEST(DoubleComparison) {
    srand((unsigned)time(NULL) ^ (unsigned)clock());
    volatile double v1 = ((double)rand() / RAND_MAX) * (2.0 - 1.1) + 1.1;
    volatile double v2 = ((double)rand() / RAND_MAX) * (3.1 - 2.1) + 2.1;

    CLOVE_DOUBLE_EQ(v1, v1);
    CLOVE_DOUBLE_NE(v1, v2);

    CLOVE_DOUBLE_GT(v2, v1);
    CLOVE_DOUBLE_GTE(v2, v1);
    CLOVE_DOUBLE_GTE(v2, v2);

    CLOVE_DOUBLE_LT(v1, v2);
    CLOVE_DOUBLE_LTE(v1, v2);
    CLOVE_DOUBLE_LTE(v2, v2);
}

CLOVE_TEST(StringEq) {
    CLOVE_STRING_EQ("a", "a");
    //CLOVE_STRING_EQ("a", "b");
}

CLOVE_TEST(StringNe) {
    CLOVE_STRING_NE("a", "b");
    //CLOVE_STRING_NE("a", "a");
}

CLOVE_TEST(PtrComparison) {
    const void* p1 = (void*)1;
    const void* p2 = (void*)2;

    CLOVE_PTR_EQ(p1, p1);
    CLOVE_PTR_NE(p1, p2);
    
    CLOVE_PTR_GT(p2, p1);
    CLOVE_PTR_GTE(p2, p1);
    CLOVE_PTR_GTE(p2, p2);
    
    CLOVE_PTR_LT(p1, p2);
    CLOVE_PTR_LTE(p1, p2);
    CLOVE_PTR_LTE(p2, p2);
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
