#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef LIGHT_SAMPLER_H
#define LIGHT_SAMPLER_H


/*
Get voltage of the current light level from light sensor
input: void
output: double
*/
double getLightLevelVoltage(void);

int getLightLevel();
int getLightLevelPercentage();

#endif