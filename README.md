# Clove
Clove is a unit testing single-header library for C.

Run your tests gathering information about positives and failures (file, line, reason), with a colored syntax (if supported by your shell).

![Clove test run result](./example/result.png)


# Usage
Just add [clove.h](./clove.h) header in your project and starts creating unit test for your code as follow.

Write your test cases, creating one or more files like this:
```c
//file test1.h
#include "clove.h"

CLOVE_TEST(FirstTest) {
    int a = 1;
    int b = 1;
    CLOVE_INT_EQ(a, b);
}

CLOVE_TEST(SecondTest) {
    CLOVE_IS_TRUE(1);
}
```

Than include test cases files in the one that will be the main program and explict test cases to be run:

```c
//file main.c
#include "clove.h"
#include "test1.h"
#include "test2.h"

CLOVE_RUNNER(FirstTest, SecondTest)
```

> For further examples look at the [example](./example) folder

# Apis
Here a list of availables apis
## Test Definition
Apis to be used for defining tests.
| Api | Description |
| ------------- | ------------- |
| CLOVE_RUNNER(...)  | List test cases to run (defined with CLOVE_TEST) and generate program entry point  |
| CLOVE_TEST(Name)  | Define test case named "Name" |

## Test Assertions
Assertions that can be used within a ```CLOVE_TEST```

| Api | Description |
| ------------- | ------------- |
| CLOVE_CHAR_EQ(expected, result)  | Check equality between char |
| CLOVE_CHAR_NE(expected, result)  | Check inequaliy between char |
| CLOVE_INT_EQ(expected, result)  | Check equality between int |
| CLOVE_INT_NE(expected, result)  | Check inequaliy between int |
| CLOVE_IS_TRUE(result)  | Check if int is not 0 |
| CLOVE_IS_FALSE(result)  | Check if int is 0 |
| CLOVE_FLOAT_EQ(expected, result)  | Check equality between float |
| CLOVE_FLOAT_NE(expected, result)  | Check inequaliy between float |
| CLOVE_STRING_EQ(expected, result)  | Check equality between string (null terminated char pointer or array) |
| CLOVE_STRING_NE(expected, result)  | Check inequaliy between string (null terminated char pointer or array) |
| CLOVE_NULL(result)  | Check if result is NULL |
| CLOVE_NOT_NULL(result)  | Check if result is not NULL |
| CLOVE_PTR_EQ(expected, result)  | Check equality between pointers |
| CLOVE_PTR_NE(expected, result)  | Check inequaliy between pointers |


## Test Helper
Helper apis to support test implementation
| Api | Description |
| ------------- | ------------- |
| CLOVE_EXEC_PATH  | Macro to easily retrive executable path |
| CLOVE_EXEC_BASE_PATH  | Macro to easily retrive executable base path |
