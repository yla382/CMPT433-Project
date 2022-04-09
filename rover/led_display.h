// led_display.h
// Module to display numbers on the led_display on the zen cape

#ifndef _LED_DISPLAY_H_
#define _LED_DISPLAY_H_
#include <pthread.h>
#include <stdbool.h>

// init the I2C BUS
int initI2cBus(char* bus, int address);
// Writes to the I2C Reg
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
// Read from the I2C Reg
unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);    
// switches which led is on alternating them
void driveSwitchLight(int on);
// returns hex value of int
int* getValHex(int num, int* vals);
void ledDisplay(void);
void ledScreenStartInit();

#endif