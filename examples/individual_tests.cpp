#include "unipp.hpp"

// Define your test function
void test1()
{
      int a = 2;
      int b = 2;
      ASSERT_NOT_EQUAL(a, b, "Expected a to be equal to b");          // This will pass
      ASSERT_GREATER(a + b, 10, "Expected a + b to be less than 10"); // This will fail
}

// Run an individual test
int main(void)
{
      RUN(
            TEST("Test 1", "Test 1 description", test1)
      );
}
