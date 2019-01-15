/**
 * Tests the density calculator with valid parameters
 */

#include <iostream>

#include <unity.h>

#include "density_calculator.h"

void test_valid_parameters_1(void) {
  TEST_ASSERT_EQUAL(0.9838932262942124, calculate_density(1220.0, 1000.0));
}

/**
 * Entypoint for the test code
 * @param argc integer representing the number of command line arguments.
 *   Not relevant in a testing context
 * @param argv string array of the command line arguments. Also not relevant
 *  in a testing environment
 * @returns exit code of the program
 */
int main(int argc, char* argv[]) {
  UNITY_BEGIN();

  RUN_TEST(test_valid_parameters_1);

  UNITY_END();
  return 0;
}

