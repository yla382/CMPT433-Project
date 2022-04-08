#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define LED0_PATH_TRIG "/sys/class/leds/beaglebone:green:usr0/trigger"   // up
#define LED1_PATH_TRIG "/sys/class/leds/beaglebone:green:usr1/trigger"
#define LED2_PATH_TRIG "/sys/class/leds/beaglebone:green:usr2/trigger"   // down
#define LED3_PATH_TRIG "/sys/class/leds/beaglebone:green:usr3/trigger"

#define LED0_PATH_BRIGHT "/sys/class/leds/beaglebone:green:usr0/brightness"   // up
#define LED1_PATH_BRIGHT "/sys/class/leds/beaglebone:green:usr1/brightness"
#define LED2_PATH_BRIGHT "/sys/class/leds/beaglebone:green:usr2/brightness"   // down
#define LED3_PATH_BRIGHT "/sys/class/leds/beaglebone:green:usr3/brightness"

// go into led brightness folder and change value
void changeLED(char *ledPath, char *trigPath, bool on) {
    FILE *ptrigFile = fopen(trigPath, "w");
    if (ptrigFile == NULL) {
        printf("ERROR: Unable to open a file for read\n");
        exit(-1);
    }
    fprintf(ptrigFile, "none");
    fclose(ptrigFile);

    FILE *pledFile = fopen(ledPath, "w");
    if (pledFile == NULL) {
        printf("ERROR: Unable to open a file for read\n");
        exit(-1);
    }
    if (on) {
        fprintf(pledFile, "1");
    } else {
        fprintf(pledFile, "0");
    }
    fclose(pledFile);
}

void turnOffAllLED() {
    changeLED(LED0_PATH_BRIGHT, LED0_PATH_TRIG, false);
    changeLED(LED1_PATH_BRIGHT, LED1_PATH_TRIG, false);
    changeLED(LED2_PATH_BRIGHT, LED2_PATH_TRIG, false);
    changeLED(LED3_PATH_BRIGHT, LED3_PATH_TRIG, false);
}

void turnOnAllLED() {
    changeLED(LED0_PATH_BRIGHT, LED0_PATH_TRIG, true);
    changeLED(LED1_PATH_BRIGHT, LED1_PATH_TRIG, true);
    changeLED(LED2_PATH_BRIGHT, LED2_PATH_TRIG, true);
    changeLED(LED3_PATH_BRIGHT, LED3_PATH_TRIG, true);
}

void turnOnGoLED() {
    turnOffAllLED();
    changeLED(LED1_PATH_BRIGHT, LED1_PATH_TRIG, true);
    changeLED(LED2_PATH_BRIGHT, LED2_PATH_TRIG, true);
}

void turnOnRightLED() {
    turnOffAllLED();
    changeLED(LED3_PATH_BRIGHT, LED3_PATH_TRIG, true);
}

void turnOnLeftLED() {
    turnOffAllLED();
    changeLED(LED0_PATH_BRIGHT, LED0_PATH_TRIG, true);
}