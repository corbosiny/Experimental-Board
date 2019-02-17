#include "interpolate.h"

#include <cassert>
#include <utility>

int64_t BinarySearch(std::pair<double, double> *array, size_t array_size, double key) {
  assert(array_size > 0);

  size_t index_min = 0;
  size_t index_max = array_size;

  /* Handle keys outside of the array range first */
  if (key > array[array_size - 1].first) {
    return array_size;
  }
  else if (key < array[0].first) {
    return -1;
  }

  /*
   * If len <= 4 use linear search.
   * From above we know key >= array[0] when we start.
   */
  if (array_size <= 4) {
    size_t index;

    for (index = 1; index < array_size && key >= array[index].first; ++index);
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

double Interp(std::pair<double, double> *array, size_t size, double key) {
  int64_t index = BinarySearch(array, size, key);
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
