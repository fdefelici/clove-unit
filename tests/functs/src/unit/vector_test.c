#define CLOVE_SUITE_NAME UNIT_VectorTest
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

    char** v1 = (char**)__clove_vector_get(&vector, 0);
    CLOVE_STRING_EQ("Hello", *v1);
}


CLOVE_TEST(NullVectorFree) {
    __clove_vector_t vector = __clove_vector_null();
    __clove_vector_free(&vector);

    CLOVE_PASS();
}

CLOVE_TEST(VectorAddAllWithoutExtendingCapacity) {    
    __clove_vector_t vector1;
    __CLOVE_VECTOR_INIT_CAPACITY(&vector1, int, 10);
    __CLOVE_VECTOR_ADD(&vector1, int, 1);
    __CLOVE_VECTOR_ADD(&vector1, int, 2);

    __clove_vector_t vector2;
    __CLOVE_VECTOR_INIT(&vector2, int);
    __CLOVE_VECTOR_ADD(&vector2, int, 3);
    __CLOVE_VECTOR_ADD(&vector2, int, 4);
    __CLOVE_VECTOR_ADD(&vector2, int, 5);

    __clove_vector_add_all(&vector1, &vector2);
    CLOVE_INT_EQ(5, __clove_vector_count(&vector1));
    CLOVE_INT_EQ(10, vector1.capacity);
    CLOVE_INT_EQ(1, *(int*)__clove_vector_get(&vector1, 0));
    CLOVE_INT_EQ(2, *(int*)__clove_vector_get(&vector1, 1));
    CLOVE_INT_EQ(3, *(int*)__clove_vector_get(&vector1, 2));
    CLOVE_INT_EQ(4, *(int*)__clove_vector_get(&vector1, 3));
    CLOVE_INT_EQ(5, *(int*)__clove_vector_get(&vector1, 4));
}

CLOVE_TEST(VectorAddAllExpandingCapacity) {    
    __clove_vector_t vector1;
    __CLOVE_VECTOR_INIT_CAPACITY(&vector1, int, 2);
    __CLOVE_VECTOR_ADD(&vector1, int, 1);
    __CLOVE_VECTOR_ADD(&vector1, int, 2);

    __clove_vector_t vector2;
    __CLOVE_VECTOR_INIT(&vector2, int);
    __CLOVE_VECTOR_ADD(&vector2, int, 3);
    __CLOVE_VECTOR_ADD(&vector2, int, 4);
    __CLOVE_VECTOR_ADD(&vector2, int, 5);

    __clove_vector_add_all(&vector1, &vector2);
    CLOVE_INT_EQ(5, __clove_vector_count(&vector1));
    CLOVE_INT_EQ(5, vector1.capacity);
    CLOVE_INT_EQ(1, *(int*)__clove_vector_get(&vector1, 0));
    CLOVE_INT_EQ(2, *(int*)__clove_vector_get(&vector1, 1));
    CLOVE_INT_EQ(3, *(int*)__clove_vector_get(&vector1, 2));
    CLOVE_INT_EQ(4, *(int*)__clove_vector_get(&vector1, 3));
    CLOVE_INT_EQ(5, *(int*)__clove_vector_get(&vector1, 4));
}
