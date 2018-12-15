
from typing import Callable
import numpy as np

class ConstantAreaDragCalculator(object):
    """Calculates drag according to https://en.wikipedia.org/wiki/Drag_equation

    Attributes:
        density: A reference to the callable object for calculating density as
            a function of height

    """
    def __init__(self, area: float = None, diameter: float = None,
                 radius: float = None, drag_coefficient: float = None,
                 density: Callable[[float], float] = None, **kwargs):
        """Initializes the DragCalculator object

        Args:
            area: Optional, constant area to use in the equation. Should be the
                cross sectional area
            diameter: Optional diameter if the body is an assumed cylindrical
                rocket body tube. Equivalent to area=(diameter * 0.5) ** 2 * PI
            radius: Optional radius if the body is an assumed cylindrical
                rocket body tube. Equivalent to area=radius * radius * PI
            density: Function used to evaluate density as a function of height
            drag_coefficient: constant value used in the calculation of drag

        Raises:
            TypeError: Raised if improper input
        """

        if area != None:
            self._area = area
        elif diameter != None:
            self._area = (diameter * 0.5) ** 2 * np.pi
        elif radius != None:
            self._area = radius ** 2 * np.pi
        else:
            raise TypeError('one of \'area\', \'radius\', \
                             or \'diameter\' must be defined')

        if density == None:
            raise TypeError('density() is undefined')
        else:
            self.density = density

        self._drag_coefficient = drag_coefficient 


    @property
    def area(self) -> float:
        """Returns the precomputed area from initialization"""
        return self._area


    def __call__(
        self,
        velocity: float = None, height: float = None,
        drag_coefficient: float = None) -> float:
        """Calculates the drag at the specified point

        Returns:
            Measured drag force in Newtons.
        """
        density = self.density(height)
        return self.calculate_drag(
            velocity,
            drag_coefficient,
            density
        )


    def calculate_drag(self, velocity: float = None,
                       drag_coefficient: float = None,
                       density: float = None, **kwargs) -> float:
        """Calculates the drag from the given keyword arguments

        Args:
            velocity: (METERS / SECONDS) Current speed in the air
            height: (METERS) altitude
            drag_coefficient: dimensionless constant related to Reynold's
                number

        Returns:
            float: (KILOGRAMS * METERS / SECONDS ^ 2) (NEWTONS) the drag at the
                specified point
        """
        return (0.5 * density * velocity * velocity *
            drag_coefficient * self._area)
