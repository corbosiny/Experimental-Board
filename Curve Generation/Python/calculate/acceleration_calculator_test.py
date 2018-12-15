
"""Unit test script for the AccelerationCalculator.py"""

import unittest
import numpy as np

from calculate import acceleration_calculator

class AccelerationCalculatorUnitTest(unittest.TestCase):
    """Unittest case for AccelerationCalculator"""

    def test_single_value(self):
        """Tests the AccelerationCalculator with a single value."""
        acceleration = acceleration_calculator.AccelerationCalculator(
            time=[0.0, 1.0],
            thrust=[0.0, 100.0],
            mass=[1.0, 0.5],
            base_mass=0.01
        )

        result = acceleration(0.5)
        self.assertEqual(result, 55.98282368421052)


    def test_array(self):
        """Tests the AccelerationCalculator with an array of objects."""
        acceleration = acceleration_calculator.AccelerationCalculator(
            time=[0.0, 1.0],
            thrust=[0.0, 100.0],
            mass=[1.0, 0.5],
            base_mass=0.01
        )

        result = acceleration(np.array([0.5]))
        self.assertEqual(result[0], 55.98282368421052)


if __name__ == '__main__':
    unittest.main()