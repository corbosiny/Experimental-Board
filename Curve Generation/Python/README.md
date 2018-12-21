# Curve Generation - Python

This is a collection of python modules for visualizing rocket simulation and testing algorithms

## Requirements
1. [Python](https://www.python.org/) version 3.6+
2. [virtualenv](https://virtualenv.pypa.io/en/latest/) This is an optional requirement, but read [this](https://virtualenv.pypa.io/en/latest/#introduction) before deciding not to use it

## Setup
Once you've installed the [requirements](#requirements), you'll need to change your working directory to the directory containing this README. If you're using virtual environments, you'll want to run these commands first:
1. `$ virtualenv venv` This creates a virtual environment for Python within this directory. Call it `venv` since this directory should be ignored by [git](https://git-scm.com/) as written in the `.gitignore`
2. `$ source venv/bin/activate` This will activate the virtual environment. Any dependencies you install while in this virtual environment won't pollute your global Python installation. You should see some indication in your command line that says you're in the `(venv)` virtual environment.
3. When you're done using the virtual environment, run `$ deactivate` as described in the [virtualenv usage](https://virtualenv.pypa.io/en/latest/userguide/#usage) documentation

Even if you're not using virtual environments, you'll still need to make sure you have [pip](https://pip.pypa.io/en/stable/) installed. You can double check by following [these](https://pip.pypa.io/en/stable/installing/) instructions but most Python installations comes with pip by default.

Once you can run pip commands, from this directory run `$ pip install -r requirements.txt`. Now Python should have everything it needs to run the code in this directory.

## Usage

Running `$ main.py --help` prints out the help output with argument placement and available flags. Currently, `main.py` won't perform any calculation without the `-f` flag.

The classes and methods can be used by themselves in isolation. Run python in interactive mode, import the desired modules/packages, and run the code. Documentation can be found using the `__doc__` attribute on each function/method/module itself, or just read from the source code.

The more convenient way to use the utilities provided by this code is to define the actions you want performed in a file and use the `-f` flag. This would look like `$ main.py -f input.json` if `input.json` was within your current directory.  The schema for this input file is defined [here](https://github.com/Longhorn-Rocketry-Association/Experimental-Board/blob/master/Curve%20Generation/Python/schema/input.schema.json), but a more user-friendly description of how to construct this file is defined below.

The input file must be valid [JSON](https://www.json.org/). The top level object must be an array, with each item being an object identifying an action and its variables. Each individual action and its variables, required or not, are described below, but these are the variables required for all actions:

| Variable | Type | Description | Required |
| --- | --- | --- | :---: |
| `acceleration_error_constant` | `float` | A constant error associated with the accelerometer. Influences the graph of the accelerometer error curves. `acceleration` must be present in the `errors` array. | No
| `base_mass` | `float` | Specifies the mass of an empty rocket in kilograms. | No
| `drag_coefficient` | `float` | Specifies the dimensionless constant associated with [this](https://en.wikipedia.org/wiki/Drag_equation) drag equation for the rocket. | No
| `engine_file` | `string` | A path to a file specifying the properties of the engine. Currently only [RockSim](https://www.apogeerockets.com/Rocket_Software/RockSim) formatted files are supported. | Yes
| `errors` | `array` | An array of strings determining what errors to include. Only `acceleration` and `gyro` are currently accepted, but `acceleration` is the only one implemented. Note that most errors expressed in this array will required additional parameters. |No
| `gyro_error_constant` | `float` | An error associated with the gyroscope. Influences the graph of the gyroscope error curve | No
| `initial_altitude` | `float` | The initial altitude of the rocket in meters. This won't factor into the graph (i.e., the graph's y-axis minimum will still be zero) but *will* factor into drag calculations | No
| `num_steps` | `int` | The number of intervals to calculate in the graph. The larger the number, the more accurate the simulation at the cost of calculation time. | No
| `total_time` | `float` | Total amount of time to simulate. | No





## Contributing

A few things to anyone who'd like to contribute to this codebase:

- Try to follow the style guide [here](https://github.com/google/styleguide/blob/gh-pages/pyguide.md) as much as possible. You can use [yapf](https://github.com/google/yapf) on your edited files to fix some of the more cumbersome rules, such as line length.
- Be sure to write [type hints](https://www.python.org/dev/peps/pep-0484/) for method parameters and return values.
- Read [this](https://chris.beams.io/posts/git-commit/) before writing commit messages.
- When writing methods that take in parameters that correspond to important scientific values, be sure to include the expected dimensions in the description of the parameter.
  
    Example:
    ```
    def calculate_density(height: float) -> float:
      """Calculates the density from the given height
        
        Args:
          height: (meters)
           
        Returns:
          float
        """
        ...
   ```
   Be sure to include something like (meters) or any other specific dimension used in the method