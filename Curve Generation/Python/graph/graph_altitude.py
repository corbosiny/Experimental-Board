"""Defines all of the functionality for plotting altitude"""

import operator
import io
import csv
import enum
from typing import List, Tuple
import matplotlib.pyplot as plt
import numpy as np
from calculate import unary_linear_interpolator
from calculate import acceleration_calculator
import verlet_integrator

UnaryLinearInterpolator = unary_linear_interpolator.UnaryLinearInterpolator
AccelerationCalculatorDrag = acceleration_calculator.AccelerationCalculatorDrag


class GRAPH(enum.IntFlag):
    ALTITUDE = 1 << 0
    BURNOUT = 1 << 1
    LEGEND = 1 << 2
    ACCELERATION = 1 << 3
    ACCELEROMETER_ERROR = 1 << 4
    SAVE_PLOT = 1 << 5


class AltitudeGrapher(object):
    """Responsible for data collection and provides plotting methods for
    altitude data.
    """

    def __init__(self,
                 previous_altitude: List[Tuple[float, float]] = None,
                 previous_acceleration: List[Tuple[float, float]] = None,
                 thrust_values: List[Tuple[float, float]] = None,
                 initial_altitude: float = 0,
                 total_time: float = None,
                 num_steps: int = None,
                 drag_coefficient: float = None,
                 base_mass: float = None,
                 mass_values: List[Tuple[float, float]] = None,
                 diameter: float = None,
                 acceleration_error_constant: float = None,
                 **kwargs) -> None:
        """Initializes the object

        Args:
            thrust_values: A list of tuples that contain times (in seconds)
                and forces (in newtons) generated from thrust
            initial_altitude: Starting altitude in meters. This won't be
                displayed on the graph (since we're only interested in how far
                the rocket travels off of the ground) but will factor into air
                density calculations
            total_time: The total amount of time in seconds to simulate
            num_steps: The number of steps within the timespan to simulate.
                Discretizes the time span specified by total_time
            drag_coefficient (optional): Optional, dimensionless constant used
                in drag calculation
            base_mass (optional): Optional value that specifies the mass of an
                empty rocket in kilograms
            diameter (optional): Diameter of the rocket in meters

        Raises:
            TypeError: If the correct arguments aren't supplied
        """
        if thrust_values:
            self.thrust_values = thrust_values
        if mass_values:
            self.mass_values = mass_values

        self._base_mass = base_mass
        self._drag_coefficient = drag_coefficient
        self._diameter = diameter
        self._acceleration_error_constant = acceleration_error_constant

        self._previous_acceleration = previous_acceleration if previous_acceleration else []
        self._previous_altitude = previous_altitude if previous_altitude else []

        self._total_time = total_time
        self._num_steps = num_steps
        if self.total_time == None or self.num_steps == None:
            raise TypeError('Must provide total time and number of steps')

    @property
    def thrust_values(self) -> List[Tuple[float, float]]:
        """Accessor for thrust_values

        Returns:
            thrust values as a list of tuples with time (SECONDS) and force
                (NEWTONS)
        """
        return self._thrust_values

    @thrust_values.setter
    def thrust_values(self, thrust_values: List[Tuple[float, float]]) -> None:
        """Mutator for thrust_values

        Args:
            thrust_values: A list of tuples that contain time and force values
        """
        self._thrust_values = thrust_values
        self._thrust_interpolator = UnaryLinearInterpolator(
            list(map(operator.itemgetter(0), self.thrust_values)),
            list(map(operator.itemgetter(1), self.thrust_values)))

    @property
    def mass_values(self) -> List[Tuple[float, float]]:
        """Accessor for the mass_values

        Returns:
            mass_values as a list of binary tuples with time in seconds and
                mass in kilograms
        """
        return self._mass_values

    @mass_values.setter
    def mass_values(self, mass_values: List[Tuple[float, float]]) -> None:
        """Mutator for mass_values

        Args:
            mass_values: values as a list of binary tuples with time in seconds
                and mass in kilograms
        """
        self._mass_values = mass_values

    @property
    def base_mass(self) -> float:
        """Accessor for the base_mass value

        Returns:
            mass of an empty rocket in kilograms
        """
        return self._base_mass

    @property
    def drag_coefficient(self) -> float:
        """Accessor for the drag_coefficient value

        Returns:
            dimensionless constant related to the drag of the rocket
        """
        return self._drag_coefficient

    @property
    def acceleration_error_constant(self) -> float:
        """Property for the error in the accelerometer

        Returns:
            floating point value for the error in the acceleration calculation
        """
        return self._acceleration_error_constant

    @property
    def thrust_interpolator(self) -> UnaryLinearInterpolator:
        """Accessor for the thrust_interpolator

        Returns:
            the linear interpolator object for thrust values
        """
        return self._thrust_interpolator

    def find_thrust(self, time: float) -> float:
        """Returns the thrust value at the given time

        Args:
            time: in seconds

        Returns:
            thrust force in newtons
        """
        return self.thrust_interpolator(time)

    @property
    def previous_acceleration(self) -> List[Tuple[float, float]]:
        """Accessor to the previously saved acceleration data

        Returns:
            list of binary tuples with time (seconds) and acceleration
                (meters / seconds ^ 2)
        """
        return self._previous_acceleration

    @previous_acceleration.setter
    def previous_acceleration(self, values: List[Tuple[float, float]]) -> None:
        """Mutator for the previous_acceleration values

        Args:
            values: List of binary tuples that contains the previous
                acceleration data. In seconds and meters / seconds ^ 2
        """
        self._previous_acceleration = values

    @property
    def previous_altitude(self) -> List[Tuple[float, float]]:
        """Accessor to the previously saved altitude data

        Returns:
            list of binary tupes with time (seconds) and height (meters)
        """
        return self._previous_altitude

    @previous_altitude.setter
    def previous_altitude(self, values: List[Tuple[float, float]]) -> None:
        """Mutator for the previous_altitude values

        Args:
            values: List of binary tuples that contains the previous
                altitude data. In seconds and meters / seconds ^ 2
        """
        self._previous_altitude = values

    @property
    def total_time(self) -> float:
        """Accessor for the total_time property

        Returns:
            total time to simulate, in seconds
        """
        return self._total_time

    @property
    def num_steps(self) -> int:
        """Accessor for the number of steps to simulate

        Returns:
            an integer representing the number of steps to simulate
        """
        return self._num_steps

    @property
    def current_time(self) -> float:
        """Returns the latest time that a altitude value has been recorded.

        In the actual rocket flight, this would be retrieved from an altimeter

        Returns:
            time of the last altitude value captured in seconds
        """
        return max(
            map(operator.itemgetter(0), self.previous_altitude), default=0.0)

    @property
    def diameter(self) -> float:
        """Accessor for the given diameter of the rocket

        Returns:
            diameter of the rocket in meters
        """
        return self._diameter

    def plot(self,
             flags: int = GRAPH.ALTITUDE | GRAPH.BURNOUT,
             figure_size: Tuple[float, float] = (12.8, 9.6),
             num_steps: int = None,
             total_time: float = None,
             base_mass: float = None,
             drag_constant: float = None,
             diameter: float = None,
             acceleration_error_constant: float = None,
             title: str = 'Altitude',
             filename: str = None) -> None:
        """Plots the values determined from the given flags

        Args:
            flags: An integer that determines which elements to include on the
                graph
            figure_size: Size (in inches?) of the figure. The default
                should(?) be fine
            num_steps: Number of steps within the overall time span to
                simulate. Defaults to self.num_steps
            total_time: Total timespan in seconds to simulate, INCLUDING
                collected data. Defaults to self.total_time
            base_mass: Mass of an empty rocket (without fuel). Defaults to
                self.base_mass
            drag_constant: Dimensionless constant related to the drag of the
                rocket
            diameter: Diameter of the rocket in meters,
            acceleration_error_constant: Constant that represents the maximum
                error in the accelerometer
            title: String that titles the figure
            filename: String for a filename to save to
        """
        total_time = total_time if total_time else self.total_time
        num_steps = num_steps if num_steps else self.num_steps
        drag_constant = drag_constant if drag_constant else self.drag_coefficient
        base_mass = base_mass if base_mass else self.base_mass
        diameter = diameter if diameter else self.diameter
        acceleration_error_constant = acceleration_error_constant if acceleration_error_constant else self.acceleration_error_constant

        figure = plt.figure(figsize=figure_size)
        axes = figure.add_subplot(
            1, 1, 1, xlabel=r'Time $(seconds)$', ylabel=r'Altitude $(meters)$')

        collected_data = [{
            'time': (acceleration[0] + altitude[0]) * 0.5,
            'acceleration': acceleration[1],
            'altitude': altitude[1]
        } for acceleration, altitude in zip(self.previous_acceleration, self.
                                            previous_altitude)]

        acceleration_drag = acceleration_calculator.AccelerationCalculatorDrag(
            thrust=self.thrust_values,
            mass=self.mass_values,
            base_mass=base_mass,
            drag_constant=drag_constant,
            diameter=diameter,
            collected_data=collected_data)

        if flags & GRAPH.ALTITUDE:
            axes.plot(
                list(map(operator.itemgetter(0), self.previous_altitude)),
                list(map(operator.itemgetter(1), self.previous_altitude)),
                color='blue')
            time = np.linspace(self.current_time, total_time, num=num_steps)

            time_step = time[1] - time[0]
            altitude_drag = verlet_integrator.SteppingVerletIntegrator(
                time_step,
                acceleration_drag,
                num_steps=num_steps,
                collected_data=collected_data,
                acceleration_error_constant=acceleration_error_constant)
            axes.plot(
                time[:len(altitude_drag)],
                altitude_drag,
                color='black',
                label='Simulation')

            if flags & GRAPH.ACCELEROMETER_ERROR:
                upper_error, lower_error = altitude_drag.get_accelerometer_error(
                )
                axes.plot(
                    time[:len(upper_error)],
                    upper_error,
                    linestyle='--',
                    color='orange',
                    label='Acceleration Upper Error')
                axes.plot(
                    time[:len(lower_error)],
                    lower_error,
                    linestyle='--',
                    color='orange',
                    label='Acceleration Lower Error')

        if flags & GRAPH.BURNOUT:
            axes.axvline(
                x=max(map(operator.itemgetter(0), self.thrust_values)),
                color='red')

        if flags & GRAPH.LEGEND:
            axes.legend()

        axes.set_xlim(right=20)
        axes.set_ylim(bottom=0, top=3000)
        axes.set_title(title)
        if not flags & GRAPH.SAVE_PLOT:
            figure.show()
            plt.show()
        else:
            figure.savefig(filename)
        plt.close(figure)

    def save(self,
             filename: str,
             flags: int = GRAPH.ALTITUDE,
             num_steps: int = None,
             total_time: float = None,
             base_mass: float = None,
             drag_constant: float = None,
             diameter: float = None,
             acceleration_error_constant: float = None) -> None:
        """Saves the generated curves to a csv file
        
        Args:
            flags: An integer that determines which elements to include on the
                graph
            filename: Location where to save the file
            num_steps: Number of steps within the overall time span to
                simulate. Defaults to self.num_steps
            total_time: Total timespan in seconds to simulate, INCLUDING
                collected data. Defaults to self.total_time
            base_mass: Mass of an empty rocket (without fuel). Defaults to
                self.base_mass
            drag_constant: Dimensionless constant related to the drag of the
                rocket
            diameter: Diameter of the rocket in meters,
            acceleration_error_constant: Constant that represents the maximum
                error in the accelerometer
        """
        total_time = total_time if total_time else self.total_time
        num_steps = num_steps if num_steps else self.num_steps
        drag_constant = drag_constant if drag_constant else self.drag_coefficient
        base_mass = base_mass if base_mass else self.base_mass
        diameter = diameter if diameter else self.diameter
        acceleration_error_constant = acceleration_error_constant if acceleration_error_constant else self.acceleration_error_constant

        collected_data = [{
            'time': (acceleration[0] + altitude[0]) * 0.5,
            'acceleration': acceleration[1],
            'altitude': altitude[1]
        } for acceleration, altitude in zip(self.previous_acceleration, self.
                                            previous_altitude)]

        acceleration_drag = acceleration_calculator.AccelerationCalculatorDrag(
            thrust=self.thrust_values,
            mass=self.mass_values,
            base_mass=base_mass,
            drag_constant=drag_constant,
            diameter=diameter,
            collected_data=collected_data)

        time = np.linspace(0.0, total_time, num=num_steps)
        time_step = time[1] - time[0]
        altitude_drag = verlet_integrator.SteppingVerletIntegrator(
            time_step,
            acceleration_drag,
            num_steps=num_steps,
            collected_data=collected_data,
            acceleration_error_constant=acceleration_error_constant)

        with io.open(filename, 'w', newline='\n') as file:
            writer = csv.writer(file, delimiter=' ')
            for time_value, altitude_value in zip(time, altitude_drag):
                row = [time_value]
                if flags & GRAPH.ALTITUDE:
                    row.append(altitude_value)
                if flags & GRAPH.ACCELERATION:
                    row.append(acceleration_drag(time_value))

                writer.writerow(row)
