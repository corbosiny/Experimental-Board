
import typing
import numpy as np

class UnaryLinearInterpolator(object):
    """Interpolates single values linearly with the given data."""

    def __init__(self,
                 x_values: typing.Iterable[float],
                 # y_values could be complex numbers, but not currently
                 # supported
                 y_values: typing.Iterable[float]):
        """Constructs the interpolator.

        Args:
            x_values: the 'x' values at which the data has been given. Assumes
                they are in sorted order.
            y_values: the data points themselves.
        """

        self._x_values = x_values
        self._y_values = y_values

    def __call__(
        self,
        value: typing.Union[np.array, float]) -> typing.Union[np.array, float]:
        """Evaluates the data at the given value

        Args:
            value: the 'x' value at which to interpret the data from.

        Returns:
            data at the given value
        """
        return np.interp(value, self._x_values, self._y_values)