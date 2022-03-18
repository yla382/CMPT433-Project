#include <time.h>

void delay(int milisecs)
{
	long seconds = 0;
	long nanoseconds = milisecs*1000000;
	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}

