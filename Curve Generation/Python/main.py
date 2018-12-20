#!/usr/bin/env python
"""Entry point into the "application". Really a convenience to the user"""

import argparse
import random
import json
import jsonschema
import io
import csv
import os.path
from typing import List, Dict, Any, IO

import data_loader
from graph import graph_altitude


def main() -> None:
    """Starting function within the application"""
    parser = create_argparser()
    args = parser.parse_args()

    if hasattr(args, 'f') and args.f != None:
        schema = load_schema(args.s) if hasattr(
            args, 's') and args.s != None else load_schema()
        actions = json.load(args.f)
        try:
            jsonschema.Draft7Validator(schema).validate(actions)
            parse_actions(actions)
            args.f.close()
        except (jsonschema.exceptions.SchemaError,
                jsonschema.exceptions.ValidationError) as err:
            raise err
        except ValueError as err:
            raise ValueError('invalid input from file: ' +
                             str(args.f.name)) from err


def create_argparser() -> argparse.ArgumentParser:
    """Defines the argument parser for collecting command line arguments

    Returns:
        the argparser object
    """
    parser = argparse.ArgumentParser(
        description='Utilities for rocket simulation analysis')
    parser.add_argument(
        '-f',
        type=argparse.FileType(),
        help='a JSON file of a list of actions')
    parser.add_argument(
        '-s',
        type=str,
        help='a JSON schema to validate against file input')
    return parser


def parse_actions(actions: List[Dict[str, Any]]) -> None:
    """Parses the given actions

    Args:
        actions: List of dictionaries that represent actions. Usually read in
            from JSON file

    Raises:
        ValueError: Raised when the format of the JSON is wrong
    """
    for action in actions:
        parse_action(action)


def parse_action(action: Dict[str, Any]) -> None:
    """Parses the given action

    Args:
        action: Dictionary that represents an action

    Raises:
        ValueError: Raised if the format of the given dictionary is incorrect.
    """
    if not 'action' in action:
        raise ValueError('no action is defined')
    action_type = action['action']

    data_filename = action['engine_file']
    data = data_loader.read_rock_sim(data_filename)

    thrust_values = [(x['t'], x['f']) for x in data]
    mass_values = [(x['t'], x['m']) for x in data]
    grapher = graph_altitude.AltitudeGrapher(
        thrust_values=thrust_values, mass_values=mass_values, **action)

    if action_type == 'plot_rocket':

        flags = graph_altitude.GRAPH.ALTITUDE | graph_altitude.GRAPH.BURNOUT | graph_altitude.GRAPH.LEGEND
        if 'errors' in action and 'acceleration' in action['errors']:
            flags |= graph_altitude.GRAPH.ACCELEROMETER_ERROR
        grapher.plot(flags=flags)

    elif action_type == 'save_rocket':
        flags = graph_altitude.GRAPH.ALTITUDE | graph_altitude.GRAPH.ACCELERATION
        grapher.save(action['filename'], flags=flags)

    elif action_type == 'generate_flight':
        previous_altitude = []
        previous_acceleration = []
        flags = graph_altitude.GRAPH.ALTITUDE | graph_altitude.GRAPH.BURNOUT | graph_altitude.GRAPH.SAVE_PLOT
        if 'errors' in action and 'acceleration' in action['errors']:
            flags |= graph_altitude.GRAPH.ACCELEROMETER_ERROR

        with io.open(action['data_file'], 'r', newline='\n') as file:
            reader = csv.reader(file, delimiter=' ')
            csv_iterator = iter(reader)

            grapher.previous_altitude = previous_altitude
            grapher.previous_acceleration = previous_acceleration
            grapher.plot(
                flags=flags,
                filename=os.path.join(action['result_directory'], '00000.png'))
            for index, row in enumerate(csv_iterator):
                previous_altitude.append((float(row[0]), float(
                    row[1]) + random.uniform(-1, 1) * action['random_scale']))
                previous_acceleration.append((float(row[0]), float(
                    row[2]) + random.uniform(-1, 1) * action['random_scale']))

                grapher.plot(
                    flags=flags,
                    filename=os.path.join(action['result_directory'],
                                          '{:0>5}.png'.format(str(index + 1))))


def load_schema(filename: str = os.path.join('schema', 'input.schema.json')) -> Dict[str, Any]:
    """Loads and returns the JSON schema used for input validation

    Args:
        filename: Optional parameter to specify a different schema

    Returns:
        python dictionary containing the schema
    """
    schema = None
    with io.read(os.path.abspath(os.path.join(os.path.dirname(__file__), filename))) as file:
        schema = json.load(file)
    return schema


if __name__ == '__main__':
    main()
