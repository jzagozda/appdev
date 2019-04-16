#include <stdio.h>
#include <time.h>
#include <stdlib.h> // for random numbers
#include "screen.h"
#include "sound.h"
#include "comm.h"
#include <signal.h>
int main(int argc, char ** argv)
{
	if(argc == 2)
	{
		printf("Test tone generator\n");
		int fR, fL, ch;
		float duration;
		printf("No. of channels (1 or 2): ");
		scanf("%d", &ch);

		if(ch == 1)
		{
			printf("Mono Frequency: ");
			scanf("%d", &fL);
		}
		else if(ch == 2)
		{
			printf("Give me Left and Rigth freq: ");
			scanf("%d %d", &fL, &fR);
		}
		else
		{
			printf("Wrong number of channels\n");
			return 1;
		}

		printf("How long is the test tone? (1-10 sec):");
		scanf("%f", &duration);
		testTone(ch, fL, fR, duration);
		return 0;
	}
	FILE *f;
	short sd[RATE];
	for(;;){
		int ret = system(RCMD);
		if(ret == SIGINT) break;
		f = fopen("test.wav","r");
		if(f == NULL){
			printf("Cannot open a file\n");
			return 1;
		}
		clearScreen();
		setColors(RED, bg(YELLOW));
		struct WAVHDR hdr;
		fread(&hdr, sizeof(hdr),1,f); //read WAV header
		fread(&sd,sizeof(sd),1,f); //This will read WAV data
		fclose(f);
		displayWAVHDR(hdr);
		displayWAVDATA(sd);
		sendData(sd);
	}
	resetColors();
}
