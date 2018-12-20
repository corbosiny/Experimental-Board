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

Running `$ main.py --help` prints out the generic help output with argument placement and available flags

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