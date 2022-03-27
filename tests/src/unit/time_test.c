#define CLOVE_SUITE_NAME TimeTest
#include "clove-unit.h"

#ifdef _WIN32
    #include <windows.h>
    #define sleep_secs(sec) Sleep(sec * 1000)
#else
    #include <unistd.h>
    #define sleep_secs(sec) sleep(sec)
#endif
CLOVE_TEST(TimeElapsedAfterSleep1Second) {
    __clove_time_t start = __clove_time_now();

    sleep_secs(1);
    
    __clove_time_t end = __clove_time_now();

   __clove_time_t diff = __clove_time_sub(&end, &start);
   CLOVE_INT_EQ(1, diff.seconds);
   CLOVE_IS_TRUE( diff.nanos_after_seconds >= 0);
}   
