#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include "network.h"

#define THREAD_NUM 1

static pthread_t tids[THREAD_NUM];
static bool continueProram = true;

/*
Function to get string containing program info such as runtime
*/
static char *getProgramStatus() {
	struct sysinfo info;
	sysinfo(&info);
	
	int hour, minute, seconds;
	hour = (info.uptime / 3600);
	minute = (info.uptime - (3600 * hour)) / 60;
	seconds = (info.uptime - (3600 * hour)- (minute * 60));
	
	static char status[40];
	memset(status, 0, sizeof(char)*40);
	snprintf(status, 40, "update,%d:%d:%d", hour, minute, seconds);
	return status;
}


static void *udpCommunication() {
	openConnection();
	while(continueProram) {
		char request[1024];
		receiveRequest(request, 1024);
		
		if(strcmp(request, "QUIT") == 0) {
			continueProram = false;
			break; //exit the while loop
		} else if(strcmp(request, "UPDATE") == 0) {
			char *status = getProgramStatus();
			sendResponse(status);
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

	pthread_create(&tids[0], NULL, udpCommunication, NULL);

	//Wait until threads are done and clean up memories used by threads
	for(int i = 0; i < THREAD_NUM; i++) {
		pthread_join(tids[i], NULL);
	}

	printf("Finished the Rover #1\n");
}