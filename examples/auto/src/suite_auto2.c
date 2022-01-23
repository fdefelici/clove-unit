#define CLOVE_ENABLE_AUTODISCOVERY
#include "clove.h"

#define CLOVE_SUITE_NAME MySecondSuite


CLOVE_TEST(FailingTest) {
    int a = 1;
    int b = 2;
    CLOVE_INT_EQ(a, b);
}

