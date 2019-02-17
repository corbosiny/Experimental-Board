/**
 * @file test_density_calculator.cc
 * @brief Tests the density calculator
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

#include "density_calculator.h"

/**
 * Tests a valid case with 1220 meter starting altitude and 1000 meters off the
 * ground
 */
void test_valid_parameters(void) {
  TEST_ASSERT_EQUAL_FLOAT(0.9838932262942124, calculate_density(1220.0, 1000.0));
}

/**
 * Tests the zero case
 */
void test_zero(void) {
  TEST_ASSERT_EQUAL_FLOAT(1.2246767605769946, calculate_density(0.0f, 0.0f));
}

/**
 * Portable function to run all the tests
 */
void run_tests() {
  UNITY_BEGIN();

  RUN_TEST(test_valid_parameters);
  RUN_TEST(test_zero);

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
