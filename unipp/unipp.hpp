/**
 * @file   test_framework.hpp
 * @author JoaoAJMatos
 *
 * @brief Self-contained single header test framework
 *	    used by skynet to perform unit tests. Check
 *	    
 *
 * @version 0.1
 * @date 2023-02-01
 * @license MIT
 * @copyright Copyright (c) 2023
 */

// TODO: Add support for benchmarking
// TODO: Add different logging levels for failed tests
// TODO: Maybe add support for custom test suites with custom setup and teardown functions

#ifndef UNIPP_TEST_FRAMEWORK_HPP
#define UNIPP_TEST_FRAMEWORK_HPP


/** C++ headers */
#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <memory>

/** MACROS */
#define UNIPP_TEST_FRAMEWORK_VERSION "0.1.0"

/** Macros for creating and running tests */
#define TEST(name, description, testfunction) unipp::UnitTest(name, description, testfunction)
#define SUITE(name, description, ...) unipp::TestSuite(name, description, __VA_ARGS__)
#define RUN(...) unipp::TestRunner::RunAll(__VA_ARGS__)

/** Macros for benchmarking */
#define BENCHMARK(function, iterations) unipp::Benchmark(function, iterations)
#define SECONDS_TO_MILLISECONDS(seconds_count) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(seconds_count)).count()

/** Macros for testing assertions */
/** These stand out in your testing code */
#define PANIC(msg) throw unipp::Fail(msg)
#define WARNING(msg) throw unipp::Warning(msg)
#define ASSERT_EQUAL(a, b, msg) if ((a) != (b)) PANIC(msg)
#define ASSERT_NOT_EQUAL(a, b, msg) if ((a) == (b)) PANIC(msg)
#define ASSERT_GREATER(a, b, msg) if ((a) <= (b)) PANIC(msg)
#define ASSERT_GREATER_EQUAL(a, b, msg) if ((a) < (b)) PANIC(msg)
#define ASSERT_LESS(a, b, msg) if ((a) >= (b)) PANIC(msg)
#define ASSERT_LESS_EQUAL(a, b, msg) if ((a) > (b)) PANIC(msg)
#define ASSERT_TRUE(expr, msg) if (!(expr)) PANIC(msg)
#define ASSERT_FALSE(expr, msg) if ((expr)) PANIC(msg)
#define ASSERT_NULL(expr, msg) if ((expr) != nullptr) PANIC(msg)
#define ASSERT_NOT_NULL(expr, msg) if ((expr) == nullptr) PANIC(msg)
#define EXPECT_EQUAL(a, b, msg) if ((a) != (b)) WARNING(msg)
#define EXPECT_NOT_EQUAL(a, b, msg) if ((a) == (b)) WARNING(msg)
#define EXPECT_GREATER(a, b, msg) if ((a) <= (b)) WARNING(msg)
#define EXPECT_GREATER_EQUAL(a, b, msg) if ((a) < (b)) WARNING(msg)
#define EXPECT_LESS(a, b, msg) if ((a) >= (b)) WARNING(msg)
#define EXPECT_LESS_EQUAL(a, b, msg) if ((a) > (b)) WARNING(msg)
#define EXPECT_TRUE(expr, msg) if (!(expr)) WARNING(msg)
#define EXPECT_FALSE(expr, msg) if ((expr)) WARNING(msg)
#define EXPECT_NULL(expr, msg) if ((expr) != nullptr) WARNING(msg)
#define EXPECT_NOT_NULL(expr, msg) if ((expr) == nullptr) WARNING(msg)


namespace unipp
{
      /** Type definitions */
      typedef std::function<void()> TestFunction;

      /** Exceptions */
      class Warning : public std::exception
      {
      public:
            Warning(std::string msg) : msg(msg) {}
            const char* what() const throw() { return msg.c_str(); }
      private:
            std::string msg;
      };

      class Fail : public std::exception
      {
      public:
            Fail(std::string msg) : msg(msg) {}
            const char* what() const throw() { return msg.c_str(); }
      private:
            std::string msg;
      };

      /** Structs */
      typedef struct {
            long long total;
            long long average;
      } BenchmarkResult;

      /**
       * @brief Defines a unit test
       */
      struct UnitTest
      {
            std::string name;         
            std::string description;
            TestFunction test;

