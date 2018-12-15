"""Defines various plotting functions that take in rocket engine data"""

from typing import List, Dict, Iterable
import operator
import matplotlib.pyplot as plot
import numpy as np

from calculate import acceleration_calculator
import verlet_integrator


def plot_thrust_curve(data: List[Dict[str, float]], title=None) -> None:
    """Plots the thrust curve from the given engine data

    Args:
        data: List of data points that contain t and f parameters (for time and
            thrust, respectively)
    """
    fig = plot.figure()

    axes = fig.add_subplot(1, 1, 1, xlabel=r'$t$', ylabel=r'Force')
    if title:
        axes.set_title(title)

    time = list(map(lambda x: x['t'], data))
    force = list(map(lambda x: x['f'], data))

    axes.plot(
        time,
        force,
        color='black',
        marker='o',
        markerfacecolor='red',
        markeredgecolor='red',
        markersize=2.25,
        linewidth=2)
    axes.set_ylim(bottom=0.0)

    fig.show()


def plot_altitude_curve(
        engine_data: List[Dict[str, float]],
        drag_constant: float = 0.0,
        base_mass: float = 0.0,
        title: str = None,
        total_time: float = 30.0,
        num_steps: int = 50,
        diameter: float = 0.0,
        collected_data: Iterable[Dict[str, float]] = []) -> None:
    """Plots the altitude curve.

    Plots out the altitude curve from the given data without any consideration
    of error. Also assumes no air brakes.

    Args:
        engine_data: A list of dictionaries that account for various data at a
            specific time t.
        drag_constant: constant used when calculating drag for different
            velocities.
        base_mass: mass of an empty rocket.
        title: optional title for the figure.
        total_time: amount of time to simulate the altitude for. Highly
            dependent on the acceleration for how long this should be.
        num_steps: Number of steps to use, which influences the timestep.
    """

    engine_data.sort(key=operator.itemgetter('t'))

    thrust_values = [(x['t'], x['f']) for x in engine_data]
    mass_values = [(x['t'], x['m']) for x in engine_data]
    acceleration_drag = acceleration_calculator.AccelerationCalculatorDrag(
        thrust=thrust_values,
        mass=mass_values,
        base_mass=base_mass,
        drag_constant=drag_constant,
        diameter=diameter,
        collected_data=collected_data)

    acceleration = acceleration_calculator.AccelerationCalculator(
        thrust=thrust_values,
        mass=mass_values,
        base_mass=base_mass,
        drag_constant=drag_constant,
        diameter=diameter,
        collected_data=collected_data)

    fig = plot.figure(figsize=(12.8, 9.6))
    axes = fig.add_subplot(
        1, 1, 1, xlabel=r'Time $(s)$', ylabel=r'Altitude $(m)$')

    time = np.linspace(0, total_time, num=num_steps)
    time_step = time[1]

    altitude_drag = verlet_integrator.SteppingVerletIntegrator(
        time_step,
        acceleration_drag,
        num_steps=num_steps,
        collected_data=collected_data)
    altitude = verlet_integrator.SteppingVerletIntegrator(
        time_step,
        acceleration,
        num_steps=num_steps,
        collected_data=collected_data)

    # axes.plot(time, altitude, color='blue', label=r'$C_d = 0.0$')
    axes.plot(
        time,
        altitude_drag,
        color='black',
        label=r'$C_d = ' + str(drag_constant) + r'$')

    axes.legend()
    axes.axhline(color='black')
    axes.axvline(
        x=max(map(operator.itemgetter('t'), engine_data)), color='red')

    axes.set_ylim(bottom=0)
    axes.set_title(title if title != None else 'Altitude')
    fig.show()


# def plot_acceleration(
#     engine_data: List[Dict[str, float]],
#     drag_constant: float = 0.0,
#     base_mass: float = 0.0):
#     """Plots acceleration

#     Plots out an acceleration curve without respecting error

#     Args:
#         engine_data: A list of dictionaries that account for various data at a
#             specific time t
#         drag_constant: constant used when calculating drag for different
#             velocities
#         base_mass: mass of an empty rocket
#     """

#     engine_data.sort(key=operator.itemgetter('t'))
#     acceleration_calculator = calculate.acceleration_calculator.AccelerationCalculator(
#         time=list(map(operator.itemgetter('t'), engine_data)),
#         thrust=list(map(operator.itemgetter('f'), engine_data)),
#         mass=list(map(operator.itemgetter('m'), engine_data)),
#         base_mass=base_mass,
#         drag_constant=drag_constant)

#     fig = plot.figure(figsize=(12.8, 9.6))
#     axes = fig.add_subplot(1, 1, 1,
#                            xlabel=r'$t$ $(s)$',
#                            ylabel=r'Acceleration $(\frac{m}{s^2})$')

#     time = np.linspace(0, 8.0)
#     axes.axhline(-9.81, color='red', linewidth=2)

#     axes.plot(time, acceleration_calculator(time), color='black')

#     axes.axhline(color='black')

#     fig.show()
