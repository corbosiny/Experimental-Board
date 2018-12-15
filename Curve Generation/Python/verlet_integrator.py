"""A class used for verlet integration

https://en.wikipedia.org/wiki/Verlet_integration
"""

import math
from typing import Dict, Iterable, Callable, List
import numpy as np
import operator

from calculate import unary_linear_interpolator


class SteppingVerletIntegrator(object):
    """Verlet integration using discrete samples.

    This verlet integrator does NOT use any matrix multiplication.

    Attributes:
        acceleration: Access to the second dervative function that is passed
            to this object during construction.
        time_step: The time step using to integrate. Note that the error will
            be related to this variable.
        feedback: Interpolator for feedback of previously received altitude
            values
        past_n_steps: Number of steps to go back in time for calculation of
            velocity as a moving average
    """

    def __init__(self,
                 time_step: float,
                 acceleration: Callable[[float], float],
                 initial_value: float = 0.0,
                 initial_velocity: float = 0.0,
                 num_steps: int = 50,
                 collected_data: Iterable[Dict[str, float]] = [],
                 time_cumulation: float = 100,
                 acceleration_error_constant: float = 10.0,
                 start_time: float = 0.0):
        """Initializes the integrator with a timestep

        Args:
            time_step: floating point value
            acceleration: A function that can be evaluated for any time t, that
                returns the second derivative of the value the integrator is
                solving for. Should return METERS / SECONDS ^ 2
            initial_value: (METERS) initial altitude
            initial_velocity: (METERS / SECONDS) initial velocity
            collected_data: List of dictionaries that contain 
            time_cumulation: (MILLISECONDS) amount of time used for the moving
                average for the velocity
            acceleration_error_constant: A constant to use in acceleration
                error computing when there's not enough data points, in
                meters / seconds ^ 2
            start_time: Starting time, usually at 0
        """
        self.acceleration = acceleration
        self._timestep = time_step
        self._start_time = start_time

        self._previous_values = [None] * num_steps
        self._previous_values[0] = initial_value
        self._previous_values[1] = (
            time_step * initial_velocity + initial_value)

        self._initial_value = initial_value
        self._initial_velocity = initial_velocity

        self._num_steps = num_steps
        self._max_collected_data_time: float = max(
            map(operator.itemgetter('time'), collected_data), default=0.0)
        self._collected_data = collected_data
        self.feedback = unary_linear_interpolator.UnaryLinearInterpolator(
            list(map(operator.itemgetter('time'), collected_data)),
            list(map(operator.itemgetter('altitude'), collected_data)))

        self._last_index = self.fill_values(self._previous_values)

        self.past_n_steps = math.ceil(time_cumulation / (time_step * 1000))
        self._acceleration_error_constant = acceleration_error_constant

    def fill_values(self, array: List[float]) -> int:
        """If present, uses any collected data to fill in the integrator

        Args:
            array: List to store the collected data in

        Returns:
            the last index which had collected data for
        """

        if self._max_collected_data_time <= 0:
            return 0

        current_time = self.start_time
        index = 0
        while current_time < self._max_collected_data_time and index < len(array):
            array[index] = self.feedback(current_time)
            index += 1
            current_time = index * self.time_step

        # subtract one since index - 1 is the actual last index that was
        # written to
        return index - 1

    @property
    def initial_value(self) -> float:
        """Property for the initial value of the altitude

        Returns:
            floating point value that represents the initial altitude, in
                meters
        """
        return self._initial_value

    @property
    def initial_velocity(self) -> float:
        """Property for the initital velocity

        Returns:
            floating point value for the initial velocity, in meters / seconds
        """
        return self._initial_velocity

    @property
    def time_step(self) -> float:
        """Returns the time-step for this integrator"""
        return self._timestep

    @property
    def num_steps(self) -> int:
        """Returns the number of steps to simulate

        Returns:
            number of steps to simulate, i.e. the discretization of the time
                span
        """
        return self._num_steps

    @property
    def collected_data(self) -> Iterable[Dict[str, float]]:
        """Property for the list of previously collected data

        Returns:
            list of previously collected acceleration data, from an
                accelerometer
        """
        return self._collected_data

    @property
    def last_index(self) -> int:
        """Property for the last index that was calculated from previous data

        Returns:
            An integer value for the last index that was computed using
                previous data
        """
        return self._last_index

    @property
    def acceleration_error_constant(self) -> float:
        """Property for the constant error in accelerometer data

        Returns:
            floating point value for the constant in accelerometer data
        """
        return self._acceleration_error_constant

    @property
    def start_time(self) -> float:
        """Property for the starting time

        Returns:
            floating point value for the starting time in the simulation, in
                seconds
        """
        return self._start_time

    def __setitem__(self, index: int, value: float) -> None:
        """Sets the given value in the 'previous_values' internal property
        
        Args:
            index: the index within 'previous_values' corresponding to the
                value at time (time_step * value)
            value: the calculated value to save
        """
        self._previous_values[index] = value

    def __getitem__(self, index: int) -> float:
        """Retrieves the value at the specified index.

        Args:
            index: The index which corresponds to time (time_step * index).

        Returns:
            The calculated value.
        """
        return self.retrieve_value(index, self._previous_values)

    def retrieve_value(self,
                       index: int,
                       array: List[float],
                       acceleration_error: float = 0.0) -> float:
        """Calculates the value at the given index

        Determines what the value of the integration is at the given index,
        which corresponds to the time at index * time step + start time

        Args:
            index: integer specifying the index
            array: List of values to look in
            acceleration_error: error to apply to the acceleration. In meters
                / seconds ^ 2

        Returns:
            the floating point value at that location in the list
        """
        if index == 0 or index == 1 or array[index] != None:
            return array[index]

        current_index = index
        while (array[current_index - 1] == None
               or array[current_index - 2] == None):
            current_index -= 1

        while array[index] == None:
            if current_index != index:
                self.calculate_value(current_index, array)
            else:
                self.calculate_value(current_index, array, acceleration_error)

            current_index += 1

        return array[index]

    def get_velocity(self, index: int, array: List[float]) -> float:
        """Returns the velocity as a moving average

        Args:
            index: Step index from which to calculate velocity for
            array: The container of data from which to calculate from

        Returns:
            float
        """
        velocity_sum = 0.0
        number_iterations = min(self.past_n_steps, index - 1)
        for i in range(number_iterations):
            velocity_sum += (array[index - 1 - i] - array[index - 2 - i])

        return velocity_sum / (self.time_step * number_iterations)

    def calculate_value(self,
                        index: int,
                        array: List[float],
                        acceleration_error: float = 0.0) -> None:
        """Caculates the value at the specified index and records it in the
        array

        Args:
            index: integer specifying windex + 1here to calculate the value
            array: the list of numbers to store the result
            acceleration_error: error to apply to the acceleration. In meters
                / seconds ^ 2
        """
        velocity = self.get_velocity(index, array)
        acceleration = self.acceleration(
            time=self.time_step * (index - 1),
            velocity=velocity,
            height=array[index - 1] + velocity * self.time_step)

        acceleration += acceleration_error

        value = (2 * array[index - 1] - array[index - 2] +
                 acceleration * self.time_step * self.time_step)

        array[index] = value

    def __iter__(self):
        """Returns an iterator for all of the values at each time step

        Returns:
            iterator for all of the values at the timesteps
        """
        self.__getitem__(self._num_steps - 1)
        return iter(self._previous_values[self.last_index:])

    def __len__(self):
        """Returns the number of steps"""
        return len(self._previous_values[self.last_index:])

    def get_accelerometer_error(self) -> (Iterable[float], Iterable[float]):
        """Returns two new lines representing min/max accelerometer error

        Returns:
            two lists of values, representing two lines of accelerometer errors
        """
        acceleration_error = self.compute_accelerometer_error()
        upper_error = [None] * self.num_steps
        lower_error = [None] * self.num_steps

        upper_error[0] = lower_error[0] = self.initial_value
        upper_error[1] = lower_error[1] = (
            self.time_step * self.initial_velocity + self.initial_value)

        index = self.fill_values(upper_error)
        self.fill_values(lower_error)
        if index >= self.num_steps - 1:
            return (upper_error[index:], lower_error[index:])

        if index > 1:
            self.retrieve_value(
                index + 1, upper_error, acceleration_error=acceleration_error)
            self.retrieve_value(
                index + 1, lower_error, acceleration_error=-acceleration_error)
        else:
            self.retrieve_value(index + 1, upper_error)
            self.retrieve_value(index + 1, lower_error)
            self.retrieve_value(
                index + 2, upper_error, acceleration_error=acceleration_error)
            self.retrieve_value(
                index + 2, lower_error, acceleration_error=-acceleration_error)

        self.retrieve_value(self.num_steps - 1, upper_error)
        self.retrieve_value(self.num_steps - 1, lower_error)

        return (upper_error[index:], lower_error[index:])

    def compute_accelerometer_error(self) -> float:
        """Computes the amount of error to propogate over the acceleration
            error lines

        Returns:
            a floating point number representing the amount of error to
                propogate
        """
        number_iterations = min(self.past_n_steps, len(self.collected_data))
        if number_iterations <= 1:
            return self.acceleration_error_constant

        # Generate a best fitting line for the last few data points
        collected_data_iterator = reversed(self.collected_data)
        x_values = []
        y_values = []

        assert (len(self.collected_data) >= number_iterations)
        for i in range(number_iterations):
            data = next(collected_data_iterator)
            x_values.insert(0, data['time'])
            y_values.insert(0, data['acceleration'])

        slope, intercept = np.polyfit(x_values, y_values, 1)
        line = np.poly1d((slope, intercept))

        standard_deviation = math.sqrt(
            sum([
                math.pow(abs(line(x_values[i]) - y_values[i]), 2)
                for i in range(number_iterations)
            ]) / number_iterations)

        return standard_deviation
