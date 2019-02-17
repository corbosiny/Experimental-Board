/**
 * Defines a linear interpolation function
 */

#ifndef _CURVE_GEN_INTERPOLATE_H_
#define _CURVE_GEN_INTERPOLATE_H_

#include <cstddef>
#include <inttypes.h>
#include <utility>

/**
 * @brief Performs binary search for the first argument within each pair in the array.
 *
 * This function is designed for use in the interp function. Implementation
 * based on https://github.com/numpy/numpy/blob/0ea890a7b8a26f45e05c71075994e5be863fd49b/numpy/core/src/multiarray/compiled_base.c#L388
 * @param array array of pairs to search
 * @param array_size size of the array
 * @param key value being searched for
 * @returns the index i of the array such that array[i] <= key < array[i + 1]
 */
int64_t BinarySearch(std::pair<double, double> *array, size_t array_size, double key);

/**
 * @brief Linearly interpolates over one dimensional data
 * 
 * @param array array of data to interpolate
 * @param size size of the given array
 * @param key location of the desired data point
 * @return double 
 */
double Interp(std::pair<double, double> *array, size_t size, double key); 

#endif // _CURVE_GEN_INTERPOLATE_H_
