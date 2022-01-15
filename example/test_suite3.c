#include "clove.h"

int* count;
void my_setup() {
    count = (int*)calloc(1, sizeof(int));
}
void my_teardown() {
    free(count);
}

CLOVE_TEST(CountTest1) {
    (*count)++;
    CLOVE_INT_EQ(1, *count);
}

CLOVE_TEST(CountTest2) {
    (*count)++;
    CLOVE_INT_EQ(1, *count);
}

CLOVE_SUITE(TestSuite3) {
    CLOVE_SUITE_SETUP(my_setup);
    CLOVE_SUITE_TEARDOWN(my_teardown);
    CLOVE_SUITE_TESTS(
        CountTest1, CountTest2
    );
}