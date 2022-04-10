#include "temp_hum.h"
#include "fileHandler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TEMPERATURE_FILE "temp_hum/tempData.txt"
#define HUMIDITY_FILE "temp_hum/humData.txt"

#define MAX_LENGTH 1024

static double stringToDouble(char *str) {
	char *ptr;
	return strtod(str, &ptr);
}


double getTemperature() {
	char tempString[MAX_LENGTH];
	memset(tempString,0, MAX_LENGTH);
	readFile(TEMPERATURE_FILE, tempString, MAX_LENGTH);

	return stringToDouble(tempString);
}


double getHumidity() {
	char humString[MAX_LENGTH];
	memset(humString,0, MAX_LENGTH);
	readFile(HUMIDITY_FILE, humString, MAX_LENGTH);

	return stringToDouble(humString);
}