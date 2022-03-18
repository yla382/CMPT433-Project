// light_sampler.h
// Module to sample light levels in the background (thread).
// It provides access to a reading the number of samples taken and displaying voltage levels.
// TODO: add intensity of light reading to a 7-seg display (10 being brightest)

#ifndef _SAMPLER_H_
#define _SAMPLER_H_

#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095

// Begin the background thread which samples light levels.
void startLightSampling(void);

// End the background thread which samples light levels.
void stopLightSampling(void);

// Reads total number of light samples taken
int getLightSensorReading(void);

// Returns total number of light samples taken
int getTotalLightSamples(void);

#endif

