#include "morsecode.h"

#define MAX_LEN 1024

//Morse code look up table
static char* morsecode[] = {
		".-", // A
		"-...", // B
		"-.-.", // C
		"-..", // D
		".", // E
		"..-.", // F
		"--.", // G
		"....", // H
		"..", // I
		".---", // J
		"-.-", // K
		".-..", // L
		"--", // M
		"-.", // N
		"---", // O
		".--.", // P
		"--.-", // Q
		".-.", // R
		"...", // S
		"-", // T
		"..-", // U
		"...-", // V
		".--", // W
		"-..-", // X
		"-.--", // Y
		"--..", // Z
};


/*
Helper function to check if the character is a whitespace
input: char
output: bool
*/
static bool isWhiteSpace(char ch)
{
	return (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t');
}


/*
Helper function to check if character is alphabet
input: char
output: bool
*/
static bool isAlphabet(char ch)
{
	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

char *getMorseCode(char *str) {
	char *morseCode = malloc(MAX_LEN * sizeof(char));
	memset(morseCode, 0, MAX_LEN);
	int morseCodeIndex = 0;
	int whiteSpaceCount = 0;
	for(int i = 0; str[i] != '\0'; i++) {
		bool isCharWhiteSpace = isWhiteSpace(str[i]);

		if(i == 0 && isCharWhiteSpace) continue;
		if(i == strlen(str) - 1 && isCharWhiteSpace) continue;

		if(!isCharWhiteSpace) {
			if(!isAlphabet(str[i])) continue;

			int ascii = str[i];
            int ascii_flag = 0;

            if (ascii >= 65 && ascii <= 90) { //check for upper case alphabet
                ascii = str[i] - 65;
                ascii_flag = 1;

            } else if (ascii >= 97 && ascii <= 122 && ascii_flag == 0) { //check for lower case alphabet
                ascii = str[i] - 97;
                ascii_flag = 1;
            }

            char *morse = morsecode[ascii];

            int morse_size;
			morse_size = 0;
			for(int k = 0; morse[k] != '\0'; k++) {
				morse_size++;
			}


			for(int j = 0; j < morse_size; j++) {
				morseCode[morseCodeIndex] = morse[j];
				morseCodeIndex++;
			}

			morseCode[morseCodeIndex] = ' ';
			morseCodeIndex++;

			whiteSpaceCount = 0;

		} else {
			if(whiteSpaceCount == 0) {
				morseCode[morseCodeIndex] = ' ';
				morseCodeIndex++;
			}
			whiteSpaceCount++;
		}
	}
	
	return morseCode;
}