#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"
#include "constantes.h"


//Print the cube in binary form into f
int printCube(float cube[],FILE *f)
{
	int res = fwrite(cube,sizeof(float),WIDTH,f);
	//verifySumCube(cube);
	return res;
}

void readCube_i(int position)
{
 	FILE *f;
 	f = fopen("img/histograms", "rb");
	float buffer[WIDTH];

	fseek(f,position*sizeof(buffer),SEEK_SET);
	int res=fread(buffer,sizeof(float),WIDTH,f);
	
	if(res==0)
	{
		printf("\n ERROR : can't read histogramm %i\n",position);
	}
	else
	{
		verifySumCube(buffer);	
	}
	fclose(f);
}


int readNextCube(FILE *f,float buffer[])
{
	int res =fread(buffer,sizeof(float),WIDTH,f);
	if(res!=0) // on n'affiche pas s'il n'y avait rien à lire
	{
		//verifySumCube(buffer);
	}
	return res;
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
	printf("%.20f\n\n",sum);

}

