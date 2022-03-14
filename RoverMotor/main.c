#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include "rover_motor.h"

int main()
{   
    initGpioMotor();
    turnAllMotors();
    sleep_ms(1000);
    turnOffMotors();
    printf("test");
    return 0;
}