#include "audio.h"
#include "audioMixer_template.h"
#include <stdio.h>
#include <time.h>

#define NANO_SEC_CONVERSION 1000000000

//Volumn range for audio [0, 100]
#define MAX_VOLUMN 100
#define MIN_VOLUMN 0

#define BEEP      "rover_sounds/beep.wav"
#define LONG_BEEP "rover_sounds/long_beep.wav"

static wavedata_t beep;
static wavedata_t long_beep;

//Current tempo status
static int tempo = 150;

/*
Initialize audio play using audioMixer_template
input: void
output: void
*/
void start_audio()
{
    AudioMixer_init();
}

/*
End audio play using audioMixer_template
input: void
output: void
*/
void quit_audio() 
{
    AudioMixer_cleanup();
}

/*
Copy audio files to memory
input: void
output: void
*/
void initialize_audio_files() 
{
    AudioMixer_readWaveFileIntoMemory(BEEP, &beep);
    AudioMixer_readWaveFileIntoMemory(LONG_BEEP, &long_beep);
}

/*
Remove audio files from memory
input: void
output: void
*/
void remove_audio_files() 
{
    AudioMixer_freeWaveFileData(&beep);
    AudioMixer_freeWaveFileData(&long_beep);
}

/*
Wait function to make audio play in tempo
quarter note 4, 8th note 8, 16th note 16, 32nd note 32
input: int, int
output: void
*/
static void waitTempo(int tempo, int note) 
{
    float waitNanoSecond = (240 / (float)(tempo * note)) * NANO_SEC_CONVERSION;
    struct timespec reqDelay = {0, (long) waitNanoSecond};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void talk_morse_code(char *str) 
{
    char prev = '!';
    for(int i = 0; str[i] != '\0'; i++) {
        if(str[i] == '.') {
            AudioMixer_queueSound(&beep);
            waitTempo(tempo, 16);
        }

        if(str[i] == '-') {
            AudioMixer_queueSound(&long_beep);
            waitTempo(tempo, 4);
            waitTempo(tempo, 4);
        }

        if(str[i] == ' ') {
            if(prev == '.') {
                waitTempo(tempo, 4);
            }
        }

        waitTempo(tempo, 16);
        prev = str[i];
    }
}

/*
Increase volumn by 5 is input is true else descrease volumn by 5
if volumn + 5 is > MAX_VOLUMN then set volumn to MAX_VOLUMN
if volumn - 5 is < MIN_VOLUMN then set volumn to MIN_VOLUMN
input: bool
output: void
*/
void changeVolumn(int changePercentage) 
{
    int newVolumn = changePercentage;
    AudioMixer_setVolume(newVolumn);
}