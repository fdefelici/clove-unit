#define CLOVE_SUITE_NAME UNIT_MapTest
#include "clove-unit.h"
#include <string.h>

CLOVE_TEST(MapInit) {
    __clove_map_t map;
   __CLOVE_MAP_INIT(&map);
   CLOVE_INT_EQ(0, (int)map.count);
   CLOVE_NOT_NULL(map.hash_funct);
   CLOVE_NOT_NULL(map.hashmap);
   CLOVE_INT_EQ(10, (int)map.hashmap_size);
   
   __clove_map_free(&map);
   CLOVE_INT_EQ(0, (int)map.count);
   CLOVE_NOT_NULL(map.hash_funct);
   CLOVE_NULL(map.hashmap);
   CLOVE_INT_EQ(0, (int)map.hashmap_size);
}

CLOVE_TEST(MapPut) {
    __clove_map_t map;
   __CLOVE_MAP_INIT(&map);
   
   int value1 = 1;
   int value2 = 2;

   __clove_map_put(&map, "one", &value1);
   __clove_map_put(&map, "two", &value2);
    CLOVE_INT_EQ(2, (int)__clove_map_count(&map));

    int* value1_ptr = __clove_map_get(&map, "one");
    CLOVE_INT_EQ(1, *value1_ptr);
    int* value2_ptr = __clove_map_get(&map, "two");
    CLOVE_INT_EQ(2, *value2_ptr);

   __clove_map_free(&map);
}

static int dtor_call_count = 0;
static void item_dtor(void* item) {
    dtor_call_count++;
}
CLOVE_TEST(FreeWithCustomDtorWhenNoCollision) {
    dtor_call_count = 0;

    __clove_map_t map;
    __clove_map_params_t params = __clove_map_params_defaulted();
    params.item_dtor = item_dtor;
    __clove_map_init(&map, &params);
    
    int* value1 = __CLOVE_MEMORY_MALLOC_TYPE(int);
    int* value2 = __CLOVE_MEMORY_MALLOC_TYPE(int);
    __clove_map_put(&map, "one", value1);
    __clove_map_put(&map, "two", value2);
    __clove_map_free(&map);

    CLOVE_INT_EQ(2, dtor_call_count);
}

size_t const_hash_funct(void *key, size_t keylen) {
    return 0;
}

CLOVE_TEST(FreeWithCustomDtorWithCollision) {
    dtor_call_count = 0;

    __clove_map_t map;
    __clove_map_params_t params = __clove_map_params_defaulted();
    params.hash_funct = const_hash_funct;
    params.item_dtor = item_dtor;
    __clove_map_init(&map, &params);
    
    int* value1 = __CLOVE_MEMORY_MALLOC_TYPE(int);
    int* value2 = __CLOVE_MEMORY_MALLOC_TYPE(int);
    __clove_map_put(&map, "one", value1);
    __clove_map_put(&map, "two", value2);
    __clove_map_free(&map);

    CLOVE_INT_EQ(2, dtor_call_count);
}