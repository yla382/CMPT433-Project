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
#include "led_display.h"
#include "morsecode.h"
#include "audioMixer_template.h"
#include "audio.h"
#include "potential_meter.h"
#include "sleep.h"
// #include "temp_hum.h" - Removed due to hardware complication

#define THREAD_NUM 5

static pthread_t tids[THREAD_NUM];
static bool continueProram = true;
static char *morsecode = NULL;
static int currentLightIntensity = 0;
static int currentPotVal = 0;

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
	currentLightIntensity = getLightLevelPercentage();

	float temperature, humidity;
	temperature = 0;
	humidity = 0;
	
	// updateTempHumData(&temperature, &humidity); //Removed due to hardware complication

	static char status[1024];
	memset(status, 0, sizeof(char)*1024);
	snprintf(status, 1024, "update,%d:%d:%d,%d,%d,%d,%d,%.2f,%.2f", 
				hour, minute, seconds, lightLevel, 
				accelerometer[0], accelerometer[1], accelerometer[2],
				temperature, humidity
			);
	return status;
}

static void *playSound() {
	initialize_audio_files();
	while(continueProram) {
		if(morsecode != NULL) {
			if(strlen(morsecode) > 0) {
				printf("%s\n", morsecode);
				talk_morse_code(morsecode);
			} else {
				printf("Invalid\n");
			}
			free(morsecode);
			morsecode = NULL;
		}
	}
	
	remove_audio_files();
	printf("Ending playSound() thread\n");
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
			if(strlen(arg2) > 0) printf("%s\n", arg2);
			morsecode = getMorseCode(arg2);
		} else {
			continue;
		}
		memset(request, 0, sizeof(char)*1024);
	}
	closeConnection();
	printf("Ending udpCommunication() thread\n");
	return NULL;
}

static void *joystickThread() {
	while(continueProram) {
		getDirections();
	}
	printf("Ending joystickThread() thread\n");
	return NULL;
}

static void *display() {
	initializeDisplay();
	while(continueProram) {
		int lightIntensity = currentLightIntensity;
		int stringLen = snprintf(NULL, 0, "%d", lightIntensity);
		char *lightIntensityStr = malloc(sizeof(char) * stringLen + 1);
		
		snprintf(lightIntensityStr, stringLen + 1, "%d", lightIntensity);
		
		//show current dip count to display
		displayScreen(lightIntensityStr);
		free(lightIntensityStr);
		lightIntensityStr = NULL;
	}
	//Turn off display
	closeDisplay();
	printf("Ending display() thread\n");
	return NULL;
}

static void *updateVolumn() {
	while(continueProram) {
		int potVal = getPotReadingPercentage();
		if(currentPotVal != potVal) {
			currentPotVal = potVal;
			changeVolumn(currentPotVal);
		}
		sleepNow(0, 500000000);
	}
	printf("Ending updateVolumn() thread\n");
	return NULL;
}

int main() {
	pid_t pid = fork();

	if(pid == 0) {
		static char *argv[]={};
		execv("./capture", argv); //Run capture.c
		exit(127);
	} else {
		printf("Initiating the Rover #1\n");
		start_audio();
		initGpioMotor(); // initialize gpio motors
		initializeJoyStick();
		Accelerometer_initialize();
		
		turnOffAllLED();
		pthread_create(&tids[0], NULL, udpCommunication, NULL);
		pthread_create(&tids[1], NULL, joystickThread, NULL);
		pthread_create(&tids[2], NULL, display, NULL);
		pthread_create(&tids[3], NULL, playSound, NULL);
		pthread_create(&tids[4], NULL, updateVolumn, NULL);

		//Wait until threads are done and clean up memories used by threads
		for(int i = 0; i < THREAD_NUM; i++) {
			pthread_join(tids[i], NULL);
		}

		printf("Finished the Rover #1\n");
		Accelerometer_destroy();
		quit_audio();
		kill(pid, SIGKILL); //Terminate Child Process
	}

	return 0;
}
