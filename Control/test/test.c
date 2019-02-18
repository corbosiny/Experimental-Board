#include "../pid.h"
#include <stdio.h>

int main()
{
    pidc controller = pid_make(1, 0, 0);
    pid_constrain_input(controller, 0, 0.5);
    float update = pid_update(controller, 1000, 1);
    printf("%f\n", update);
    pid_destroy(controller);
}
