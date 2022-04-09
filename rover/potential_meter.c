#include "potential_meter.h"

#define A2D_FILE_VOLTAGE0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095

/*
Get current raw value of the potentiometer
input: void
output: int
*/
static int getPotReading() {
	FILE *file = fopen(A2D_FILE_VOLTAGE0, "r");

	//exit program if unable to read file, exit the program
	if (file == NULL) {
		printf("Error: unable to open in_voltage0_raw file\n");
		exit(-1);
	}

	int potReading = 0;
	int inputNum = fscanf(file, "%d", &potReading); //get raw current pot value
	//if nothing in read from the file, exit the program
	if(inputNum <= 0) {
		printf("Error: UInable to read values from the in_voltage0_raw file\n");
		exit(-1);
	}

	fclose(file);
	return potReading;
}

int getPotReadingPercentage(void) {
	int val =  ((double) getPotReading() / A2D_MAX_READING) * 100;
	return val;
}
