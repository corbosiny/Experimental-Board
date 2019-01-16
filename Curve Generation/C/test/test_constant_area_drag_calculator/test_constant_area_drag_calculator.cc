
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
  RUN_TEST(test_zeros);

  UNITY_END();
  return 0;
}
