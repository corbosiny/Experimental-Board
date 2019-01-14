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
