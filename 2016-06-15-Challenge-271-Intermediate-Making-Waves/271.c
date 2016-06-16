#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES // for C Visual C++
#include <math.h>
#include <stdint.h>

float noteFrequency[] = { //8 element
	440.00,	//A, la
	493.88,	//B, si
	523.25,	//C, do
	587.33,	//D, re
	659.25,	//E, mi
	698.46,	//F, fa
	783.99,	//G, sol
	0		//rest
};

int* toNumbers(char *notes);

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		exit(-1); //Arguments size need to be 3;
	}
	
	int sampleRate = atoi(argv[1]);
	float noteLength = (float) atoi(argv[2]) / 1000;
	int *notes = toNumbers(argv[3]);
	int totalNotes = strlen(argv[3]);
	
	int noOfSamples = (int) (noteLength * sampleRate);
	
	int8_t *wave = (uint8_t*) malloc(sizeof(uint8_t) * noOfSamples * totalNotes);
	
	int selectedNote, noteSample;
	
	for(selectedNote = 0; selectedNote < totalNotes; selectedNote++)
	{
		for(noteSample = 0; noteSample < noOfSamples; noteSample++)
		{
			wave[selectedNote * noOfSamples + noteSample] = 128 * sin(2 * M_PI * noteSample * sampleRate / noteFrequency[notes[selectedNote]]);
		}
	}

	FILE *fl = fopen("answer.pcm", "wb");;
	
	fwrite(wave, sizeof(int8_t), noOfSamples * totalNotes, fl);
	fclose(fl);
	
	return 0;
}

int* toNumbers(char *notes)
{
	int length = strlen(notes);
	int *numbers = (int*) malloc(sizeof(int) * length);
	
	int i;
	for(i = 0; i < length; i++)
	{
		int a = notes[i] - 'A';
		numbers[i] = a < 7 ? a : 7;
	}
	
	
	return numbers;
}

