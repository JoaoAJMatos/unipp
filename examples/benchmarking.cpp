#include "unipp.hpp"

// Computationally intensive function
void benchmark_function() 
{
      for (int i = 0; i < 1000000; i++) 
      {
            int a = i * 2;
      }
}

// Test function
void test() 
{
      // The function should take less than 20 ms to run 1000 times
      ASSERT_LESS(
            BENCHMARK(benchmark_function, 1000).total,
            MILLISECONDS(20),
            "Benchmarking failed"
      );
}

int main() 
{
      RUN(
            SUITE("Benchmarking", "Benchmarking test suite",
                  TEST("Test 1", "Multi 2", test)
            )
      );

      return 0;
}
