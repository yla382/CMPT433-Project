#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H


/*
Initialize I2C setting to control 14 seg display. Turn off display,
input: void
output: void
*/
void initializeDisplay();

/*
Turn off display by settting values in 
GPIO 44 and 61 to zero close I2C connection
input: void
output: void
*/
void closeDisplay();

/*
Display numbers on 14 seg display
input: *char
output: void
*/
void displayScreen(char *value);


#endif