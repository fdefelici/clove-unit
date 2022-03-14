#define CLOVE_SUITE_NAME MySuite02
#include "clove-unit.h"

int setup_once_count = 0;
int teardown_once_count = 0;
int setup_count = 0;
int teardown_count = 0;

CLOVE_SUITE_SETUP_ONCE() {
    setup_once_count++;
}

CLOVE_SUITE_TEARDOWN_ONCE() {
    teardown_once_count++;
}

CLOVE_SUITE_SETUP() {
   setup_count++;
}

CLOVE_SUITE_TEARDOWN() {
    teardown_count++;
}

CLOVE_TEST(A_FirstTest) {
    CLOVE_INT_EQ(1, setup_once_count);
    CLOVE_INT_EQ(0, teardown_once_count);
    CLOVE_INT_EQ(1, setup_count);
    CLOVE_INT_EQ(0, teardown_count);
}

CLOVE_TEST(B_SecondTest) {
    CLOVE_INT_EQ(1, setup_once_count);
    CLOVE_INT_EQ(0, teardown_once_count);
    CLOVE_INT_EQ(2, setup_count);
    CLOVE_INT_EQ(1, teardown_count);
}
