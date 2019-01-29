/**
 * Defines utilities for calculating acceleration
 */

#ifndef _CURVE_GEN_ACCELERATION_CALCULATOR_H_
#define _CURVE_GEN_ACCELERATION_CALCULATOR_H_

#define GRAVITY 9.80665

#include <array>
#include <cassert>
#include <utility>

#include "constant_area_drag_calculator.h"
#include "interpolate.h"

template<size_t thrust_length, size_t max_collected_data_length>
double calculate_acceleration(
  const std::array<std::pair<double, double>, max_collected_data_length> &collected_data,
  size_t collected_data_length,
  const std::array<std::pair<double, double>, thrust_length> &thrust_values,
  const std::array<std::pair<double, double>, thrust_length> &mass_values,
  double base_mass,
  double velocity,
  double height,
  double start_height,
  double radius,
  double drag_coefficient,
  double time
) {
  assert(collected_data_length <= max_collected_data_length);
  if (collected_data_length > 0) {
    std::pair<double, double> last_recorded_thrust = collected_data[collected_data_length - 1];
    if (time <= last_recorded_thrust.first) {
      return Interp<max_collected_data_length>(
        std::array<std::pair<double, double>, max_collected_data_length>(collected_data),
        time
      );
    }
  }

  double current_thrust = Interp<thrust_length>(thrust_values, time);
  double current_mass = Interp<thrust_length>(mass_values, time);

  current_mass += base_mass;
  double drag = calculate_drag(
    start_height,
    height,
    radius,
    drag_coefficient,
    velocity
  );

  double weight = current_mass * GRAVITY;
  double force = current_thrust - weight - drag;
  return force / current_mass;
}

#endif // _CURVE_GEN_ACCELERATION_CALCULATOR_H_
