/**
 * Defines tests that are above the domain of interpolation
 */

#include <unity.h>

#include "interpolate.h"

/**
 * Tests for a key above the domain in a small array
 */
void test_above_domain_1(void) {
  std::array<std::pair<double, double>, 3> array = {{ {0, 0}, {1, 1}, {2, 2} }};
  double result = Interp<3>(array, 3);
  TEST_ASSERT_EQUAL_FLOAT(2, result);
}

/**
 * Tests for a key above the domain in a large array
 */
void test_above_domain_2(void) {
  std::array<std::pair<double, double>, 10> array = {{
    {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
    {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}
  }};
  double result = Interp<10>(array, 11);
  TEST_ASSERT_EQUAL_FLOAT(9, result);
}

/**
 * Tests for a key above the domain in an array of one element
 */
void test_above_domain_3(void) {
  std::array<std::pair<double, double>, 1> array = {{ {0, 0} }};
  double result = Interp<1>(array, 1);
  TEST_ASSERT_EQUAL_FLOAT(0, result);
}

/**
 * Tests for a key below the domain in a small array
 */
void test_below_domain_1(void) {
  std::array<std::pair<double, double>, 3> array = {{ {0, 0}, {1, 1}, {2, 2} }};
  double result = Interp<3>(array, -1);
  TEST_ASSERT_EQUAL_FLOAT(0, result);
}

/**
 * Tests for a key below the domain in a large array
 */
void test_below_domain_2(void) {
  std::array<std::pair<double, double>, 10> array = {{
    {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
    {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}
  }};
  double result = Interp<10>(array, -1);
  TEST_ASSERT_EQUAL_FLOAT(0, result);
}

/**
 * Tests for a key below the domain in an array of one element
 */
void test_below_domain_3(void) {
  std::array<std::pair<double, double>, 1> array = {{ {0, 0} }};
  double result = Interp<1>(array, -1);
  TEST_ASSERT_EQUAL_FLOAT(0, result);
}

/**
 * Tests for a key within the domain in a small array on an edge
 */
void test_within_domain_1(void) {
  std::array<std::pair<double, double>, 3> array = {{ {0, 0}, {1, 1}, {2, 2} }};
  double result = Interp<3>(array, 0);
  TEST_ASSERT_EQUAL_FLOAT(0, result);

  result = Interp<3>(array, 2);
  TEST_ASSERT_EQUAL_FLOAT(2, result);
}

/**
 * Tests for a key within the domain in a large array on an edge
 */
void test_within_domain_2(void) {
  std::array<std::pair<double, double>, 10> array = {{
    {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
    {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}
  }};
  double result = Interp<10>(array, 0);
  TEST_ASSERT_EQUAL_FLOAT(0, result);

  result = Interp<10>(array, 9);
  TEST_ASSERT_EQUAL_FLOAT(9, result);
}

/**
 * Tests for a key within the domain in an array of one element
 */
void test_within_domain_3(void) {
  std::array<std::pair<double, double>, 1> array = {{ {0, 0} }};
  double result = Interp<1>(array, 0);
  TEST_ASSERT_EQUAL_FLOAT(0, result);
}

/**
 * Tests for a key in a normal case that requires interpolation with a small
 * array
 */
void test_normal_case_1(void) {
  std::array<std::pair<double, double>, 2> array = {{ {1, 1}, {2, 2} }};
  double result = Interp<2>(array, 1.5);
  TEST_ASSERT_EQUAL_FLOAT(1.5, result); 
}

/**
 * Tests for a key in a normal case that requires interpolation with a large
 * array
 */
void test_normal_case_2(void) {
  std::array<std::pair<double, double>, 10> array = {{
    {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4},
    {5, 5}, {6, 6}, {7, 7}, {8, 8}, {9, 9}
  }};
  double result = Interp<10>(array, 4.5);
  TEST_ASSERT_EQUAL_FLOAT(4.5, result); 
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
  return 0;
}


