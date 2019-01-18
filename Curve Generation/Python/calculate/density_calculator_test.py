"""Unit test script for the air density calculator.

Hopefully, during the competition launch, we'll just have a table of density
values that we can index by altitude.
"""

import unittest

from calculate import density_calculator


class DensityCalculatorTest(unittest.TestCase):
    """Unittest class for the density calculator"""

    def testCalculateDensity_GivenHeight(self) -> None:
        """Tests the density calculator for giving correct values."""

        # 1200 meters above sea level start height since that's average
        # Arizona elevation
        density = density_calculator.DensityCalculator(start_height=1200)

        # Calculate the result at 5000 feet off the ground, 1524 meters
        result = density(height=1524)

        self.assertLess(abs(0.9352297623 - result), 0.0000001)


if __name__ == '__main__':
    unittest.main()
