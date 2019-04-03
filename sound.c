#include <stdio.h>
#include <math.h>
#include "sound.h"
void showID(char *idname, char *id){
	int i;
	printf("%s: ", idname);
	for(i = 0; i < 4; i++){
		printf("%c",id[i]);
	}
	puts("");
}
//This function gets 1 second of samples and calculates
//80 pieces of decibel value
//We need to calculate one decibel value from 200 samples
//decibel is calculated by RMS formula
void displayWAVDATA(short s[]){
	double rms[80];
	short *ptr = s; //we use a pointer and point to the beginning of an array
	int i,j; //for nested loop counters, outer loop repeats 80 times
	for(i = 0; i < 80; i++){ //inter loop repeats for 200 times
		double sum = 0;
		for(j = 0; j < 200; j++ ){
			sum += (*ptr) * (*ptr);
			ptr++;
		}
		rms[i] = sqrt(sum/200);
		printf("rms[%d] = %f\n", i, rms[i]);
	}
}
void displayWAVHDR(struct WAVHDR h){

	showID("ChunkID",h.ChunkID);
	printf("Chunk size: %d\n", h.ChunkSize);
	showID("Format", h.Format);
	showID("Subchunk1ID", h.Subchunk1ID);
	printf("Subchunk1 size: %d\n", h.Subchunk1Size);
	printf("Audio Format: %d\n", h.AudioFormat);
	printf("Num. of channels: %d\n", h.NumChannels);
	printf("Sample rate: %d\n", h.SampleRate);
	printf("Byte rate: %d\n", h.ByteRate);
	printf("Block align: %d\n", h.BlockAlign);
	printf("Bits per sample: %d\n", h.BitsPerSample);
	showID("Subchunk2ID", h.Subchunk2ID);
	printf("Subchunk2 size: %d\n", h.Subchunk2Size);
	//to be continued in other fields
}
