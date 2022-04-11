#include "temp_hum.h"
#include "fileHandler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bbb_dht_read.h"

#define SENSOR 22
#define BASE 0
#define NUMBER 2

void updateTempHumData(float *temperature, float *humidty) {
	printf("Temperature: %f\n", *temperature);
	printf("Temperature: %f\n", *humidty);
	bbb_dht_read(SENSOR, BASE, NUMBER, humidty, temperature);
	printf("Temperature: %f\n", *temperature);
	printf("Temperature: %f\n", *humidty);
}
