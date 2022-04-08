#include <stdio.h>
#include <time.h>

#ifndef SLEEP_H
#define SLEEP_H

/*
Sleeps program using nanosleep
input: long, long
output: void
*/
void sleepNow(long seconds, long nanoseconds);

#endif