/**
 * @file test_interpolation.cc
 * @brief Defines tests that are above the domain of interpolation
 */

#ifdef ARDUINO

#include <Arduino.h>

namespace std {
  void __throw_length_error( char const*e ) {
    Serial.print("Length Error :");
    Serial.println(e);
    while(1);
  }
}

#endif

#include <unity.h>

#include "interpolate.h"

/**
 * @brief Tests for a key above the domain in a small array
 * 
 */
void test_above_domain_1(void) {
  size_t array_size = 3;
  std::pair<double, double> array[] = { {0, 0}, {1, 1}, {2, 2} };
  double result = Interp(array, array_size, 3);
  TEST_ASSERT_EQUAL_FLOAT(2, result);
}

/**
 * Tests for a key above the domain in a large array
 */
void test_above_domain_2(void) {
  size_t array_size = 10;
  std::pair<double, double> array[] = {
    {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
    {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}
  };
  double result = Interp(array, array_size, 11);
  TEST_ASSERT_EQUAL_FLOAT(9, result);
}

/**
 * Tests for a key above the domain in an array of one element
 */
void test_above_domain_3(void) {
  size_t array_size = 1;
  std::pair<double, double> array[] = { {0, 0} };
  double result = Interp(array, array_size, 1);
  TEST_ASSERT_EQUAL_FLOAT(0, result);
}

/**
 * Tests for a key below the domain in a small array
 */
void test_below_domain_1(void) {
  size_t array_size = 3;
  std::pair<double, double> array[] = { {0, 0}, {1, 1}, {2, 2} };
  double result = Interp(array, array_size, -1);
  TEST_ASSERT_EQUAL_FLOAT(0, result);
}

/**
 * Tests for a key below the domain in a large array
 */
void test_below_domain_2(void) {
  size_t array_size = 10;
  std::pair<double, double> array[] = {
    {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
    {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}
  };
  double result = Interp(array, array_size, -1);
  TEST_ASSERT_EQUAL_FLOAT(0, result);
}

/**
 * Tests for a key below the domain in an array of one element
 */
void test_below_domain_3(void) {
  size_t array_size = 1;
  std::pair<double, double> array[] = { {0, 0} };
  double result = Interp(array, array_size, -1);
  TEST_ASSERT_EQUAL_FLOAT(0, result);
}

/**
 * Tests for a key within the domain in a small array on an edge
 */
void test_within_domain_1(void) {
  size_t array_size = 3;
  std::pair<double, double> array[] = { {0, 0}, {1, 1}, {2, 2} };
  double result = Interp(array, array_size, 0);
  TEST_ASSERT_EQUAL_FLOAT(0, result);

  result = Interp(array, array_size, 2);
  TEST_ASSERT_EQUAL_FLOAT(2, result);
}

/**
 * Tests for a key within the domain in a large array on an edge
 */
void test_within_domain_2(void) {
  size_t array_size = 10;
  std::pair<double, double> array[] = {
    {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
    {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}
  };
  double result = Interp(array, array_size, 0);
  TEST_ASSERT_EQUAL_FLOAT(0, result);

  result = Interp(array, array_size, 9);
  TEST_ASSERT_EQUAL_FLOAT(9, result);
}

/**
 * Tests for a key within the domain in an array of one element
 */
void test_within_domain_3(void) {
  size_t array_size = 1;
  std::pair<double, double> array[] = { {0, 0} };
  double result = Interp(array, array_size, 0);
  TEST_ASSERT_EQUAL_FLOAT(0, result);
}

/**
 * Tests for a key in a normal case that requires interpolation with a small
 * array
 */
void test_normal_case_1(void) {
  size_t array_size = 2;
  std::pair<double, double> array[] = { {1, 1}, {2, 2} };
  double result = Interp(array, array_size, 1.5);
  TEST_ASSERT_EQUAL_FLOAT(1.5, result); 
}

/**
 * Tests for a key in a normal case that requires interpolation with a large
 * array
 */
void test_normal_case_2(void) {
  size_t array_size = 10;
  std::pair<double, double> array[] = {
    {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
    {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}
  };
  double result = Interp(array, array_size, 4.5);
  TEST_ASSERT_EQUAL_FLOAT(4.5, result); 
}

/**
 * Portable function to run all the tests
 */
void run_tests() {
  UNITY_BEGIN();

  RUN_TEST(test_above_domain_1);
  RUN_TEST(test_above_domain_2);
  RUN_TEST(test_above_domain_3);

  RUN_TEST(test_below_domain_1);
  RUN_TEST(test_below_domain_2);
  RUN_TEST(test_below_domain_3);

  RUN_TEST(test_within_domain_1);
  RUN_TEST(test_within_domain_2);
  RUN_TEST(test_within_domain_3);

  RUN_TEST(test_normal_case_1);
  RUN_TEST(test_normal_case_2);

  UNITY_END();
}

#ifdef ARDUINO

/**
 * Setup for arduino framework. Called once at the beginning of testing
 */
void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  run_tests();
}

/**
 * Loop function for arduino framework. Called repeatedly
 */
void loop() {}

#else

/**
 * Entypoint for the test code
 * @param argc integer representing the number of command line arguments.
 *   Not relevant in a testing context
 * @param argv string array of the command line arguments. Also not relevant
 *  in a testing environment
 * @returns exit code of the program
 */
int main(int argc, char* argv[]) {
  run_tests();
  return 0;
}

#endif
