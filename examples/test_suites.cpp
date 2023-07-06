#include "unipp.hpp"

// Define your test functions
void test1()
{
      int a = 2;
      int b = 2;
      ASSERT_NOT_EQUAL(a, b, "Expected a to be equal to b");          // This will pass
      ASSERT_GREATER(a + b, 10, "Expected a + b to be less than 10"); // This will fail
}

void test2()
{
      int a = 1;
      int b = 2;
      ASSERT_EQUAL(a, b, "Expected a to be equal to b"); // This will fail
}

// Group and run your tests in a test suite
int main(void)
{
      RUN(
            SUITE("Test suite 1", "This is a test suite",
                  TEST("Test 1", "This is test 1", test1),
                  TEST("Test 2", "This is test 2", test2)
            )
      );
}
