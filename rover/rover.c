#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include "network.h"
#include "light_sampler.h"
#include "rover_motor.h"

#define THREAD_NUM 1

static pthread_t tids[THREAD_NUM];
static bool continueProram = true;

/*
Function to get string containing program info such as runtime
*/
static char *getProgramStatus() {
	struct sysinfo info;
	sysinfo(&info);
	
	int hour, minute, seconds, lightLevel;
	hour = (info.uptime / 3600);
	minute = (info.uptime - (3600 * hour)) / 60;
	seconds = (info.uptime - (3600 * hour)- (minute * 60));
	lightLevel = getLightLevel();
	static char status[1024];
	memset(status, 0, sizeof(char)*1024);
	snprintf(status, 1024, "update,%d:%d:%d,%d", hour, minute, seconds, lightLevel);
	return status;
}


static void *udpCommunication() {
	openConnection();
	while(continueProram) {
		char request[1024];
		receiveRequest(request, 1024);
		printf("%s\n", request);
		
		if(strcmp(request, "QUIT") == 0) {
			continueProram = false;
			break; //exit the while loop
		} else if(strcmp(request, "UPDATE") == 0) {
			char *status = getProgramStatus();
			sendResponse(status);
		} else if(strcmp(request, "MOTOR_LEFT") == 0) {
			rotateLeftMotors();
		} else if(strcmp(request, "MOTOR_RIGHT") == 0) {
			rotateRightMotors();
		} else if(strcmp(request, "MOTOR_GO") == 0) {
			turnAllMotors();
		} else if(strcmp(request, "MOTOR_STOP") == 0) {
			turnOffMotors();
		} else {
			continue;
		}
		memset(request, 0, sizeof(char)*1024);
	}
	closeConnection();
	return NULL;
}

int main() {
	printf("Initiating the Rover #1\n");

	initGpioMotor(); // initialize gpio motors

	pthread_create(&tids[0], NULL, udpCommunication, NULL);

	//Wait until threads are done and clean up memories used by threads
	for(int i = 0; i < THREAD_NUM; i++) {
		pthread_join(tids[i], NULL);
	}

	printf("Finished the Rover #1\n");
}