/**
 * Entrypoint into the Arduino code
 */

#ifdef ARDUINO

#include "Arduino.h"

/**
 * Runs once on startup. Any initialization happens in this function
 */
void setup() {

}

/**
 * Loops forever until stopping. Any code that needs to be constantly run
 * should be in this function
 */
void loop() {

}

#else

/**
 * Entrypoint for native code.
 *
 * This should really never be run, since the code is meant for the teensy
 * @param argc integer representing the number of command line arguments
 * @param argv array of strings for the command line arguments
 * @returns exit code of the program
 */
int main(int argc, char* argv[]) {
  return 0;
}

#endif
