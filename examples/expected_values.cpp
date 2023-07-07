#include "unipp.hpp"

void benchmark_function() 
{
      for (int i = 0; i < 1000000; i++) 
      {
            int a = i * 2;
      }
}

void test() 
{
      unipp::BenchmarkResult result = BENCHMARK(benchmark_function, 1000);
      EXPECT_LESS(result.total, 20, "Expected benchmark to take less than 20 ms");
      EXPECT_LESS(result.average, 0.02, "Expected benchmark iteration time average to be less than 0.02 ms");
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
