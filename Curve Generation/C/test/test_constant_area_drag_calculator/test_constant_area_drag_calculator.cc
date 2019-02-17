/**
 *  @file test_constant_area_drag_calculator.cc
 *  @brief Unit testing file for the constant_area_drag_calculator
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

#include "constant_area_drag_calculator.h"

/**
 * Tests the function with valid parameters
 */
void test_valid_parameters(void) {
  double result = calculate_drag(
    1220.0, 1000.0, 0.033, 0.1, 100.0);
  TEST_ASSERT_EQUAL_FLOAT(1.6830449978792872, result);
}

void test_zeros(void) {
  double result = calculate_drag(
    0.0, 0.0, 0.0, 0.0, 0.0);
  TEST_ASSERT_EQUAL_FLOAT(0.0, result);
}

/**
 * Portable function to run all the tests
 */
void run_tests() {
  UNITY_BEGIN();

  RUN_TEST(test_valid_parameters);
  RUN_TEST(test_zeros);

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
