/**
 * Tests the density calculator with valid parameters
 */

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
 * Entypoint for the test code
 * @param argc integer representing the number of command line arguments.
 *   Not relevant in a testing context
 * @param argv string array of the command line arguments. Also not relevant
 *  in a testing environment
 * @returns exit code of the program
 */
int main(int argc, char* argv[]) {
  UNITY_BEGIN();

  RUN_TEST(test_valid_parameters);
  RUN_TEST(test_zero);

  UNITY_END();
  return 0;
}

