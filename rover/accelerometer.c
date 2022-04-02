#include "accelerometer.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x1c

#define REG_CTRL1 0x2A
#define REG_STATUS 0x00
#define REG_OUT_X_MSB 0x01
#define REG_OUT_X_LSB 0x02
#define REG_OUT_Y_MSB 0x03
#define REG_OUT_Y_LSB 0x04
#define REG_OUT_Z_MSB 0x05
#define REG_OUT_Z_LSB 0x06

static int i2cFileDesc;
static char *buf;

// helper functions from brians code
static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
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


static void readI2cReg(int i2cFileDesc, unsigned char regAddr, char* buf, int count)
{
    // To read a register, must first write the address
    int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
    if (res != sizeof(regAddr)) {
        perror("Unable to write i2c register.");
        exit(-1);
    }

    // Now read the value and return it
    res = read(i2cFileDesc, buf, count);
    if (res != count) {
        perror("Unable to read i2c register");
        exit(-1);
    }
}


static int initI2cBus(char* bus, int address)
{
    int i2cFileDesc = open(bus, O_RDWR);

    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }
    return i2cFileDesc;
}




void Accelerometer_initialize() 
{
    i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    writeI2cReg(i2cFileDesc, REG_CTRL1, 0x01); // set to active
    buf = malloc(sizeof(char)*7);
}

int *Accelerometer_getReadings()
{  
    readI2cReg(i2cFileDesc, REG_STATUS, buf, 7);

    int16_t x = ((buf[1] << 8) | (buf[2]))>>4;
    if (x > 2047) x -= 4096;

    int16_t y = ((buf[3] << 8) | (buf[4]))>>4;
    if (y > 2047) y -= 4096;

    int16_t z = ((buf[5] << 8) | (buf[6]))>>4;
    z -= 1024;

    if (y > 2047) y -= 4096;
    int *status = malloc(3*sizeof(int));
    status[0] = x;
    status[1] = y;
    status[2] = z;
    return status;
}


void Accelerometer_destroy() {
    close(i2cFileDesc);
}