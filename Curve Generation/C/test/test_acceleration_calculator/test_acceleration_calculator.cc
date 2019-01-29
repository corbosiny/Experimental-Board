
#include <unity.h>

#include "acceleration_calculator.h"

/**
 * Tests the function with valid parameters
 */
void test_valid_parameters(void) {
  std::array<std::pair<double, double>, 1> collected_data = {{ { 0.0, 0.0 } }};
  std::array<std::pair<double, double>, 13> thrust_values = {{
    { 0.0, 0.0 }, { 0.0, 0.09 }, { 0.1, 224.89 }, { 0.2, 206.88 },
    { 0.5, 202.43 }, { 0.7, 184.41 }, { 1.0, 179.92 }, { 1.3, 166.4 },
    { 1.6, 134.91 }, { 1.8, 112.44 }, { 2.0, 62.98 }, { 2.1, 22.51 },
    { 2.3, 0.0 }
  }};
  std::array<std::pair<double, double>, 13> mass_values = {{
    { 0.0, 0.1875 }, { 0.0, 0.1875 }, { 0.1, 0.181176 }, { 0.2, 0.169039 },
    { 0.5, 0.134522 }, { 0.7, 0.112774 }, { 1.0, 0.0820506 }, { 1.3, 0.0528457 },
    { 1.6, 0.0274365 }, { 1.8, 0.0135306 }, { 2.0, 0.0036686 }, { 2.1, 0.0012655 },
    { 2.3, 0.000 }
  }};

  double result = calculate_acceleration<13, 1>(
    collected_data,
    0,
    thrust_values,
    mass_values,
    1.04363,
    138.61194746310215,
    66.70122083500863,
    0.0,
    0.033,
    0.06,
    0.960960960960961
  );

  TEST_ASSERT_EQUAL_FLOAT(147.85418701937494, result);
}

/**
 * Tests the function with collected data
 */
void test_collected_data(void) {
  std::array<std::pair<double, double>, 3> collected_data = {{
    { 0.0, 10.0 }, { 0.1, 10.0 }, { 0.2, 20.0 }
  }};
  std::array<std::pair<double, double>, 13> thrust_values = {{
    { 0.0, 0.0 }, { 0.0, 0.09 }, { 0.1, 224.89 }, { 0.2, 206.88 },
    { 0.5, 202.43 }, { 0.7, 184.41 }, { 1.0, 179.92 }, { 1.3, 166.4 },
    { 1.6, 134.91 }, { 1.8, 112.44 }, { 2.0, 62.98 }, { 2.1, 22.51 },
    { 2.3, 0.0 }
  }};
  std::array<std::pair<double, double>, 13> mass_values = {{
    { 0.0, 0.1875 }, { 0.0, 0.1875 }, { 0.1, 0.181176 }, { 0.2, 0.169039 },
    { 0.5, 0.134522 }, { 0.7, 0.112774 }, { 1.0, 0.0820506 }, { 1.3, 0.0528457 },
    { 1.6, 0.0274365 }, { 1.8, 0.0135306 }, { 2.0, 0.0036686 }, { 2.1, 0.0012655 },
    { 2.3, 0.000 }
  }};

  double result = calculate_acceleration<13, 3>(
    collected_data,
    3,
    thrust_values,
    mass_values,
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

#include <Arduino.h>

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
