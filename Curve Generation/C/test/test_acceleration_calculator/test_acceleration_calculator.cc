/**
 * @file test_acceleration_calculator.cc
 * @brief Unit testing for the acceleration calculator
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

#include "acceleration_calculator.h"

/**
 * Tests the function with valid parameters
 */
void test_valid_parameters(void) {
  std::pair<double, double> collected_data[] = { { 0.0, 0.0 } };

  double result = calculate_acceleration(
    collected_data,
    0,
    1.04363,
    138.61194746310215,
    66.70122083500863,
    0.0,
    0.033,
    0.06,
    0.960960960960961
  );

  TEST_ASSERT_EQUAL_FLOAT(-12.1051, result);
}

/**
 * Tests the function with collected data
 */
void test_collected_data(void) {
  std::pair<double, double> collected_data[] = {
    { 0.0, 10.0 }, { 0.1, 10.0 }, { 0.2, 20.0 }
  };

  double result = calculate_acceleration(
    collected_data,
    3,
    1.04363,
    0.0,
    0.0,
    0.0,
    0.033,
    0.06,
    0.15
  );

  TEST_ASSERT_EQUAL_FLOAT(15.0, result);
}

/**
 * Portable function to run all the tests
 */
void run_tests() {
  UNITY_BEGIN();

  RUN_TEST(test_valid_parameters);
  RUN_TEST(test_collected_data);

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
