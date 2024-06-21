# CLove-Unit &middot; [![version](https://img.shields.io/github/v/release/fdefelici/clove-unit?label=latest&sort=semver)](./clove-unit.h) [![workflow](https://img.shields.io/github/actions/workflow/status/fdefelici/clove-unit/ci_action.yml)](https://github.com/fdefelici/clove-unit/actions/workflows/ci_action.yml) [![conan](https://img.shields.io/conan/v/clove-unit)](https://conan.io/center/recipes/clove-unit) [![Discord channel](https://img.shields.io/discord/1167864219190964255?logo=discord&logoColor=violet)](https://discord.gg/Mjx4YRQfFt)

`CLove-Unit` is a single-header unit testing library designed for C (compatible with C++).

This library can automatically detect and execute your tests, providing comprehensive details about successes and failures, such as file, line number,
and reason for failure. Additionally, it offers a variety of output formats thanks to a powerful command-line API.

`CLove-Unit` allows you to focus solely on creating unit tests, eliminating the need to manually register tests or create custom assertions.

Here is an example of the default report format, which features pretty printing with ANSI support:
![Clove test run result](./examples/result.png)


> Join the [**discord server**](https://discord.gg/Mjx4YRQfFt) to stay tuned for updates or to make questions that doesn't require to open an issue. <br/>
Consider also supporting `CLove-Unit` development becoming a [**sponsor**](https://github.com/users/fdefelici/sponsorship).


## Table of Contents

* [Features](#features)
* [IDE Extensions](#ide-extensions)
* [How to Integrate](#how-to-integrate)
* [How It Works](#how-it-works)
* [Getting Started](#getting-started)
* [Programming API](#programming-api)
    * [Library Configuration](#library-configuration)
    * [Test Definitions](#test-definitions)
    * [Test Assertions](#test-assertions)
    * [Test Helpers](#test-helpers)
* [Command-Line API](#command-line-api)

## Features

- **Single Header**: Simply include [clove-unit.h](./clove-unit.h) to integrate it into your project.
- **Tests Auto-discovery**: Simplify test writing with the `CLOVE_TEST` macro for automatic registration.
- **Test Report**: Report format in ANSI, CSV, or JSON. Customize format and output with command-line args.
- **Tests/Run Duration**: Measure and report test execution times.
- **Tests/Run Failure**: Get file, line, and failure info with the assertion macros.
- **Selective Test Execution**: Include or exclude tests for each run.
- **Command-line API**: Create IDE plugins and automate with versatile options.

## IDE Extensions

For those who prefer a UI oriented test executor, `CLove-Unit` is supported on the following IDEs:

* **Visual Studio:** available from [VS Market Place](https://marketplace.visualstudio.com/items?itemName=fdefelici.vs-clove-unit)
* **Visual Studio Code:** available from the [VSCode Market Place](https://marketplace.visualstudio.com/items?itemName=fdefelici.vscode-clove-unit).

Have a look and enjoy ;-)

## How to Integrate

`CLove-Unit` can be imported in your project in the following ways:
- Sourcing the header file
- Using a Package Manager
- Using CMake 

### Sourcing the Header file
Being an header-only library, you can just download [clove-unit.h](./clove-unit.h) file and include it in your project.

```c
#include "clove-unit.h"
```

Then remember to properly configure your compiler include paths.

### Using a Package Manager
`CLove-Unit` is currently available on the following Package Managers:

* [Conan](https://conan.io): read [here](https://conan.io/center/recipes/clove-unit) for details on how to import it.

### Using CMake 
In case you still need dependency management, but you want to avoid Package Manager configuration complexity, you can use standard mechansim provided by `CMake` such as [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html), [add_subdirectory](https://cmake.org/cmake/help/latest/command/add_subdirectory.html) and [find_package](https://cmake.org/cmake/help/latest/command/find_package.html).

> NOTE: CMake library is named `clove-unit`

Here a few examples:

* **FetchContent**

  ```cmake
  cmake_minimum_required(VERSION 3.18)
  project(TestProject C)

  Include(FetchContent)
  FetchContent_Declare(
    clove-unit
    GIT_REPOSITORY https://github.com/fdefelici/clove-unit.git
    GIT_TAG master       # or eventually any branch, tag or commit sha
  )
  FetchContent_MakeAvailable(clove-unit)

  add_executable(tests <YOUR_TEST_FILES>)
  target_link_libraries(tests clove-unit)
  ```

* **add_subdirectory** 

  First download `CLove-Unit` repository and then point properly to it like this:

  ```cmake
  cmake_minimum_required(VERSION 3.18)
  project(TestProject C)

  add_subdirectory(<PATH_TO_CLOVE_UNIT_REPOSITORY>)  

  add_executable(tests <YOUR_TEST_FILES>)
  target_link_libraries(tests clove-unit)
  ```

* **find_package** 

  First download `CLove-Unit` repository and then run cmake install command on it. 

  Package will be installed in at following path: `<CMAKE_INSTALL_PREFIX>/clove-unit/<CLOVE_VERSION>`
  
  Eventually you may want to customize [CMAKE_INSTALL_PREFIX](https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html) variable to override cmake default installation path for packages.

  
  Then use the package as follow:
  ```cmake
  cmake_minimum_required(VERSION 3.18)
  project(TestProject C)

  find_package(clove-unit REQUIRED PATHS <INSTALLATION_PATH>)  

  # or more strict
  # find_package(clove-unit <CLOVE_VERSION> EXACT REQUIRED PATHS <INSTALLATION_PATH>)  

  add_executable(tests <YOUR_TEST_FILES>)
  target_link_libraries(tests clove-unit)
  ```

## How It Works

`CLove-Unit` is built upon these fundamental concepts:

- **Test:** A test is essentially a function that lets you stimulate your code and verify its behavior through assertions.
- **Suite:** A suite is a collection of tests is run. It enables you to define setup and teardown procedures for each test execution or for the
  entire suite.
- **Runner:** The runner facilitates the execution of multiple suites and offers result reporting.

> The test discovery in CLove-Unit works by parsing the symbol table inside the test executable.
>
> Currently, this feature is accessible for the following combinations of OS, architecture, and executable formats:
>
> - Windows / 32-64 bit little-endian / PE (Portable Executable)
> - MacOS / 64 bit little-endian / Mach-o (Mach Object)
> - Linux / 64 bit little-endian / ELF (Executable and Linkable Format)
>
> Additional compatibilities regarding OS, architecture, and formats can be implemented in the future as needed.

## Getting Started

Getting started with CLove-Unit is simple. Just follow these steps:

### 1. Include CLove-Unit

Include the [clove-unit.h](./clove-unit.h) header in your project using one of the methods described in [How to Integrate](#how-to-integrate) section.

### 2. Define a Suite

Before creating test cases, define a suite name. For example:

```c
//file test_suite1.c
#define CLOVE_SUITE_NAME MySuite01
#include "clove-unit.h"

CLOVE_TEST(FirstTest) {
    int a = 1;
    int b = 1;
    CLOVE_INT_EQ(a, b);
}

CLOVE_TEST(SecondTest) {
    CLOVE_IS_TRUE(1);
}
```

### 3. Set up the Runner

A Runner is the entry point of the program that enables the auto-discovery feature by defining `CLOVE_IMPLEMENTATION`
**once** before `clove-unit.h`. This will include the entire library implementation.

```c
//file main.c
#define CLOVE_IMPLEMENTATION
#include "clove-unit.h"

CLOVE_RUNNER()
```

### 4. Build and Run

Build and run the test executable and `CLove-Unit` will do the magic :-)

> Look at the [CLove101](./examples/clove101) project for samples about:
> - Implementing suites and tests.
> - Assertion usage ([test_suite1.c](./examples/clove101/src/test_suite1.c))
> - Suite setup & tear down feature ([test_suite2.c](./examples/clove101/src/test_suite2.c))

## Programming API

### Library Configuration

APIs used for the very basic setup of the library.

| API                    | Description                                                                                          |
|------------------------|------------------------------------------------------------------------------------------------------|
| `CLOVE_IMPLEMENTATION` | Macro used to implement the entire library. <br />It should only be used once before `clove-unit.h`. |
| `CLOVE_RUNNER()`       | Generates program entry point.                                                                       |

### Test Definitions

APIs used for defining suites and tests.

| API                           | Description                                                                             |
|-------------------------------|-----------------------------------------------------------------------------------------|
| `CLOVE_SUITE_NAME`            | Macro used to define a suite name, before including the `clove-unit.h` header.          |
| `CLOVE_SUITE_SETUP_ONCE()`    | Implements a procedure which is executed only once before all the test cases (optional) |
| `CLOVE_SUITE_TEARDOWN_ONCE()` | Implements a procedure which is executed only once after all the test cases (optional)  |
| `CLOVE_SUITE_SETUP()`         | Implements a procedure which is executed before each test case (optional)               |
| `CLOVE_SUITE_TEARDOWN()`      | Implements a procedure which is executed after each test case (optional)                |
| `CLOVE_TEST(name)`            | Define test case named "name"                                                           |

### Test Assertions

Assertions that can be used within a `CLOVE_TEST` definition.

| API                                              | Description                                                                                     |
|--------------------------------------------------|-------------------------------------------------------------------------------------------------|
| `CLOVE_PASS()`                                   | Assertion that always make the test pass.                                                       |
| `CLOVE_FAIL()`                                   | Assertion that always make the test fail.                                                       |
| `CLOVE_IS_TRUE(actual)`                          | Checks if int is not 0.                                                                         |
| `CLOVE_IS_FALSE(actual)`                         | Checks if int is 0.                                                                             |
| `CLOVE_CHAR_EQ(expected, actual)`                | Checks if expected char is equal to actual.                                                     |
| `CLOVE_CHAR_NE(expected, actual)`                | Checks if expected char is not equal to actual.                                                 |
| `CLOVE_CHAR_GT(expected, actual)`                | Checks if expected char is greater than actual.                                                 |
| `CLOVE_CHAR_GTE(expected, actual)`               | Checks if expected char is greater than or equal to actual.                                     |
| `CLOVE_CHAR_LT(expected, actual)`                | Checks if expected char is less than actual.                                                    |
| `CLOVE_CHAR_LTE(expected, actual)`               | Checks if expected char is less than or equal to actual.                                        |
| `CLOVE_INT_EQ(expected, actual)`                 | Checks if expected int is equal to actual.                                                      |
| `CLOVE_INT_NE(expected, actual)`                 | Checks if expected int is not equal to actual.                                                  |
| `CLOVE_INT_GT(expected, actual)`                 | Checks if expected int is greater than actual.                                                  |
| `CLOVE_INT_GTE(expected, actual)`                | Checks if expected int is greater than or equal to actual.                                      |
| `CLOVE_INT_LT(expected, actual)`                 | Checks if expected int is less than actual.                                                     |
| `CLOVE_INT_LTE(expected, actual)`                | Checks if expected int is greater than or equal to actual.                                      |
| `CLOVE_UINT_EQ(expected, actual)`                | Checks if expected unsigned int is equal to actual.                                             |
| `CLOVE_UINT_NE(expected, actual)`                | Checks if expected unsigned int is not equal to actual.                                         |
| `CLOVE_UINT_GT(expected, actual)`                | Checks if expected unsigned int is greater than actual.                                         |
| `CLOVE_UINT_GTE(expected, actual)`               | Checks if expected unsigned int is greater than or equal to actual.                             |
| `CLOVE_UINT_LT(expected, actual)`                | Checks if expected unsigned int is less than actual.                                            |
| `CLOVE_UINT_LTE(expected, actual)`               | Checks if expected unsigned int is less than or equal to actual.                                |
| `CLOVE_LONG_EQ(expected, actual)`                | Checks if expected long is equal to actual.                                                     |
| `CLOVE_LONG_NE(expected, actual)`                | Checks if expected long is not equal to actual.                                                 |
| `CLOVE_LONG_GT(expected, actual)`                | Checks if expected long is greater than actual.                                                 |
| `CLOVE_LONG_GTE(expected, actual)`               | Checks if expected long is greater than or equal to actual.                                     |
| `CLOVE_LONG_LT(expected, actual)`                | Checks if expected long is less than actual.                                                    |
| `CLOVE_LONG_LTE(expected, actual)`               | Checks if expected long is less than or equal to actual.                                        |
| `CLOVE_ULONG_EQ(expected, actual)`               | Checks if expected unsigned long is equal to actual.                                            |
| `CLOVE_ULONG_NE(expected, actual)`               | Checks if expected unsigned long is not equal to actual.                                        |
| `CLOVE_ULONG_GT(expected, actual)`               | Checks if expected unsigned long is greater than actual.                                        |
| `CLOVE_ULONG_GTE(expected, actual)`              | Checks if expected unsigned long is greater than or equal to actual.                            |
| `CLOVE_ULONG_LT(expected, actual)`               | Checks if expected unsigned long is less than actual.                                           |
| `CLOVE_ULONG_LTE(expected, actual)`              | Checks if expected unsigned long is less than or equal to actual.                               |
| `CLOVE_LLONG_EQ(expected, actual)`               | Checks if expected long long is equal to actual.                                                |
| `CLOVE_LLONG_NE(expected, actual)`               | Checks if expected long long is not equal to actual.                                            |
| `CLOVE_LLONG_GT(expected, actual)`               | Checks if expected long long is greater than actual.                                            |
| `CLOVE_LLONG_GTE(expected, actual)`              | Checks if expected long long is greater than or equal to actual.                                |
| `CLOVE_LLONG_LT(expected, actual)`               | Checks if expected long long is less than actual.                                               |
| `CLOVE_LLONG_LTE(expected, actual)`              | Checks if expected long long is less than or equal to actual.                                   |
| `CLOVE_ULLONG_EQ(expected, actual)`              | Checks if expected unsigned long long is equal to actual.                                       |
| `CLOVE_ULLONG_NE(expected, actual)`              | Checks if expected unsigned long long is not equal to actual.                                   |
| `CLOVE_ULLONG_GT(expected, actual)`              | Checks if expected unsigned long long is greater than actual.                                   |
| `CLOVE_ULLONG_GTE(expected, actual)`             | Checks if expected unsigned long long is greater than or equal to actual.                       |
| `CLOVE_ULLONG_LT(expected, actual)`              | Checks if expected unsigned long long is less than actual.                                      |
| `CLOVE_ULLONG_LTE(expected, actual)`             | Checks if expected unsigned long long is less than or equal to actual.                          |
| `CLOVE_SIZET_EQ(expected, actual)`               | Checks if expected size_t is equal to actual.                                                   |
| `CLOVE_SIZET_NE(expected, actual)`               | Checks if expected size_t is not equal to actual.                                               |
| `CLOVE_SIZET_GT(expected, actual)`               | Checks if expected size_t is greater than actual.                                               |
| `CLOVE_SIZET_GTE(expected, actual)`              | Checks if expected size_t is greater than or equal to actual.                                   |
| `CLOVE_SIZET_LT(expected, actual)`               | Checks if expected size_t is less than actual.                                                  |
| `CLOVE_SIZET_LTE(expected, actual)`              | Checks if expected size_t is less than or equal to actual.                                      |
| `CLOVE_FLOAT_EQ(expected, actual)`               | Checks if expected float is equal to actual with default precision to 6 decimals (1E-6).        |
| `CLOVE_FLOAT_NE(expected, actual)`               | Checks if expected float is not equal to actual with default precision to 6 decimals (1E-6).    |
| `CLOVE_FLOAT_EQ_P(expected, actual, precision)`  | CLOVE_FLOAT_EQ variant with custom precision as the number of decimals to take into account.    |
| `CLOVE_FLOAT_NE_P(expected, actual, precision)`  | CLOVE_FLOAT_NE variant with custom precision as the number of decimals to take into account.    |
| `CLOVE_FLOAT_GT(expected, actual)`               | Checks if expected float is greater than actual.                                                |
| `CLOVE_FLOAT_GTE(expected, actual)`              | Checks if expected float is greater than or equal to actual.                                    |
| `CLOVE_FLOAT_LT(expected, actual)`               | Checks if expected float is less than actual.                                                   |
| `CLOVE_FLOAT_LTE(expected, actual)`              | Checks if expected float is greater than or equal to actual.                                    |
| `CLOVE_DOUBLE_EQ(expected, actual)`              | Checks if expected double is equal to actual with default precision to 15 decimals (1E-15).     | 
| `CLOVE_DOUBLE_NE(expected, actual)`              | Checks if expected double is not equal to actual with default precision to 15 decimals (1E-15). | 
| `CLOVE_DOUBLE_EQ_P(expected, actual, precision)` | CLOVE_DOUBLE_EQ variant with custom precision as the number of decimals to take into account.   |
| `CLOVE_DOUBLE_NE_P(expected, actual, precision)` | CLOVE_DOUBLE_NE variant with custom precision as the number of decimals to take into account.   |
| `CLOVE_DOUBLE_GT(expected, actual)`              | Checks if expected double is greater than actual.                                               |
| `CLOVE_DOUBLE_GTE(expected, actual)`             | Checks if expected double is greater than or equal to actual.                                   |
| `CLOVE_DOUBLE_LT(expected, actual)`              | Checks if expected double is less than actual.                                                  |
| `CLOVE_DOUBLE_LTE(expected, actual)`             | Checks if expected double is greater than or equal to actual.                                   |
| `CLOVE_STRING_EQ(expected, actual)`              | Checks equality between strings (null terminated char pointer or array).                        |
| `CLOVE_STRING_NE(expected, actual)`              | Checks inequality between strings (null terminated char pointer or array).                      |
| `CLOVE_NULL(actual)`                             | Checks if actual is NULL.                                                                       |
| `CLOVE_NOT_NULL(actual)`                         | Checks if actual is not NULL.                                                                   |
| `CLOVE_PTR_EQ(expected, actual)`                 | Checks if expected pointer is equal to actual.                                                  |
| `CLOVE_PTR_NE(expected, actual)`                 | Checks if expected pointer is not equal to actual.                                              |
| `CLOVE_PTR_GT(expected, actual)`                 | Checks if expected pointer is greater than actual.                                              |
| `CLOVE_PTR_GTE(expected, actual)`                | Checks if expected pointer is greater than or equal to actual.                                  |
| `CLOVE_PTR_LT(expected, actual)`                 | Checks if expected pointer is less than actual.                                                 |
| `CLOVE_PTR_LTE(expected, actual)`                | Checks if expected pointer is greater than or equal to actual.                                  |

### Test Helpers

Helper APIs to support test implementation.

| API                      | Description                                                               |
|--------------------------|---------------------------------------------------------------------------|
| `CLOVE_EXEC_PATH()`      | Macro to easily retrieve executable absolute path as a `const char*`      |
| `CLOVE_EXEC_BASE_PATH()` | Macro to easily retrieve executable absolute base path as a `const char*` |

## Command-Line API

A binary built with the `CLove-Unit` library supports a number of command line options at startup:

> `<executable> [options]`

| Options                     | Description                                                                                                                                               |
|-----------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------- |
| \<no-option\>               | Running executable with no args will execute tests <br />producing a report in `pretty` format (default).                                                 |
| `-b` `--base-path`          | Base path for test sources. Allow to shorten test file paths when running/listing tests.                                                                  |
| `-d` `--run-detail <level>` | Control Run Tests report detail level in terms of tests listed: '1' (failed), '2' (failed+skipped), '3' (passed+failed+skipped). Default is '3'.          |
| `-e` `--exclude <expr>`     | Exclude tests is run/listed (can be repeated more than once).<br /> [[read here for more details](#test-inclusionexclusion)]                         |
| `-h` `--help`               | Display usage information.                                                                                                                                |
| `-i` `--include <expr>`     | Include tests is run/listed (can be repeated more than once).<br /> [[read here for more details](#test-inclusionexclusion)]                         |
| `-l` `--list-tests`         | List all/matching test cases in `pretty` format (default).<br />Accepts inclusion/exclusion expression.                                                   |
| `-o` `--output <stream>`    | Specify output stream for a report. <br />Possible options: `stdout` (default) or `<file path>`                                                           |
| `-r` `--report <format>`    | Specify report format when running/listing tests. <br />Possible options: `pretty` `csv` `json`                                                           |
| `-t` `--run-tests`          | Execute all/matching test cases (same as \<no-option\> scenario).<br />Accepts inclusion/exclusion expression.                                            |
| `-v` `--version`            | Show CLove-Unit version.                                                                                                                                  |
| `-x` `--error-on-test-fail` | Run Tests process will end with error in case of test failure. <br />Default is to end the process successfully.                                           |

### Test Inclusion/Exclusion

Inclusion/Exclusion options are useful to collect a specific subset of Suites/Tests and can be repeated more than once on the command line.

> NOTE: If both inclusion and exclusion options are provided, the inclusion pattern always wins over the exclusion pattern.

These options allow you to specify an expression that works like this:

* **Basic:** `SuiteName.TestName`
* **Wildcard:** Only `*` is supported, and can be used to implement patterns such as "starts with", "ends with", "contains", "all", for both the
  SuiteName and TestName

For example, the following are valid selection patterns (non-exhaustive list):

| Pattern            | Description                                                                              |
|--------------------|------------------------------------------------------------------------------------------|
| `Suite.Test`       | Match exactly the Suite Name and the Test Name.                                          |
| `Suite`            | Match all tests whose Suite Name `is` "Suite".                                           |
| `Suite*`           | Match all tests whose Suite Name `starts with` "Suite".                                  |
| `*Suite`           | Match all tests whose Suite Name `ends with` "Suite".                                    |
| `*Suite*`          | Match all tests whose Suite Name `contains` "Suite".                                     |
| `Suite.*Test`      | Match all tests whose Suite Name `is` "Suite" and Test Name `ends with` "Test".          |
| `Suite*.*Test`     | Match all tests whose Suite Name `starts with` "Suite" and Test Name `ends with` "Test". |
| `*.*`              | Match all suites and tests.                                                 |
