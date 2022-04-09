#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>


/*
Initialize audio play using audioMixer_template
input: void
output: void
*/
void start_audio();


/*
End audio play using audioMixer_template
input: void
output: void
*/
void quit_audio();


/*
Copy audio files to memory
input: void
output: void
*/
void initialize_audio_files();


/*
Remove audio files from memory
input: void
output: void
*/
void remove_audio_files();



/*
Adds wavedata to buffer to play rock beat 2
input: void
output: void
*/
void talk_morse_code(char *str);



/*
Increase volumn by 5 is input is true else descrease volumn by 5
if volumn + 5 is > MAX_VOLUMN then set volumn to MAX_VOLUMN
if volumn - 5 is < MIN_VOLUMN then set volumn to MIN_VOLUMN
input: bool
output: void
*/
void changeVolumn(int changePercentage);

#endif