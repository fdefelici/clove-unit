#define CLOVE_SUITE_NAME StackTest
#include "clove.h"

CLOVE_TEST(StackInitialization) {
    __clove_stack_t stack;
    __clove_stack_init(&stack, 2);

    CLOVE_ULLONG_EQ(2, stack.capacity);
    CLOVE_ULLONG_EQ(sizeof(size_t), stack.item_size);
    CLOVE_ULLONG_EQ(0, stack.count);
    CLOVE_NOT_NULL(stack.items);
}

CLOVE_TEST(StackIsEmptyAfterInitialization) {
    __clove_stack_t stack;
    __clove_stack_init(&stack, 2);

    CLOVE_IS_TRUE(__clove_stack_is_empty(&stack));
}

CLOVE_TEST(PushItemsOverTheCapacity) {
    __clove_stack_t stack;
    __clove_stack_init(&stack, 2);

    __clove_stack_push(&stack, 10);
    __clove_stack_push(&stack, 20);
    CLOVE_ULLONG_EQ(2, stack.capacity);
    CLOVE_ULLONG_EQ(2, stack.count);
    
    __clove_stack_push(&stack, 30);
    CLOVE_ULLONG_EQ(4, stack.capacity);
    CLOVE_ULLONG_EQ(3, stack.count);

    size_t* tail = (size_t*)stack.items;
    CLOVE_ULLONG_EQ(10, *(tail + 0));
    CLOVE_ULLONG_EQ(20, *(tail + 1));
    CLOVE_ULLONG_EQ(30, *(tail + 2));
}

CLOVE_TEST(PopItems) {
    __clove_stack_t stack;
    __clove_stack_init(&stack, 2);

    __clove_stack_push(&stack, 10);
    __clove_stack_push(&stack, 20);
    
    CLOVE_ULLONG_EQ(20, __clove_stack_pop(&stack));
    CLOVE_ULLONG_EQ(10, __clove_stack_pop(&stack));
    CLOVE_IS_TRUE(__clove_stack_is_empty(&stack));

    //poping over 0  
    CLOVE_ULLONG_EQ(18446744073709551615ULL, __clove_stack_pop(&stack));
}