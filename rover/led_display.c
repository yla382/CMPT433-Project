#include "led_display.h"
#include "light_sampler.h"
#include "sleep.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <sys/stat.h>

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define GPIO44 "/sys/class/gpio/gpio44/value"
#define GPIO61 "/sys/class/gpio/gpio61/value"


#define I2C_DEVICE_ADDRESS 0x20

const char VAL_HEX_BOT[] = {0x86, 0x12, 0x0e, 0x6, 0x8a, 0x8c, 0x8c, 0x14, 0x8e, 0x8e, 0x86};
const char VAL_HEX_TOP[] = {0xe1, 0xc0, 0x31, 0xf0, 0x90, 0xf0, 0xb1, 0x04, 0xb1, 0x90, 0xe1};

#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

int i2cFileDesc;
int sign;
int *vals;

void ledScreenStartInit() {
    system("config-pin P9_18 i2c");
    system("config-pin P9_17 i2c");
    system("i2cset -y 1 0x20 0x00 0x00");
    system("i2cset -y 1 0x20 0x01 0x00");
    system("echo 61 > /sys/class/gpio/export");
    system("echo 44 > /sys/class/gpio/export");
    system("echo out > /sys/class/gpio/gpio61/direction");
    system("echo out > /sys/class/gpio/gpio44/direction");
    system("echo 1 > /sys/class/gpio/gpio61/value");
    system("echo 1 > /sys/class/gpio/gpio44/value");
    i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    sign = getLightLevelPercentage();
    vals = malloc(sizeof(int)*2);
    vals = (getValHex(sign, vals));
}

void ledDisplay() {
    // printf("%d %d", vals[1], vals[0]);
    // turn off both digits
    writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
    writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);

    driveSwitchLight(1);

    //display left digit
    writeI2cReg(i2cFileDesc, REG_OUTA, VAL_HEX_TOP[vals[1]]);
    writeI2cReg(i2cFileDesc, REG_OUTB, VAL_HEX_BOT[vals[1]]);
    sleepNow(0,5000000); 
    // turn off both digits
    writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
    writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);

    driveSwitchLight(0);

    //display right digit
    writeI2cReg(i2cFileDesc, REG_OUTA, VAL_HEX_TOP[vals[0]]);
    writeI2cReg(i2cFileDesc, REG_OUTB, VAL_HEX_BOT[vals[0]]);

    sleepNow(0,5000000); 
    vals = (getValHex(sign, vals));
    sign = getLightLevelPercentage();
}

int initI2cBus(char* bus, int address)
{
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(-1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		perror("Unable to set I2C device to slave address.");
		exit(-1);
	}
	return i2cFileDesc;
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("Unable to write i2c register");
		exit(-1);
	}
}

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		exit(-1);
	}

	// Now read the value and return it
	char value = 0;
	res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		exit(-1);
	}
	return value;
}

void driveSwitchLight(int on) {
    FILE *gpio44FILE = fopen(GPIO44, "w");
    if (gpio44FILE == NULL) {
        printf("ERROR: Unable to open a file for read\n");
        exit(-1);
    }
    if (on == 1) {
        fprintf(gpio44FILE, "1");
    } else {
        fprintf(gpio44FILE, "0");
    }
    fclose(gpio44FILE);

    FILE *gpio61FILE = fopen(GPIO61, "w");
    if (gpio61FILE == NULL) {
        printf("ERROR: Unable to open a file for read\n");
        exit(-1);
    }
    if (on == 1) {
        fprintf(gpio61FILE, "0");
    } else {
        fprintf(gpio61FILE, "1");
    }
    fclose(gpio61FILE);
}

int* getValHex(int num, int* vals) {
    if (num < 10) {
        vals[0] = 0;
        vals[1] = num;
        return vals;
    }

    if (num > 99) {
        num = 99;
    }
    vals[1] = num % 10;
    vals[0] = (int)(num/pow(10, 1));
    return vals;
}