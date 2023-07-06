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
#define VOID []() -> void {}

/** Macros for creating and running tests */
#define TEST(name, description, testfunction) unipp::UnitTest(name, description, testfunction)

#define SUITE(name, description, ...) \
      unipp::TestSuite(name, description, unipp::SetupAndTeardownType::NONE, __VA_ARGS__)

#define CUSTOM_SUITE(custom_suite_class, name, description, setup_type, ...) \
      custom_suite_class(name, description, setup_type, __VA_ARGS__)

#define DEFAULT_CONSTRUCTOR(custom_suite_name) \
      template<typename... Tests> \
      custom_suite_name(std::string name, std::string description, \
            unipp::SetupAndTeardownType setup_type = unipp::SetupAndTeardownType::PER_TEST, Tests... tests) \
            : unipp::TestSuite(name, description, setup_type, tests...) { }

#define RUN(...) unipp::TestRunner::RunAll(__VA_ARGS__)


/** Macros for testing assertions */
/** These stand out in your testing code */
#define PANIC(msg) throw std::runtime_error(msg)
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


/** Helper Macros for code readability */
#define RUN_SETUP_BEFORE_SUITE_IF_NEEDED() if (setup_type_ == SetupAndTeardownType::PER_SUITE) Setup()
#define RUN_TEARDOWN_AFTER_SUITE_IF_NEEDED() if (setup_type_ == SetupAndTeardownType::PER_SUITE) Teardown()
#define RUN_SETUP_BEFORE_TEST_IF_NEEDED() if (setup_type_ == SetupAndTeardownType::PER_TEST) Setup()
#define RUN_TEARDOWN_AFTER_TEST_IF_NEEDED() if (setup_type_ == SetupAndTeardownType::PER_TEST) Teardown()



namespace unipp
{
      /** Type definitions */
      typedef std::function<void()> TestFunction;


      /** 
       * Setup and teardown types.
       *
       * If the setup and teardown procedures are expensive, it is possible to
       * perform them only once before and after the suite. Keep in mind that the
       * tests in the suite should not depend on each other. Since one test case
       * may inadvertently change the state of the system, causing the next test
       * case to fail, leading to unexpected results and making it difficult to
       * isolate and diagnose issues within individual test cases (check docs).
       */
      enum SetupAndTeardownType
      {
            NONE,
            PER_TEST,
            PER_SUITE
      };


      /** 
       * @brief UnitTest struct.
       *        Represents a unit test
       */
      struct UnitTest
      {
            std::string name;         // Name of the test
            std::string description;  // Description of the test
            TestFunction test;        // The function that performs the test

            UnitTest(std::string name, std::string description, TestFunction test)
                  : name(name), description(description), test(test) {}
      };


      
      class TestSuite
      {
      public:
            /**
             * @brief Construct a new Test Suite object
             *        This constructor accepts an infinite number of tests.
             *        Useful for creatig suites in a readable format using the SUITE macro:
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
             * @param setup_type
             * @param tests
             */
            template<typename... Tests>
            TestSuite(std::string name, std::string description, 
                      SetupAndTeardownType setup_type = SetupAndTeardownType::PER_TEST, Tests... tests)
                      : name_(name), description_(description), setup_type_(setup_type)
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
             *
             *        Setup and teardown behavior is controlled by the setup_type.
             *
             *        The setup type indicates if the setup and teardown procedures
             *        are performed before and after each test or only once before
             *        and after the suite.
             *
             *        This may reduce overhead if the setup and teardown procedures
             *        are expensive, but can lead to unexpected results if the tests
             *        in the suite depend on each other.
             *
             *        Check the SetupAndTeardownType enum for more information.
             */
            void Run()
            {
                  std::cout << "[SUITE | " << this->name_ << " | " << this->description_ << "]" << std::endl;

                  RUN_SETUP_BEFORE_SUITE_IF_NEEDED();
                  for (auto test : tests_) {
                        std::cout << "   [+] Running test: " << test.name << std::endl;
                        std::cout << "   [+] Description: " << test.description << std::endl;

                        try {
                              RUN_SETUP_BEFORE_TEST_IF_NEEDED();
                              test.test();
                              RUN_TEARDOWN_AFTER_TEST_IF_NEEDED();
                              std::cout << "      [√] PASSED!" << std::endl << std::endl;
                        }
                        catch (const std::exception& e) {
                              std::cout << "      [X] FAILED: " << e.what() << std::endl << std::endl;
                        }
                  }
                  RUN_TEARDOWN_AFTER_SUITE_IF_NEEDED();
            }
            
            /** Virtual Setup and Teardown functions */
            virtual void Setup() {}
            virtual void Teardown() {}
      private:
            std::string name_;
            std::string description_;
            std::vector<UnitTest> tests_;

            SetupAndTeardownType setup_type_;
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
             * @brief Run all the test suites.
             *        This function accepts an infinite number of test suites.
             *        Useful for running all the test suites in a readable format using the RUN macro:
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
             * @brief Run all the individual tests.
             *        This function accepts an infinite number of tests.
             *        Useful for running all the tests in a readable format using the RUN_ALL macro:
             *
             *        RUN_ALL(
             *              TEST("My Test", "This is a test description", []() { ... }),
             *              TEST("My Test 2", "This is a test description", []() { ... })
             *        );
             *
             *
             * @tparam Tests
             * @param suites
             */
            template<typename... Tests>
            static void RunAll(UnitTest test, Tests... tests)
            {
                  RunTest(test);
                  RunAll(tests...);
            }
            
      private:
            TestRunner() {}

            static void RunTest(UnitTest test)
            {
                  std::cout << "[+] Running individual test: " << test.name << std::endl;
                  std::cout << "[+] Description: " << test.description << std::endl;

                  try {
                        test.test();
                        std::cout << "   [√] PASSED!" << std::endl << std::endl;
                  }
                  catch (const std::exception& e) {
                        std::cout << "   [X] FAILED: " << e.what() << std::endl << std::endl;
                  }
            }
      };
}


#endif // UNIPP_TEST_FRAMEWORK_HPP
