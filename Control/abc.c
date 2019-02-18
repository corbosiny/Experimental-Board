#include "abc.h"
#include "pid.h"
#include <stdlib.h>

abc abc_make(float altitude_target, float kp, float ki, float kd)
{
    abc controller = malloc(sizeof(struct air_brake_controller));
    pidc pid = pid_make(kp, ki, kd);
    controller->pid = pid;
    controller->altitude_target = altitude_target;
    return controller;
}

void abc_destroy(abc controller)
{
    free(controller);
}

float abc_update(abc controller, float altitude_min, float altitude_max,
                 float timestamp)
{
    float midpoint = altitude_min + (altitude_max - altitude_min) / 2;
    float error = midpoint - controller->altitude_target;
    return pid_update(controller->pid, error, timestamp);
}
