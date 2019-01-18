"""Calculates acceleration from the given data"""

from typing import Iterable, Dict, Callable, Tuple
import numpy as np
import operator

from calculate import unary_linear_interpolator
from calculate import density_calculator
from calculate import constant_area_drag_calculator

UnaryLinearInterpolator = unary_linear_interpolator.UnaryLinearInterpolator
ConstantAreaDragCalculator = constant_area_drag_calculator.ConstantAreaDragCalculator

GRAVITY = -9.80665


class AccelerationCalculator(object):
    """Calculates acceleration for an object (rocket) with thrust and drag.

    Doesn't take into account any airbrakes

    Attributes:
        get_thrust: unary function that allows evaluations of thrust at
            arbitrary times
    """

    def __init__(self,
                 thrust: Iterable[Tuple[float, float]] = [],
                 mass: Iterable[Tuple[float, float]] = [],
                 base_mass: float = 0.0,
                 drag_constant: float = 0.0,
                 collected_data: Iterable[Dict[str, float]] = [],
                 time_cumulation: float = 100,
                 **kwargs):
        """Constructs the acceleration calculator from the given parameters

        Args:
            thrust: List-like structure of thrust measured using binary tuples
                that keeps the time in seconds and the force in newtons
            mass: List-like structure of the mass measured. Corresponds to how
                fast the fuel burns. This is a list of binary tuples with time
                in seconds and the mass at that time in kilograms
            base_mass: Mass of the empty rocket in kilograms
            drag_constant: Dimensionless constant used in drag calculation.
            collected_data: List of previously collected data. Should be a list
                of dictionaries that contain time in SECONDS and acceleleration
                in METERS / SECONDS ^ 2.
            time_cumulation: Amount of time to "smooth" over velocity in
                seconds
        """

        self.get_thrust = UnaryLinearInterpolator(
            list(map(operator.itemgetter(0), thrust)),
            list(map(operator.itemgetter(1), thrust)))
        self._mass_values = UnaryLinearInterpolator(
            list(map(operator.itemgetter(0), mass)),
            list(map(operator.itemgetter(1), mass)))

        self._drag_constant = drag_constant
        self._base_mass = base_mass

        self._collected_data = collected_data
        self._max_collected_data_time: float = max(
            map(operator.itemgetter('time'), collected_data), default=0.0)

        self._feedback = UnaryLinearInterpolator(
            list(map(operator.itemgetter('time'), collected_data)),
            list(map(operator.itemgetter('acceleration'), collected_data)))

    @property
    def mass_values(self) -> UnaryLinearInterpolator:
        """Accessor for the interpolator for mass

        Returns:
            A interpolator for mass (in kilograms) by time (in seconds)
        """
        return self._mass_values

    def find_mass(self, t: float) -> float:
        """Calculates the mass at the given time.

        Args:
            t: time value in seconds

        Returns:
            mass at the given time in kilograms
        """
        return self._base_mass + self.mass_values(t)

    @property
    def max_collected_data_time(self) -> float:
        """Accessor for the latest time that acceleration has been recorded

        Returns:
            last time that data was recorded for acceleration, in seconds
        """
        return self._max_collected_data_time

    @property
    def feedback(self) -> UnaryLinearInterpolator:
        """Accessor for the interpolator for feedback

        Returns:
            feedback interpolator
        """
        return self._feedback

    @property
    def drag_constant(self) -> float:
        """Accessor for the dimensionless drag contant

        Returns:
            drag constant in a dimensionless float
        """
        return self._drag_constant

    def find_feeback(self, time: float) -> float:
        """Calculates any feedback given in the constructor

        Args:
            time: (SECONDS)

        Returns:
            float
        """

        return self.feedback(time)

    def __call__(self, time, **kwargs) -> float:
        """Calculates the acceleration at the given time(s)

        Args:
            time: (SECONDS) either a list like object or number value

        Returns:
            list of evaluated points or a single evaluated point
        """

        acceleration = 0.0

        if time < self.max_collected_data_time:
            acceleration = self.find_feeback(time)
        else:
            mass = self.find_mass(time)
            thrust = self.get_thrust(time)
            weight = mass * GRAVITY

            acceleration = (thrust + weight) / mass

        if isinstance(acceleration, np.ndarray):
            return np.array([
                acceleration[x] if acceleration[x] > 0 or time[x] > 1 else 0
                for x in range(len(time))
            ])
        else:
            if acceleration < 0 and time < 1:
                return 0
            return acceleration


class AccelerationCalculatorDrag(AccelerationCalculator):
    """Acceleration calculator that attempts to approximate drag."""

    def __init__(self, *args, **kwargs):
        """Initializes and constructs the acceleration calculator.

        Args:
            kwargs: Dict of keyword arguments. See the constructor of
                AccelerationCalculator
        """
        super().__init__(*args, **kwargs)

        self._density = density_calculator.DensityCalculator(**kwargs)
        kwargs.setdefault('density', self._density)

        self._drag = ConstantAreaDragCalculator(**kwargs)

    @property
    def drag(self) -> Callable:
        """Accessor for the drag calculator

        Returns:
            Callable object for calculating drag
        """
        return self._drag

    def __call__(self,
                 time: float = 0.0,
                 velocity: float = 0.0,
                 height: float = 0.0,
                 **kwargs):
        """Calculates the acceleration at time, velocity, and height

        Args:
            time: time in seconds
            velocity: velocity at the given time in meters / seconds
            height: height at the given time in meters
        Returns:
            calculated acceleration at the given time in meters / seconds ^ 2
        """

        if time < self.max_collected_data_time:
            acceleration = self.find_feeback(time)
            return acceleration
        else:
            mass = self.find_mass(time)
            thrust = self.get_thrust(time)
            # Drag is measured in Newtons, or (KILOGRAMS * METERS) / SECONDS ^ 2
            drag = self._drag(
                velocity, height, drag_coefficient=self.drag_constant)

            weight = mass * 9.80665
            force = thrust - weight - drag

            return force / mass
