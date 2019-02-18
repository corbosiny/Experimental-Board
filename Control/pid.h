#ifndef PID_H
#define PID_H

struct pid_controller
{
    float kp, ki, kd;
    float error_total, error_last, timestamp_last;
    float imin, imax, omin, omax;
};

typedef struct pid_controller* pidc;

/**
    Clamps a value between some bounds with no wrap-around.

    @param f value to constrict
    @param min lower bound
    @param max upper bound
    @return constricted value
*/
float fconstrict(float f, float min, float max);

/**
    Creates a new controller with some gains. Controllers are dynamically
    allocated and must be freed either manually or with pid_destroy.

    @param kp proportional gain
    @param ki integral gain
    @param kd derivative gain
    @returns new controller
*/
pidc pid_make(float kp, float ki, float kd);

/**
    Reclaims a controller whose time has come.
*/
void pid_destroy(pidc controller);

/**
    Offers a single update and returns the controller's response.

    @param controller controller to update
    @param error current system error
    @param timestamp current system time
    @return controller update
*/
float pid_update(pidc controller, float error, float timestamp);

/**
    Constrains future input to some range.

    @param controller controller to constrain
    @param imin input lower bound
    @param imax input upper bound
*/
void pid_constrain_input(pidc controller, float imin, float imax);

/**
    Releases input constraints on a controller.

    @param controller controller to release
*/
void pid_unconstrain_input(pidc controller);

/**
    Constrains future output to some range.

    @param controller controller to constrain
    @param imin output lower bound
    @param imax output upper bound
*/
void pid_constrain_output(pidc controller, float omin, float omax);

/**
    Releases output constraints on a controller.

    @param controller controller to release
*/
void pid_unconstrain_output(pidc controller);

#endif
