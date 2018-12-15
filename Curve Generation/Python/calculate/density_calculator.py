"""Calculates density of air"""

import typing
import math
import numpy as np
import operator

from calculate import unary_linear_interpolator

pressure_table = [
    (0, 101.3), (152, 99.4), (305, 97.6),
    (457, 95.8), (610, 94.1), (762, 92.4),
    (914 , 90.7), (1067, 89.1), (1219, 87.4),
    (1372, 85.8), (1524, 84.3), (1676, 82.7),
    (1829, 81.1), (1981, 79.6), (2134, 78.1),
    (2286, 76.7), (2438, 75.2), (2591, 73.8),
    (2743, 72.3), (2896, 71.0), (3048, 69.6),
    (3200, 68.3), (3353, 67.0), (3505, 65.7),
    (3658, 64.4), (3810, 63.1), (3962, 61.9),
    (4115, 60.7), (4267, 59.5), (4420, 58.3),
    (4572, 57.1), (4724, 56.0), (4877, 54.8),
    (5029, 53.8), (5182, 52.7), (5334, 51.5),
    (5486, 50.6), (5639, 49.5), (5791, 48.6),
    (5944, 47.5), (6096, 46.6), (6248, 45.6),
    (6401, 44.6), (6553, 43.7), (6706, 42.8),
    (6858, 41.9), (7010, 41.0), (7163, 40.1),
    (7315, 39.3), (7468, 38.4), (7620, 37.6),
    (7772, 36.8), (7925, 36.0), (8077, 35.2),
    (8230, 34.5), (8382, 33.7), (8534, 32.9),
    (8687, 32.2), (8839, 31.5), (8992, 30.8),
    (9144, 30.1)
] # yapf: disable

class DensityCalculator(object):
    """Calculates the density of air
    
    Attributes:
        R: (JOULE / MOL * KELVIN) Ideal (universal) gas constant.
        M: (KILOGRAM / MOL) Molar mass of air.
        g: (METER / SECOND ^ 2) Earth surface gravitational acceleration.
        L: (KELVIN / METER) Temperature lapse rate.
        T_0: (KELVIN) Sea level standard temperature.
        P_0: (KILOPASCALS) Sea level standard atmospheric pressure.
        pressure: An interpoloate for calculating pressure
    """

    R: float = 8.31447
    M: float = 0.0289644
    g: float = 9.80665
    L: float = 0.0065
    T_0: float = 288.15  # Kelvin
    P_0: float = 101.325

    def __init__(self, start_height: float = 1220.0, **kwargs):
        """Initializes the DensityCalculator

        Args:
            start_height: (METERS) Starting height, because we wouldn't launch
                at sea level
        """
        self._start_height = start_height
        self.pressure = unary_linear_interpolator.UnaryLinearInterpolator(
            list(map(operator.itemgetter(0), pressure_table)),
            list(map(operator.itemgetter(1), pressure_table)))

    def __call__(self, height: float = None, **kwargs) -> float:
        """Calculates the density at a specific point

        Args:
            height: If given, calculates pressure according to this
                https://en.wikipedia.org/wiki/Density_of_air#Altitude formula.

        Returns:pressure
            The calculated density.
        """
        return self.calculate_density(height)

    def calculate_density(self, height: float = None, **kwargs) -> float:
        """Performs the calculation of density

        Args:
            height: (METERS) If given, calculates the pressure according to
                this https://en.wikipedia.org/wiki/Density_of_air#Altitude
                formula.

        Raises:
            KeyError: Raised when invalid arguments are given

        Returns:
            float: (KILOGRAMS / METER ^ 3)
        """
        if height != None:
            height += self._start_height
            temperature = DensityCalculator.T_0 - DensityCalculator.L * height

            # Pressure is in KILOPASCALS, or (KILOJOULES / METER ^ 3)
            pressure = self.pressure(height)

            # Converting from KILOPASCALS to PASCALS (JOULES / METER ^ 3)
            pressure *= 1000

            return (pressure * DensityCalculator.M) / (
                DensityCalculator.R * temperature)
        else:
            raise KeyError('invalid arguments')