            UnitTest(std::string name, std::string description, TestFunction test)
                  : name(name), description(description), test(test) {}

            /**
             * @brief Runs the test
             */
            void Run()
            {
                  std::cout << "   [+] Running test: " << name << std::endl;
                  std::cout << "   [+] Description: " << description << std::endl;

                  try {
                        test();
                        std::cout << "      [√] PASSED!" << std::endl << std::endl;
                  }
                  catch (Warning& e) {
                        std::cout << "      [!] WARNING: " << e.what() << std::endl << std::endl;
                  }
                  catch (Fail& e) {
                        std::cout << "      [X] FAILED: " << e.what() << std::endl << std::endl;
                  }
            }
      };


      /** Classes */
      class TestSuite
      {
      public:
            /**
             * @brief Construct a new Test Suite object
             *
             *        SUITE("My Suite", "This is a suite description", PER_TEST,
             *              TEST("My Test", "This is a test description", []() { ... }),
             *              TEST("My Test 2", "This is a test description", []() { ... }),
             *              TEST("My Test 3", "This is a test description", []() { ... })
             *        );
             *
             * @tparam Tests
             * @param name
             * @param description
             * @param tests
             */
            template<typename... Tests>
            TestSuite(std::string name, std::string description, Tests... tests)
                      : name_(name), description_(description)
            {
                  AddTests(tests...);
            }


            /**
             * @brief Adds tests to the suite.
             */
            template<typename... Tests>
            void AddTests(Tests... tests)
            {
                  (tests_.push_back(tests), ...);
            }


            /**
             * @brief Run the tests in the suite.
             */
            void Run()
            {
                  std::cout << "[SUITE | " << this->name_ << " | " << this->description_ << "]" << std::endl;
                  for (auto test : tests_) {
                        test.Run();
                  }
                  std::cout << "[END SUITE]" << std::endl << std::endl;
            }
      private:
            std::string name_;
            std::string description_;
            std::vector<UnitTest> tests_;
      };


      /**
       * @brief TestRunner class.
       *        Runs the tests in the test suites.
       */
      class TestRunner
      {
      public:
            /** Base case for the recursive variadic template */
            static void RunAll() { }

            /**
             * @brief Variadic template for running SUITES.
             *
             *        RUN(
             *              SUITE("My Suite", "This is a suite description",
             *                    TEST("My Test", "This is a test description", []() { ... }),
             *                    TEST("My Test 2", "This is a test description", []() { ... }),
             *                    TEST("My Test 3", "This is a test description", []() { ... })
             *              ),
             *              SUITE("My Suite 2", "This is a suite description",
             *                    TEST("My Test", "This is a test description", []() { ... }),
             *                    TEST("My Test 2", "This is a test description", []() { ... }),
             *                    TEST("My Test 3", "This is a test description", []() { ... })
             *              )
             *        );
             *
             * @tparam TestSuites
             * @param suites
             */
            template<typename... TestSuites>
            static void RunAll(TestSuite suite, TestSuites... suites)
            {
                  suite.Run();
                  RunAll(suites...);
            }

            /**
             * @brief Variadic template for running TESTS.
             *
             *        RUN(
             *              TEST("My Test", "This is a test description", []() { ... }),
             *              TEST("My Test 2", "This is a test description", []() { ... })
             *        );
             *
             * @tparam Tests
             * @param suites
             */
            template<typename... Tests>
            static void RunAll(UnitTest test, Tests... tests)
            {
                  test.Run();
                  RunAll(tests...);
            }
            
      private:
            TestRunner() {}
      };


      /**
       * @brief Benchmark a function.
       *        Returns the average execution time of the function.
       *
       * @param function
       * @param iterations
       * @return BenchmarkResult
       */
      BenchmarkResult Benchmark(TestFunction test, int iterations)
      {
            BenchmarkResult result;
            std::chrono::milliseconds total_time = std::chrono::milliseconds(0);

            for (int i = 0; i < iterations; i++) {
                  auto start = std::chrono::high_resolution_clock::now();
                  test();
                  auto end = std::chrono::high_resolution_clock::now();
                  total_time += std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            }

            result.total = total_time.count();
            result.average = total_time.count() / iterations;
            return result;
      }
}


#endif // UNIPP_TEST_FRAMEWORK_HPP

// MIT License
// 
// Copyright (c) 2023 João Matos
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
