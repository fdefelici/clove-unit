#define CLOVE_SUITE_NAME TimeTest
#include "clove.h"

CLOVE_TEST(TimeElapsedAfterSleep1Second) {
    __clove_time_t start = __clove_time_now();

    //TODO: Generalizzare Sleep
    //Sleep(1000); //1s  NOTA: Sleep su windows, mentre sleep su unix

    __clove_time_t end = __clove_time_now();

   __clove_time_t diff = __clove_time_sub(&end, &start);
   CLOVE_INT_EQ(1, diff.seconds);
   CLOVE_IS_TRUE( diff.nanos_after_seconds >= 0);
}   
