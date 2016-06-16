#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES // for C Visual C++
#include <math.h>
#include <stdint.h>

float noteFrequency[] = { //8 element
	440.01,	//A, la
	493.88,	//B, si
	523.25,	//C, do
	587.33,	//D, re
	659.25,	//E, mi
	698.46,	//F, fa
	783.99,	//G, sol
	0		//rest
};

int* toNumbers(char *notes);
void createWAVFile(char *filename, int noOfSamples, int totalNotes, int8_t *data, int sampleRate);

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
	
	uint8_t *wave = (uint8_t*) malloc(sizeof(uint8_t) * noOfSamples * totalNotes);
	
	int selectedNote, noteSample;
	
	for(selectedNote = 0; selectedNote < totalNotes; selectedNote++)
	{
		float selectedNoteFreq = noteFrequency[notes[selectedNote]];
		for(noteSample = 0; noteSample < noOfSamples; noteSample++)
		{
			wave[selectedNote * noOfSamples + noteSample] = 128 + 128 * sin(2.0 * M_PI * noteSample * selectedNoteFreq / sampleRate);
		}
	}

	FILE *fl = fopen("answer.pcm", "wb");
	
	fwrite(wave, sizeof(int8_t), noOfSamples * totalNotes, fl);
	fclose(fl);
	
	createWAVFile("answer.wav", noOfSamples, totalNotes, wave, sampleRate);
	
	free(wave);
	free(numbers);
	
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

void createWAVFile(char *filename, int noOfSamples, int totalNotes, int8_t *data, int sampleRate)
{
	//write RIFF marker
	FILE *fl = fopen(filename, "wb");
	fwrite("RIFF", sizeof(char), 4 * sizeof(char), fl);
	//write filesize
	uint32_t fileSize = (36 + noOfSamples * totalNotes * sizeof(int16_t));
	fwrite(&fileSize, sizeof(uint32_t), 1, fl);
	//write WAVE header
	fwrite("WAVE", sizeof(char), 4 * sizeof(char), fl);
	//writes fmt
	fwrite("fmt ", sizeof(char), 4 * sizeof(char), fl);
	//writes Subchunk1Size
	uint32_t subchunk1Size = ((uint32_t) 16);	
	fwrite(&subchunk1Size, sizeof(uint32_t), 1, fl);
	//writes type of format.I am using pcm so it will be 1
	uint16_t channeSize = ((int16_t) 1);
	fwrite(&channeSize, sizeof(uint16_t), 1, fl);
	//writes channel which i will use 1
	uint16_t type = ((int16_t) 1);
	fwrite(&type, sizeof(uint16_t), 1, fl);
	//writes sample rate
	uint32_t sampleRateLE = (sampleRate);
	fwrite(&sampleRateLE, sizeof(uint32_t), 1, fl);
	//writes byte rate = SampleRate * NumChannels * BitsPerSample/8
	uint32_t byteRate = (sampleRate * 1 * 8 / 8);
	fwrite(&byteRate, sizeof(uint32_t), 1, fl);
	//writes BlockAlign = NumChannels * BitsPerSample/8
	uint16_t blockAlign = (1 * 8 / 8);
	fwrite(&blockAlign, sizeof(uint16_t), 1, fl);
	//writes bits per sample
	uint16_t bps = (8);
	fwrite(&bps, sizeof(uint16_t), 1, fl);
	//writes 'data' chunk header.
	fwrite("data", sizeof(char), 4 * sizeof(char), fl);
	//writes size of data chunk
	uint32_t dataChunkSize = ((uint32_t) noOfSamples * totalNotes * 1 * 8 / 8);
	fwrite(&dataChunkSize, sizeof(uint32_t), 1, fl);
	//writes data
	fwrite(data, sizeof(int8_t), noOfSamples * totalNotes, fl);

	fclose(fl);
}

