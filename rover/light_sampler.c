#include "light_sampler.h"

#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095

/*
Get raw value of current light level from light sensor
input: void
output: int
*/
static int getLightLevelReading() {
	FILE *file = fopen(A2D_FILE_VOLTAGE1, "r");
	//exit program if unable to read file, exit the program
	if (file == NULL) {
		printf("Error: unable to open in_voltage1_raw file\n");
		exit(-1);
	}

	int lightLevelReading = 0;
	//get raw current light sensor value
	int inputNum = fscanf(file, "%d", &lightLevelReading);
	if(inputNum <= 0) {
		//if nothing in read from the file, exit the program
		printf("Error: UInable to read values from the in_voltage1_raw file\n");
		exit(-1);
	}

	fclose(file);
	return lightLevelReading;
}

int getLightLevelPercentage() {
	int val = ((double) getLightLevelReading() / A2D_MAX_READING) * 100;
	return val;
}
/*
Get voltage of the current light level from light sensor
input: void
output: double
*/
double getLightLevelVoltage() {
	return ((double) getLightLevelReading() / A2D_MAX_READING) * A2D_VOLTAGE_REF_V;
}

int getLightLevel() {
	return getLightLevelReading();
}