#define CLOVE_SUITE_NAME VectorTest
#include "clove-unit.h"

static int ints_comparator(void* v1ptr, void* v2ptr) {
    int v1 = *(int*)(v1ptr);
    int v2 = *(int*)(v2ptr); 
    return v1 - v2;
}

CLOVE_TEST(SortEmpty) {
    __clove_vector_t vector;
    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(int));
    __clove_vector_init(&vector, &params);
    __clove_vector_sort(&vector, ints_comparator);

    CLOVE_PASS();
}

CLOVE_TEST(SortOneItem) {
    __clove_vector_t vector;
    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(int));
    __clove_vector_init(&vector, &params);

    int* slot = (int*) __clove_vector_add_slot(&vector);
    *slot = 10;

    __clove_vector_sort(&vector, ints_comparator);

    int first = *(int*)__clove_vector_get(&vector, 0);
    CLOVE_INT_EQ(10, first);
}

CLOVE_TEST(SortTwoItemsAlreadySorted) {
    __clove_vector_t vector;
    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(int));
    __clove_vector_init(&vector, &params);

    int* slot;
    slot = (int*) __clove_vector_add_slot(&vector);
    *slot = 10;

    slot = (int*) __clove_vector_add_slot(&vector);
    *slot = 20;

    __clove_vector_sort(&vector, ints_comparator);

    int v1 = *(int*)__clove_vector_get(&vector, 0);
    int v2 = *(int*)__clove_vector_get(&vector, 1);
    CLOVE_INT_EQ(10, v1);
    CLOVE_INT_EQ(20, v2);
}

CLOVE_TEST(SortTwoItemsNotAlreadySorted) {
    __clove_vector_t vector;
    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(int));
    __clove_vector_init(&vector, &params);

    int* slot;
    slot = (int*) __clove_vector_add_slot(&vector);
    *slot = 20;

    slot = (int*) __clove_vector_add_slot(&vector);
    *slot = 10;

    __clove_vector_sort(&vector, ints_comparator);

    int v1 = *(int*)__clove_vector_get(&vector, 0);
    int v2 = *(int*)__clove_vector_get(&vector, 1);
    CLOVE_INT_EQ(10, v1);
    CLOVE_INT_EQ(20, v2);
}


CLOVE_TEST(VectorOfStrings) {
    __clove_vector_t vector;
    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(char*));
    __clove_vector_init(&vector, &params);

    char* str = "Hello";

    char** slot = (char**) __clove_vector_add_slot(&vector);
    *slot = str;

    const char** v1 = (char**)__clove_vector_get(&vector, 0);
    CLOVE_STRING_EQ("Hello", *v1);
}

