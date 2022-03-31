#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#define RELAY_SWITCH_GPIO   "/sys/class/gpio/gpio"
#define EXPORT_FILE         "/sys/class/gpio/export"
#define RELAY_ON            "1"
#define RELAY_OFF           "0"
#define RELAY_VALUE         "/value"
#define MOTOR_PIN_1 70
#define MOTOR_PIN_2 71
#define MOTOR_PIN_3 72
#define MOTOR_PIN_4 73

#define BUFF_SIZE 1024

void sleep_ms(unsigned int delayMs)
{
	const unsigned int NS_PER_MS = 1000 * 1000;
	const unsigned int NS_PER_SECOND = 1000000000;

	unsigned long long delayNs = delayMs * NS_PER_MS;
	int seconds = delayNs / NS_PER_SECOND;
	int nanoseconds = delayNs % NS_PER_SECOND;

	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}

void driveMotor(int motorNum, char *on1OrOff0)
{
	char fileName[BUFF_SIZE];
	sprintf(fileName, "%s%d%s", RELAY_SWITCH_GPIO, motorNum, RELAY_VALUE);
	FILE *pMotorGo = fopen(fileName, "w");
	fprintf(pMotorGo, "%s", on1OrOff0);
	fclose(pMotorGo);
}

static void exportGpioSetOff(int linuxPinNumber) {
    char buff[BUFF_SIZE];

    FILE *pExportFile = fopen(EXPORT_FILE, "w");
	if (!pExportFile) {
		printf("Error opening file %s\n", EXPORT_FILE);
		exit(1);
	}
	fprintf(pExportFile, "%d", linuxPinNumber);
	fclose(pExportFile);

    // Allow enough time for GPIO export to complete
	sleep_ms(300);

	// Set direction (output)
	sprintf(buff, "/sys/class/gpio/gpio%d/direction", linuxPinNumber);
	FILE *pDirectionFile = fopen(buff, "w");
	if (!pDirectionFile) {
		printf("Error opening file %s\n", buff);
		exit(1);
	}
	fprintf(pDirectionFile, "out");
	fclose(pDirectionFile);
}

void initGpioMotor() {
	printf("init gpio motor\n");
    exportGpioSetOff(MOTOR_PIN_1);
    exportGpioSetOff(MOTOR_PIN_2);
    exportGpioSetOff(MOTOR_PIN_3);
    exportGpioSetOff(MOTOR_PIN_4);
}

void turnAllMotors() {
	printf("on motor\n");
    driveMotor(MOTOR_PIN_1, RELAY_ON);
    driveMotor(MOTOR_PIN_2, RELAY_ON);
    driveMotor(MOTOR_PIN_3, RELAY_ON);
    driveMotor(MOTOR_PIN_4, RELAY_ON);
}

void turnOffMotors() {
	printf("off motor\n");
    driveMotor(MOTOR_PIN_1, RELAY_OFF);
    driveMotor(MOTOR_PIN_2, RELAY_OFF);
    driveMotor(MOTOR_PIN_3, RELAY_OFF);
    driveMotor(MOTOR_PIN_4, RELAY_OFF);
}

void rotateLeftMotors() {
	printf("left motor\n");
	turnOffMotors();
    driveMotor(MOTOR_PIN_1, RELAY_ON);
    driveMotor(MOTOR_PIN_2, RELAY_ON);
}

void rotateRightMotors() {
	printf("right motor\n");
	turnOffMotors();
    driveMotor(MOTOR_PIN_3, RELAY_ON);
    driveMotor(MOTOR_PIN_4, RELAY_ON);
}
