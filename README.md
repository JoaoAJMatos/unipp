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

void computation() {
    int a = 2;
    for (int i = 0; i < 1000000; i++) {
        a *= 2;
    }
}

void test_function() {
    int a = 2;
    int b = 3;

    // This will trigger a warning if
    // failed
    EXPECT_LESS(
        BENCHMARK(computation, 1000).total,
        MILLISECONDS(20),
        "Expected benchmark to take less than 20 ms to execute 20 times"
    );

    // This will fail the test and return
    // if failed
    ASSERT_GREATER(a + b, 10, "Expected a + b to be greater than 10");
}

int main(void) {
    RUN(
        TEST("Test", "Test Description", test_function)
    );
}
```

> Output:

```bash
[TEST] Running test: Test
[+] Description: Test Description
      [!] WARNING: Expected benchmark to take less than 20 ms to execute 20 times
      [X] FAILED: Expected a + b to be greater than 10
```

## Assertion Macros

Unipp provides a set of macros to control the behaviour of the tests. These macros are:

- `ASSERT(a)`
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

When an assert macro fails, the test function will return immediately and the test will be marked as failed:

```cpp
void test_function() {
    ASSERT_EQUAL(1, 2, "Expected 1 to be equal to 2"); // This will fail
    ASSERT_EQUAL(2, 2, "Expected 2 to be equal to 2"); // This will not be executed
}
```

## Tests

Tests are defined using the `TEST(name, description, function)` macro.

```cpp
TEST("Test", "Test Description", test_function);
```

## Test Suites

You can also group tests into test suites in order to organize your tests. Test suites are defined using the `SUITE(name, description, tests...)` macro. Where `tests...` is a list of tests defined using the `TEST(name, description, function)` macro.

```cpp
SUITE("Test Suite", "Test Suite Description",
    TEST("Test 1", "Test Description", test_function),
    TEST("Test 2", "Test Description", test_function)
);
```

## Benchmarking

Unipp also provides a simple benchmarking API. You can define benchmarks using the `BENCHMARK(function, iterations)` macro, which will return a `BenchmarkResult` object containing the average time it took to run the function `iterations` times and the total execution time.

```cpp
void benchmark_function() {
    // Do something
}

int main(void) {
    unipp::BenchmarkResult result = BENCHMARK(benchmark_function, 1000);
    result.Show();
}
```

> Output:

```bash
[BENCHMARK] Total time: 0.000000 ms
[BENCHMARK] Average time: 0.000000 ms
```

You can use this in tests as well, as described in the [examples](examples/) folder.

Unipp provides a set of utility macros for benchmarking:

- `SECONDS_TO_MILLISECONDS(seconds)`
- `MILLISECONDS(ms)`
- `BENCHMARK(function, iterations)`

## Warnings and Expected Values

You can also specify expected values for your test functions and trigger warnings when the expected values are not met. This can be done using the `EXPECT` macros. These macros are:

- `EXPECT(a)`
- `EXPECT_EQUAL(a, b, message)`
- `EXPECT_NOT_EQUAL(a, b, message)`
- `EXPECT_GREATER(a, b, message)`
- `EXPECT_GREATER_EQUAL(a, b, message)`
- `EXPECT_LESS(a, b, message)`
- `EXPECT_LESS_EQUAL(a, b, message)`
- `EXPECT_TRUE(a, message)`
- `EXPECT_FALSE(a, message)`
- `EXPECT_NULL(a, message)`
- `EXPECT_NOT_NULL(a, message)`

Like so:

```cpp
EXPECT_LESS(
    BENCHMARK(benchmark_function, 1000).total,
    20,
    "Expected benchmark to take less than 20 ms"
);
```

Expect macros will not fail the test, meaning that the test will continue to run even if the expected value is not met.

## Examples

For some examples on how to use **unipp**, check out the [examples](examples/) folder.
