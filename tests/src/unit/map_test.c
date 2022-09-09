#define CLOVE_SUITE_NAME UNIT_MapTest
#include "clove-unit.h"
#include <string.h>

CLOVE_TEST(MapInit) {
    __clove_map_t map;
   __clove_map_init(&map);
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
   __clove_map_init(&map);
   
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