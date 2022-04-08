#include "sleep.h"

/*
Sleeps program using nanosleep
input: long, long
output: void
*/
void sleepNow(long seconds, long nanoseconds) 
{
	struct timespec delayReq = {seconds, nanoseconds};
	nanosleep(&delayReq, (struct timespec *) NULL);
}