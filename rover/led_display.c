#include "led_display.h"

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x20
#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

#define GPIO_EXPORT "/sys/class/gpio/export"

#define GPIO_61_DIRECTION "/sys/class/gpio/gpio61/direction"
#define GPIO_61_VALUE "/sys/class/gpio/gpio61/value"

#define GPIO_44_DIRECTION "/sys/class/gpio/gpio44/direction"
#define GPIO_44_VALUE "/sys/class/gpio/gpio44/value"

//Number of allowed characters for 14-seg display: 0-9
#define DIGIT_LEN 10

//8-bits representation of numbers 0-9 for lower part of display
//example bottom_segs[4] is for display number 4
static unsigned char bottom_segs[DIGIT_LEN] = {
    0xA1, 0x80, 0x31, 0xB0, 0x90,
    0xB0, 0xB1, 0x04, 0xB1, 0x90 
};

//8-bits representation of numbers 0-9 for upper part of display
//example top_segs[4] is for display number 4
static unsigned char top_segs[DIGIT_LEN] = {
    0x86, 0x12, 0x0F, 0x06, 0x8A,
    0x8C, 0x8C, 0x14, 0x8E, 0x8E
};

//File descriptor for controlling zer cape's 14 seg display
static int i2cFileDesc;


/*
Function to write to the file
input: *char, *char
output: void
*/
static void setFile(char *fileName, char *value) {
    // initialize FILE with write mode
    FILE *file = fopen(fileName, "w");
    
    if (file == NULL) { //exit program if unable open file
        printf("Error: unable to write file %s\n", fileName);
        exit(1);
    }

    fprintf(file, "%s", value);
    fclose(file);
}


/*
enable communication between beaglebone and 14-seg display
input: void
output: void
*/
static void configureI2C() {
    if (system("config-pin P9_17 i2c") > -1 && system("config-pin P9_18 i2c") > -1) {
        return;
    } else { //exit program if communication failed to enabled
        printf("Failed to configure pins to i2c\n");
        exit(1);
    }
}


/*
set slave device to read from I2c Bus
input: *char, int
output: int
*/
static int initI2cBus(char* bus, int address) {
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    
    if (result < 0) { //exit program if Unable to set I2C device to slave address
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }
    return i2cFileDesc;
}


/*
Write to zencape's 14 seg display
input: unsigned char, unsighed char
output: void
*/
static void writeI2cReg(unsigned char regAddr, unsigned char value) {
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}


/*
Initialize I2C setting to control 14 seg display. Turn off display,
input: void
output: void
*/
void initializeDisplay() {
    //Set GPIO 44 and 61 to write
    setFile(GPIO_EXPORT, "44");
    setFile(GPIO_EXPORT, "61");

    setFile(GPIO_44_DIRECTION, "out");
    setFile(GPIO_61_DIRECTION, "out");

    //enable communication between beaglebone and 14-seg display
    configureI2C();

    //set slave device to read from I2c Bus
    i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    writeI2cReg(REG_DIRA, 0x00);
    writeI2cReg(REG_DIRB, 0x00);
}


/*
Turn off display by settting values in 
GPIO 44 and 61 to zero close I2C connection
input: void
output: void
*/
void closeDisplay() {
    setFile(GPIO_44_VALUE, "0");
    setFile(GPIO_61_VALUE, "0");

    close(i2cFileDesc);
}


/*
Display numbers on 14 seg display
input: *char
output: void
*/
void displayScreen(char *value) {
    //convert string to int
    int value_int = atoi(value);

    unsigned char leftBottom;
    unsigned char leftTop;
    unsigned char rightBottom;
    unsigned char rightTop;

    //Set correct register values for the given number
    //if number is less than 10, display 0 on left side of display
    //ex. display 6 as 06
    if (value_int < 10) {
        leftBottom = bottom_segs[0];
        leftTop = top_segs[0];
        rightBottom = bottom_segs[value_int];
        rightTop = top_segs[value_int];

    } else if(10 <= value_int && value_int < 100) {
        //if number is between 10 and 99, display as it is
        char leftDigit[2] = {value[0], 0};
        char rightDigit[2] = {value[1], 0};

        leftBottom = bottom_segs[atoi(leftDigit)];
        leftTop = top_segs[atoi(leftDigit)];
        rightBottom = bottom_segs[atoi(rightDigit)];
        rightTop = top_segs[atoi(rightDigit)];

    } else {
        //if number is 100 or greater, set to 99
        leftBottom = bottom_segs[9];
        leftTop = top_segs[9];
        rightBottom = bottom_segs[9];
        rightTop = top_segs[9];
    }


    //sleep for 5ms
    long seconds = 0;
    long nanoseconds = 5000000;
    struct timespec delayReq = {seconds, nanoseconds};

    //Turn off both side of the display
    setFile(GPIO_44_VALUE, "0");
    setFile(GPIO_61_VALUE, "0");

    //Set display value for left number
    writeI2cReg(REG_OUTB, leftTop);
    writeI2cReg(REG_OUTA, leftBottom);

    //Turn on left side of the display
    setFile(GPIO_61_VALUE, "1");

    //sleep for 5ms
    nanosleep(&delayReq, (struct timespec *) NULL);

    //Turn off both side of the display
    setFile(GPIO_44_VALUE, "0");
    setFile(GPIO_61_VALUE, "0");

    //Set display value for right number
    writeI2cReg(REG_OUTB, rightTop);
    writeI2cReg(REG_OUTA, rightBottom);

    //Turn on right side of the display
    setFile(GPIO_44_VALUE, "1");

    // //sleep for 5ms
    nanosleep(&delayReq, (struct timespec *) NULL);
}