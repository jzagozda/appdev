#include <stdio.h>
#include <time.h>
#include <stdlib.h> // for random numbers
#include "screen.h"
int main()
{
	int decibels[COL], i; //80pieces of some decibels
	srand(time(NULL));
	for(i=0; i<COL; i++) decibels[i] = rand()%70+30;
	clearScreen();
	setColors(RED, bg(YELLOW));
	barChart(decibels);
//	printf("Message from main\n");
	resetColors();
	printf("Message from main\n");

}
