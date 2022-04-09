#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include "network.h"
#include "light_sampler.h"
#include "rover_motor.h"
#include "accelerometer.h"
#include "joyStickControl.h"
#include "leds.h"
#include "morsecode.h"

#define THREAD_NUM 3

static pthread_t tids[THREAD_NUM];
static bool continueProram = true;
static char *morsecode = NULL;

static int extractRequest(char *str, char **arg1, char **arg2) {
	char separator[] = ":"; 
	//get first string by converting first space into 0
	char *sepPointer = strtok(str, separator);
	*arg1 = sepPointer;
	
	int sepCount = 1;
	bool arg2Defined = false;
	//loop through rest of the input to separte string by space
	while(sepPointer != NULL) {
		sepPointer = strtok(NULL, separator);
		if(!arg2Defined) {
			arg2Defined = true;
			*arg2 = sepPointer;
		}
		sepCount++;
	}

	return sepCount;
}

/*
Function to get string containing program info such as runtime
*/
static char *getProgramStatus() {
	struct sysinfo info;
	sysinfo(&info);
	int* accelerometer = Accelerometer_getReadings();
	
	int hour, minute, seconds, lightLevel;
	hour = (info.uptime / 3600);
	minute = (info.uptime - (3600 * hour)) / 60;
	seconds = (info.uptime - (3600 * hour)- (minute * 60));
	lightLevel = getLightLevel();
	static char status[1024];
	memset(status, 0, sizeof(char)*1024);
	snprintf(status, 1024, "update,%d:%d:%d,%d,%d,%d,%d", hour, minute, seconds, lightLevel, accelerometer[0], accelerometer[1], accelerometer[2]);
	return status;
}

static void *playSound() {
	//initialize_audio_files();

	while(continueProram) {
		if(morsecode != NULL && strlen(morsecode) > 0) {
			printf("%s\n", morsecode);
			free(morsecode);
			morsecode = NULL;
		}
	}

	//remove_audio_files();
	return NULL;
}


static void *udpCommunication() {
	openConnection();
	while(continueProram) {
		char request[1024];
		receiveRequest(request, 1024);
		
		char *arg1 = NULL;
		char *arg2 = NULL;

		extractRequest(request, &arg1, &arg2);
		
		if(strcmp(arg1, "QUIT") == 0) {
			continueProram = false;
			break; //exit the while loop
		} else if(strcmp(arg1, "UPDATE") == 0) {
			char *status = getProgramStatus();
			sendResponse(status);
		} else if(strcmp(arg1, "MOTOR_LEFT") == 0) {
			rotateLeftMotors();
		} else if(strcmp(arg1, "MOTOR_RIGHT") == 0) {
			rotateRightMotors();
		} else if(strcmp(arg1, "MOTOR_GO") == 0) {
			turnAllMotors();
		} else if(strcmp(arg1, "MOTOR_STOP") == 0) {
			turnOffMotors();
		} else if(strcmp(arg1, "TEXT") == 0) {
			morsecode = getMorseCode(arg2);
		} else {
			continue;
		}
		memset(request, 0, sizeof(char)*1024);
	}
	closeConnection();
	return NULL;
}

static void *joystickThread() {
	while(continueProram) {
		getDirections();
	}
	return NULL;
}

int main() {
	pid_t pid = fork();
	if(pid == 0) {
		static char *argv[]={};
		execv("./capture", argv);
		exit(127);
	} else {
		printf("Initiating the Rover #1\n");
		initGpioMotor(); // initialize gpio motors
		initializeJoyStick();
		Accelerometer_initialize();
		turnOffAllLED();
		pthread_create(&tids[0], NULL, udpCommunication, NULL);
		pthread_create(&tids[1], NULL, joystickThread, NULL);
		pthread_create(&tids[2], NULL, playSound, NULL);

		//Wait until threads are done and clean up memories used by threads
		for(int i = 0; i < THREAD_NUM; i++) {
			pthread_join(tids[i], NULL);
		}

		printf("Finished the Rover #1\n");
		Accelerometer_destroy();
		kill(pid, SIGKILL);
	}

	return 0;
}