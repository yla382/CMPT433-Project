#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "light_sampler.h"


int main(int argc, char* args[])
{
	startLightSampling();
	printf("Main program starts \n");	
	stopLightSampling();
	printf("Sampling data finished \n");
	
	return 0;
}
