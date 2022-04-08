#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "joyStickControl.h"
#include "sleep.h"
#include "fileHandler.h" // importing file read/write methods
#include "rover_motor.h"

// Number of possible events for GPIO joystick
#define MAX_EVENTS 5

// GPIO joystick direction setting value
#define DIRECTION_SETTING    "in"

// GPIO joystick trigger setting value
#define EDGE_TRIGGER_SETTING "both" //"falling"

#define GPIO_EXPORT "/sys/class/gpio/export"

// GPIO 65(left) related file locations
#define GPIO_DIRECTION_LEFT  "/sys/class/gpio/gpio65/direction"
#define GPIO_VALUE_LEFT      "/sys/class/gpio/gpio65/value"
#define GPIO_TRIGGER_LEFT    "/sys/class/gpio/gpio65/edge"

// GPIO 47(right) related file locations
#define GPIO_DIRECTION_RIGHT "/sys/class/gpio/gpio47/direction"
#define GPIO_VALUE_RIGHT     "/sys/class/gpio/gpio47/value"
#define GPIO_TRIGGER_RIGHT    "/sys/class/gpio/gpio47/edge"

// GPIO 26(up) related file locations
#define GPIO_DIRECTION_UP 	 "/sys/class/gpio/gpio26/direction"
#define GPIO_VALUE_UP        "/sys/class/gpio/gpio26/value"
#define GPIO_TRIGGER_UP      "/sys/class/gpio/gpio26/edge"

// GPIO 46(down) related file locations
#define GPIO_DIRECTION_DOWN	 "/sys/class/gpio/gpio46/direction"
#define GPIO_VALUE_DOWN      "/sys/class/gpio/gpio46/value"
#define GPIO_TRIGGER_DOWN    "/sys/class/gpio/gpio46/edge"

// GPIO 27(down) related file locations
#define GPIO_DIRECTION_CENTER  "/sys/class/gpio/gpio27/direction"
#define GPIO_VALUE_CENTER      "/sys/class/gpio/gpio27/value"
#define GPIO_TRIGGER_CENTER    "/sys/class/gpio/gpio27/edge"


/*
Export all joystick gpio pins
input: void
output: void;
*/
void initializeJoyStick() 
{
	//Add timer later
	setFile(GPIO_EXPORT, "65");
	sleepNow(0, 300000000);
	setFile(GPIO_EXPORT, "47");
	sleepNow(0, 300000000);
	setFile(GPIO_EXPORT, "26");
	sleepNow(0, 300000000);
	setFile(GPIO_EXPORT, "46");
	sleepNow(0, 300000000);
	setFile(GPIO_EXPORT, "27");
	sleepNow(0, 300000000);
}


/*
Return the direction of joystick pressed
input: void
output: enum
*/
void getDirections() 
{
	// set joystick direction for input
	setFile(GPIO_DIRECTION_LEFT, DIRECTION_SETTING);
	setFile(GPIO_DIRECTION_RIGHT, DIRECTION_SETTING);
	setFile(GPIO_DIRECTION_UP, DIRECTION_SETTING);
	setFile(GPIO_DIRECTION_DOWN, DIRECTION_SETTING);
	setFile(GPIO_DIRECTION_CENTER, DIRECTION_SETTING);

	char leftVal = getFileContent(GPIO_VALUE_LEFT);
	char rightVal = getFileContent(GPIO_VALUE_RIGHT);
	char upVal = getFileContent(GPIO_VALUE_UP);
	char downVal = getFileContent(GPIO_VALUE_DOWN);
	char centerVal = getFileContent(GPIO_VALUE_CENTER);
	if (leftVal == '0') {
		rotateLeftMotors();
	} else if (rightVal == '0') {
		rotateRightMotors();
	} else if (upVal == '0') {
		turnAllMotors();
	} else if (downVal == '0') {
		turnOffMotors();
	} else if (centerVal == '0') {
		// toggle screen?
	}
}
