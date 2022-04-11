#include "fileHandler.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 1024


/*
Write to the file
input: char*, char*
output: void
*/
void setFile(char *fileName, char *value) 
{
	// initialize FILE with write mode
	FILE *file = fopen(fileName, "w");
	
	if (file == NULL) {
		printf("Error: unable to write file %s\n", fileName);
		exit(1);
	}

	fprintf(file, "%s", value);
	fclose(file);
}

/*
Copy the whole content of file
input: char*, char*, int
output: void
*/
void readFile(char *fileName, char *buff, int arrLength) {
	FILE *pFile = fopen(fileName, "r");
	if (pFile == NULL) {
		printf("ERROR: Unable to open file (%s) for read\n", fileName);
		exit(-1);
	}

	fgets(buff, arrLength, pFile);
	fclose(pFile);
}


/*
Read the file and retrieve the file content (0 or 1)
input: char*
output: char
*/
char getFileContent(char *fileName) 
{
	// initialize FILE with read mode
	FILE *file = fopen(fileName, "r");

	if (file == NULL) {
		printf("Error: unable to read from file %s\n", fileName);
	}

	char stringBuf[MAX_LENGTH];
	fgets(stringBuf, MAX_LENGTH, file);
	fclose(file);

	return stringBuf[0];
}