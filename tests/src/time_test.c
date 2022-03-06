#define CLOVE_SUITE_NAME TimeTest
#include "clove.h"


CLOVE_TEST(TimeElapsedAfterSleep1Second) {
    __clove_time_t start = __clove_time_now();

    int sleepSeconds = 1;

    #ifdef _WIN32
        Sleep(sleepSeconds * 1000); //Millisecond on Windows
    #else
        sleep(sleepSeconds); //Seconds on Unix
    #endif
    
    __clove_time_t end = __clove_time_now();

   __clove_time_t diff = __clove_time_sub(&end, &start);
   CLOVE_INT_EQ(1, diff.seconds);
   CLOVE_IS_TRUE( diff.nanos_after_seconds >= 0);
}   
