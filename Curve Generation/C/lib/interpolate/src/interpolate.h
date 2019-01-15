/**
 * Defines a linear interpolation function
 */

#ifndef _CURVE_GEN_INTERPOLATE_H_
#define _CURVE_GEN_INTERPOLATE_H_

#include <array>
#include <type_traits>
#include <utility>

/**
 * Performs binary search for the first argument within each pair in the array.
 *
 * This function is designed for use in the interp function. Implementation
 * based on https://github.com/numpy/numpy/blob/0ea890a7b8a26f45e05c71075994e5be863fd49b/numpy/core/src/multiarray/compiled_base.c#L388
 * @param array array of pairs to search
 * @param key value being searched for
 * @returns the index i of the array such that array[i] <= key < array[i + 1]
 */
template<size_t size>
size_t BinarySearch(
  const std::array<std::pair<double, double>, size> &array,
  double key
) {
  static_assert(size > 0,
                "BinarySearch requires array with at least one element");

  size_t index_min = 0;
  size_t index_max = size;

  /* Handle keys outside of the array range first */
  if (key > array[size - 1].first) {
      return size;
  }
  else if (key < array[0].first) {
      return -1;
  }

  /*
   * If len <= 4 use linear search.
   * From above we know key >= array[0] when we start.
   */
  if (size <= 4) {
    size_t index;

    for (index = 1; index < size && key >= array[index].first; ++index);
    return index - 1;
  }

  // find index by bisection
  while (index_min < index_max) {
    const size_t middle = index_min + ((index_max - index_min) >> 1);
    if (key >= array[middle].first) {
      index_min = middle + 1;
    }
    else {
      index_max = middle;
    }
  }
  return index_min - 1;
}

/**
 * Linearly interpolates over one dimensional data
 *
 * @param array data points
 * @param double key
 * @returns interpolated value
 */
template<size_t size>
double Interp(
  const std::array<std::pair<double, double>, size> &array,
  double key
) {
  size_t index = BinarySearch<size>(array, key);
  if (index < 0) {
    return array[0].second;
  }
  else if (index >= size - 1) {
    return array[size - 1].second;
  }

  std::pair<double, double> bottom = array[index];
  std::pair<double, double> top = array[index + 1];

  double slope = (top.second - bottom.second) / (top.first - bottom.first);
  return slope * (key - bottom.first) + bottom.second;
}

#endif // _CURVE_GEN_INTERPOLATE_H_
