#ifndef FILEHANDLER_H
#define FILEHANDLER_H

/*
Write to the file
input: char*, char*
output: void
*/
void setFile(char *fileName, char *value);

void readFile(char *fileName, char *buff, int arrLength);

/*
Read the file and retrieve the file content
input: char*
output: char
*/
char getFileContent(char *fileName);

#endif