# Unipp

Unipp is a simple single-header self-contained unit testing and benchmarking framework for C++.

## Features

- Test definition
- Behaviour controll through macros
- Test suites
- Benchmarking

## Usage

In order to use **unipp** in your project, simply **copy and paste** the `unipp.hpp` header file into your project directory and include it:

```cpp
#include "unipp.hpp"

void test_function() {
    int a = 2;
    int b = 2;
    ASSERT_EQUAL(a, b, "Expected a and b to be equal"); // This will pass
    ASSERT_GREATER(a + b, 10, "Expected a + b to be greater than 10"); // This will fail
}

int main(void) {
    RUN(
        TEST("Test", "Test Description", test_function)
    );
}
```

> Output:

```bash
[+] Running individual test: Test
[+] Description: Test Description
   [X] FAILED: Expected a + b to be greater than 10
```

## Macros

Unipp provides a set of macros to control the behaviour of the tests. These macros are:

- `ASSERT_EQUAL(a, b, error_message)`
- `ASSERT_NOT_EQUAL(a, b, message)`
- `ASSERT_GREATER(a, b, message)`
- `ASSERT_GREATER_EQUAL(a, b, message)`
- `ASSERT_LESS(a, b, message)`
- `ASSERT_LESS_EQUAL(a, b, message)`
- `ASSERT_TRUE(a, message)`
- `ASSERT_FALSE(a, message)`
- `ASSERT_NULL(a, message)`
- `ASSERT_NOT_NULL(a, message)`

If fore some reason none of these macros fit your needs, you can always define your own test assertions and call the `PANIC(message)` macro to fail the test and show a _panic message_. Like so:

```cpp
void test_function() {
    if (/** Something happens */) {
        PANIC("Something happened"); // This will fail the test
    }
}
```

## Tests

Tests are defined using the `TEST(name, description, function)` macro.

```cpp
TEST("Test", "Test Description", test_function);
```

## Test Suites

You can also group tests into test suites in order to organize your tests. Default test suites are defined using the `SUITE(name, description, tests...)` macro. Where `tests...` is a list of tests defined using the `TEST(name, description, function)` macro.

```cpp
SUITE("Test Suite", "Test Suite Description",
    TEST("Test 1", "Test Description", test_function),
    TEST("Test 2", "Test Description", test_function)
);
```

## Examples

For some examples on how to use **unipp**, check out the [examples](examples/) folder.
