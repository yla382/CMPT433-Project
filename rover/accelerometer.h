// acceleromter.h
// Module to sample accelerometer in a background thread.
// Provides the direction (X, Y, Z) the accelerometer is indicatating
#ifndef _ACCELEROMETER_H_
#define _ACCELEROMETER_H_

#include <pthread.h>

// enum type to define accelerometer movements
typedef enum {
	DIRECTION_NONE = 0,
	DIRECTION_X,
	DIRECTION_Y,
	DIRECTION_Z,
} DIRECTION;


// initializes the accelerometer registers
void Accelerometer_initialize(void);

// sampels accelerometer readings
// records x y z movement
int *Accelerometer_getReadings(void);

void Accelerometer_destroy(void);

#endif