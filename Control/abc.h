#ifndef ABC_H
#define ABC_H

#include "pid.h"

struct air_brake_controller
{
    pidc pid;
    float altitude_target;
};

typedef struct air_brake_controller* abc;

/**
    Creates a new air brake controller.

    @param altitude_target target apogee
    @param kp internal PID proportional gain
    @param ki " integral gain
    @param kd " derivative gain
*/
abc abc_make(float altitude_target, float kp, float ki, float kd);

/**
    Shortcut for reallocating an abc.

    @param controller controller to destroy
*/
void abc_destroy(abc controller);

/**
    Fetches a single update from the controller given projected min and
    max altitudes. Returned values should be treated as either servo
    position increments or targets. Positive updates indicate more braking,
    negative updates indicate less.

    @param altitude_min altitude lower bound (in the event of full brake)
    @param altitude_max altitude upper bound (no brake)
    @param timestamp current system time
    @return brake update
*/
float abc_update(abc controller, float altitude_min, float altitude_max,
                 float timestamp);

#endif
