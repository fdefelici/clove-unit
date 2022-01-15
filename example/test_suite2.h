#include "clove.h"

CLOVE_TEST(BooleanTest) {
    CLOVE_IS_TRUE(1);
}

CLOVE_TEST(CharTest) {
    CLOVE_CHAR_EQ('a', 'a');
    CLOVE_CHAR_NE('a', 'b');
}

CLOVE_TEST(IntTest) {
    CLOVE_INT_EQ(2-1, 1);
}

CLOVE_TEST(UIntTest) {
    unsigned int value = 1;
    CLOVE_UINT_EQ(value, 1u);
}

CLOVE_TEST(LongfamilyTest) {
    long l = 1;
    long long ll = 1;
    unsigned long ul = 1;
    unsigned long long ull = 1;
    
    CLOVE_LONG_EQ(l, 1L);
    CLOVE_LLONG_EQ(ll, 1LL);
    CLOVE_ULONG_EQ(ul, 1UL);
    CLOVE_ULLONG_EQ(ull, 1ULL);
}

CLOVE_TEST(FloatTest) {
    CLOVE_FLOAT_NE(1.0f, 1.0001f);
    CLOVE_FLOAT_EQ(1.0f, 1.000001f); //Two float are equals if their diff is <= 0.000001f 
}

CLOVE_TEST(DoubleTest) {
    CLOVE_DOUBLE_NE(1.0, 1.0001);
    CLOVE_DOUBLE_EQ(1.0, 1.000001); //Two double are equals if their diff is <= 0.000001f 
}

CLOVE_TEST(StringTest) {
    CLOVE_STRING_NE("123", "1234");
    CLOVE_STRING_EQ("123", "123");

    const char array[] = {'1', '2', '3', '\0'};
    CLOVE_STRING_EQ(array, "123");
}

CLOVE_TEST(PassTest) {
    CLOVE_PASS();
}

CLOVE_TEST(FailTest) {
    CLOVE_FAIL();
}

CLOVE_SUITE(TestSuite2) {
    CLOVE_SUITE_TESTS(
        BooleanTest, CharTest, 
        IntTest, UIntTest, LongfamilyTest,
        FloatTest, DoubleTest, StringTest, 
        PassTest, FailTest
    );
}