# Curve Generation - C

Ported C/C++ code from the [curve generation python modules](https://github.com/Longhorn-Rocketry-Association/Experimental-Board/tree/master/Curve%20Generation/Python)

# Requirements
* [Python2.7](https://www.python.org/downloads/release/python-2715/) - Python 3 will **not** work
* [virtualenv](https://virtualenv.pypa.io/en/latest/) - Isolated Python environment creation

# Setup
Once you've verified your environment satisfies the [requirements](#requirements), follow these steps to setup building and/or development for the project

1. Create the virtual environment: `$ virtualenv -p <path to Python2.7 interpreter> venv`. This will make the `venv` directory containing a copy of the interpreter as well as the dependencies for the project
2. Activate the virtual environment. This will be different depending on what operating system you're using, use [this](https://virtualenv.pypa.io/en/latest/userguide/#activate-script) guide to help you.
3. Install [platformio](https://platformio.org/) with `$ pip install -U platformio`

# Unit Testing

Run the unit tests locally (i.e. on your own computer rather than external board) with `pio test -e native`. You'll likely need to setup an account with platformio as their test runner is a part of the PIO Plus offering.

Run the unit tests for an embedded environment, run `pio test -e teensy35`. Note that I have **not** tested these functions in an embedded environment, but have no reason to believe they wouldn't work.

Consult the platformio [docs](https://docs.platformio.org/en/latest/plus/unit-testing.html#) for more information on unit testing.

# Building

`pio run` or `platformio run` builds the code for both the `teensy35` and `native` environments. See [here](https://docs.platformio.org/en/latest/userguide/index.html) for a guide and more useful commands for the platformio command line interface.
