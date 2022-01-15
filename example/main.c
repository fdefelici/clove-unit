#include "clove.h"
#include "test_suite1.h"
#include "test_suite2.h"

//Forward Declaration example in case one want work with translation unit (saving recompilation time)
//Eventually could be put in related header (in this scenario "test_suite3.h")
CLOVE_SUITE_DECL(TestSuite3);

CLOVE_RUNNER(
   TestSuite1, TestSuite2, TestSuite3
);
