/**
 * Defines a function used for calculating drag
 */

#ifndef _CURVE_GEN_CONSTANT_AREA_DRAG_CALCULATOR_H_
#define _CURVE_GEN_CONSTANT_AREA_DRAG_CALCULATOR_H_

#define _PI 3.14159265358979323846

#include "density_calculator.h"

/**
 * Calculates drag with the given data.
 * @param start_height launch pad altitude in meters.
 * @param height height from the ground in meters.
 * @param radius radius of the rocket tube in meters.
 * @param drag_coefficient dimensionless constant used in calculation of the
 *   drag according to the body's shape.
 * @param velocity current velocity of the rocket in meters / seconds.
 * @return drag, epressed in Newtons (Kilograms / meters ^ 2)
 */
double calculate_drag(
  double start_height,
  double height,
  double radius,
  double drag_coefficient,
  double velocity
);

#endif // _CURVE_GEN_CONSTANT_AREA_DRAG_CALCULATOR_H_
