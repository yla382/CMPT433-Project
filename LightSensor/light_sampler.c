// Application to read analog input voltage 1 from the light sensor on the BeagleBone
// Assumes ADC cape already loaded by uBoot

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include "light_sampler.h"
#include "sleep.h"


static int totalLightSamples = 0;
static pthread_t samplingThreadId;

void *samplingThread(void* args);

void startLightSampling(void)
{
	pthread_create(&samplingThreadId, NULL, &samplingThread, NULL);
}

void stopLightSampling(void)
{
	pthread_join(samplingThreadId, NULL);
}

void *samplingThread(void* args)
{
	while (true) {
		int reading1 = getLightSensorReading();
		double voltage1 = ((double)reading1 / A2D_MAX_READING) * A2D_VOLTAGE_REF_V;
		printf("Light Sensor Value %5d ==> %5.2fV\n", reading1, voltage1);
		
		if(totalLightSamples%5 == 0 && totalLightSamples != 0)
			printf("Read %d samples \n", totalLightSamples);
		
		totalLightSamples++;
		delay(100);
	}
}

int getTotalLightSamples(void)
{
	return totalLightSamples;
}

int getLightSensorReading(void)
{
	// Open file
	FILE *f = fopen(A2D_FILE_VOLTAGE1, "r");
	if (!f) {
		printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
		printf("       Check /boot/uEnv.txt for correct options.\n");
		exit(-1);
	}

	// Get reading
	int a2dReading = 0;
	int itemsRead = fscanf(f, "%d", &a2dReading);
	if (itemsRead <= 0) {
		printf("ERROR: Unable to read values from voltage input file.\n");
		exit(-1);
	}

	// Close file
	fclose(f);

	return a2dReading;
}



