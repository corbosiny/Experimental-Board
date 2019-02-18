#include <math.h>
#include "pid.h"
#include <stdlib.h>

float fconstrict(float f, float min, float max)
{
    return f < min ? min : (f > max ? max : f);
}

pidc pid_make(float kp, float ki, float kd)
{
    pidc controller = malloc(sizeof(struct pid_controller));
    controller->kp = kp;
    controller->ki = ki;
    controller->kd = kd;
    // NANs in imin and omin indicate unconstrained input and output,
    // respectively
    controller->imin = NAN;
    controller->omin = NAN;
}

void pid_destroy(pidc controller)
{
    free(controller);
}

float pid_update(pidc controller, float error, float timestamp)
{
    // Constrict input if configured
    if (controller->imin == controller->imin)
        error = fconstrict(error, controller->imin, controller->imax);

    controller->error_total += error;
    float dt = timestamp - controller->timestamp_last;
    float deriv = 0;

    // Only attempt derivative calculation if dt is positive and nonzero
    if (dt > 0)
        deriv = (error - controller->error_last) / dt;

    float update = controller->kp * error + controller-> ki *
                   controller->error_total + controller->kd * deriv;
    controller->error_last = error;
    controller->timestamp_last = timestamp;

    // Constrict output if configured
    if (controller->omin == controller->omin)
        update = fconstrict(update, controller->omin, controller->omax);

    return update;
}

void pid_constrain_input(pidc controller, float imin, float imax)
{
    controller->imin = imin;
    controller->imax = imax;
}

void pid_unconstrain_input(pidc controller)
{
    controller->imin = NAN;
}

void pid_constrain_output(pidc controller, float omin, float omax)
{
    controller->omin = omin;
    controller->omax = omax;
}

void pid_unconstrain_output(pidc controller)
{
    controller->omin = NAN;
}
