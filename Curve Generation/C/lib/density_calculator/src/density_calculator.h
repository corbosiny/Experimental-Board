/**
 * Funtions and data for calculation of density
 */

#ifndef _CURVE_GEN_DENSITY_CALCULATOR_H_
#define _CURVE_GEN_DENSITY_CALCULATOR_H_

#include <array>
#include <utility>

/**
 * Static pressure table at various altitudes
 */
const size_t kNumPressureTableValues = 61;
extern std::pair<double, double> kPressureTable[];

/**
 * Calculates the density from the given height.
 * 
 * Uses this https://en.wikipedia.org/wiki/Density_of_air#Altitude formula.
 * @param start_height launch pad altitude, expressed in meters.
 * @param height current height above the ground, expressed in meters.
 * @return density, expressed in kilograms / (meters ^ 3).
 */
double calculate_density(double start_height, double height);

#endif // _CURVE_GEN_DENSITY_CALCULATOR_H_
