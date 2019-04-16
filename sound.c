#include <stdio.h>
#include <math.h>
#include "sound.h"
#include "screen.h"

//This function gets in an array of decibel values and finds out
//the number of peaks in this array
int findPeaks(int d[]){
	int i, c = 0;
	for(i = 1; i < 80; i++){
		if(d[i] >= 78 && d[i-1] < 75) c++;
	}
	if(d[0] >= 75) c++;
	return c;
}
// this function takes 1 second of samples (16000 in our
// case) and calcualte 80 pieces of RMS value, and then
// turn these values into decibels, and diplay them as
// a barchart
void displayWAVDATA(short s[]){
	double rms[80];		// because we have 16000 samples, 16000/80=200
						// therefore every 200 samples makes one RMS
	int db[80];			// for decibel values
	int i, j;			// nested loop counters
	short *ptr = s;		// use pointer point to the beginning of the samples

	for(i=0; i<80; i++){	// outer loop repeats 80 times
		double sum = 0;		// initialize the sum
		for(j=0; j<200; j++){
			sum += (*ptr)*(*ptr);	// accumulate the sum
			ptr++;					// pointer increments
		}
		rms[i] = sqrt(sum/200);
		db[i] = 20*log10(rms[i]);	// decibel value
#ifdef DEBUG						// conditional compilation
		printf("RMS[%d] = %f\n", i, rms[i]);
#endif
	}	// end of for
#ifndef DEBUG
	barChart(db);
	int peaks = findPeaks(db);
	setColors(WHITE, bg(BLACK));
	printf("\033[1;41H");
	printf("Peaks: %d            \n", peaks);
#endif
}

void showID(char *name, char *value){
	printf("%s: ", name);
	for(int i=0; i<4; i++)
		printf("%c", value[i]);
	puts("");		// \n
}

// function definition
void displayWAVHDR(struct WAVHDR h){
#ifdef DEBUG
	showID("Chunk ID", h.ChunkID);
	printf("Chunk size: %d\n", h.ChunkSize);
	showID("Format", h.Format);
	showID("Subchunk1 ID", h.Subchunk1ID);
	printf("Subchunk1 size: %d\n", h.Subchunk1Size);
	printf("Audio format: %d\n", h.AudioFormat);
	printf("Num of Channels: %d\n", h.NumChannels);
	printf("Sample rate: %d\n", h.SampleRate);
	printf("Byte rate: %d\n", h.ByteRate);
	printf("Block align: %d\n", h.BlockAlign);
	printf("Bits per sample: %d\n", h.BitsPerSample);
	showID("Subchunk 2 ID: ", h.Subchunk2ID);
	printf("Subchunk 2 size: %d\n", h.Subchunk2Size); 
#else
	setColors(RED, bg(BLUE));
	printf("\033[1;1H");
	printf("test.wav            \n");
	setColors(YELLOW, bg(GREEN));
	printf("\033[1;21H");
	printf("Sample rate=%d      \n", h.SampleRate);
	setColors(WHITE, bg(MAGENTA));
	printf("\033[1;61H");
	printf("Duration=%.2f       \n", (float)h.Subchunk2Size/h.ByteRate);
#endif
}

void fillID(char *dst, const char *m)
{
	for(int i = 0; i < 4; i++)
	{
		*dst++ = *m++;
	}
}

void testTone(int channel, int frequencyLeft, int frequencyRight, float duration)
{
	if(frequencyLeft < 30 || frequencyLeft > 16000)
	{
		printf("Frequency is out of range.\n");
		return;
	}
	if(channel < 1 || channel > 2)
	{
		printf("The number of channels is incorrect.\n");
		return;
	}
	if(duration < 1 || duration > 10)
	{
		printf("Duration is incorrect.\n");
		return;
	}
	int samples=44100*duration;
	struct WAVHDR h; //We need to prepare a WAV header
	fillID(h.ChunkID, "RIFF");
	fillID(h.Format, "WAVE");
	fillID(h.Subchunk1ID, "fmt ");
	fillID(h.Subchunk2ID, "data");
	h.Subchunk1Size = 16;
	h.AudioFormat = 1;
	h.NumChannels = channel;
	h.SampleRate = 44100;
	h.BitsPerSample = 16;
	h.ByteRate = 44100*channel*16/8;
	h.BlockAlign = channel * 16 / 8;
	h.Subchunk2Size = samples * channel * 16 / 8;
	h.ChunkSize = h.Subchunk2Size + 36;
	//Prepare sound data
	FILE *f = fopen("TestTone.wav", "w");
	if(f == NULL)
	{
		printf("Cannot open the file");
		return;
	}
	fwrite(&h, sizeof(h), 1, f);
	for(int i=0; i<samples; i++)
	{
		short sL = 32767.0 * sin(2*PI*frequencyLeft*i/44100);
		fwrite(&sL, sizeof(short), 1, f);
		if(channel == 2)
		{
			short sR = 32767.0 * sin(2*PI*frequencyRight*i/44100);
			fwrite(&sR, sizeof(short), 1, f);
		}
	}
//	fwrite(data, duration * h.SampleRate * sizeof(short), 1, f);
	fclose(f);
	printf("TestTone is generated\n");
}
