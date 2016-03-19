#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"
#include "constantes.h"


//Print the cube in binary form into f
void printCube(float cube[],FILE *f)
{
	fwrite(cube,sizeof(float),WIDTH,f);
}

void readCube_i(int position)
{
 	FILE *f;
 	f = fopen("img/histograms", "r");
	float buffer[WIDTH];

	fseek(f,position*sizeof(buffer),SEEK_SET);
	fread(buffer,sizeof(buffer),1,f);	
	
	fclose(f);
}



void readNextCube(FILE *f)
{
	float buffer[WIDTH];	
	fread(buffer,sizeof(buffer),1,f);
	verifySumCube(buffer);
}

void verifySumCube(float buffer[])
{
	float sum=0;
	for (int i=0;i<WIDTH;i++)
	{
		sum+=buffer[i];
		printf("%f ",buffer[i]);
		if((i+1)%16==0 && i!=0)
		{
		printf("\n");
		}
	}
	printf("%f\n\n",sum);
}

