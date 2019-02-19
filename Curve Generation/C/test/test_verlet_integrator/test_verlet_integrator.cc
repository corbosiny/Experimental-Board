/**
 * @file test_verlet_integrator.cc
 * @brief Defines tests for the verlet integrator
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

#include <cstddef>  // std::size_t
#include <iostream>

#include <unity.h>

#include "verlet_integrator.h"

/**
 * @brief Tests verlet_integrator with normal parameters
 * 
 */
void test_normal_parameters_1() {
  struct InitializationData initialization_data;
  initialization_data.initial_value = 10.0;
  initialization_data.initial_velocity = 5.0;
  initialization_data.start_time = 2.0;
  initialization_data.acceleration_error_constant = 0.0;

  double data[] = { 0.0, 0.0, 0.0 };
  std::size_t data_size = 3;
  double timestep = 0.1;

  struct AccelerationCalculationData acceleration_data;
  acceleration_data.drag_coefficient = 0.1;
  acceleration_data.radius = 0.03;
  acceleration_data.base_mass = 1.0;

  VerletIntegrator verlet_integrator(initialization_data);
  verlet_integrator.Simulate(data, data_size, timestep, acceleration_data);

  double expected[] = { 10, 15, 19.8976 };
  TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected, data, data_size);
}

/**
 * @brief Tests verlet_integrator with normal parameters and decreasing
 *  altitude
 * 
 */
void test_normal_parameters_2() {
  struct InitializationData initialization_data;
  initialization_data.initial_value = 10.0;
  initialization_data.initial_velocity = 0.0;
  initialization_data.start_time = 0.0;
  initialization_data.acceleration_error_constant = 0.0;

  const std::size_t data_size = 3;
  double data[3];
  
  double timestep = 0.1;

  struct AccelerationCalculationData acceleration_data;
  acceleration_data.drag_coefficient = 0.1;
  acceleration_data.radius = 0.03;
  acceleration_data.base_mass = 1.0;

  VerletIntegrator verlet_integrator(initialization_data);
  verlet_integrator.Simulate(data, data_size, timestep, acceleration_data);

  double expected[] = { 10.0, 0.0, 10 - 9.80665 };
  TEST_ASSERT_EQUAL_FLOAT_ARRAY(expected, data, data_size);
}


/**
 * Portable function to run all the tests
 */
void run_tests() {
  UNITY_BEGIN();

  RUN_TEST(test_normal_parameters_1);
  RUN_TEST(test_normal_parameters_2);

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
