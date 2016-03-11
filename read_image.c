#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "rdjpeg.h"

#define SIZE 4
#define WIDTH (SIZE*SIZE*SIZE)
#define COL 4 //le nombre de valeurs à afficher avant un retour à la ligne

void init( int cube[],int length);
void colorSection(int cube[], CIMAGE cim);
void printCube(int cube[],int nbCol,FILE*f);
void createHistograms(char * fileToRead);
void processOneFile(char * stringIn);


int main(int argc, char *argv[])
{
	createHistograms("img/list.txt");
	exit(0);
}

void createHistograms(char * fileToRead)
{
	char stringIn[100];	
	FILE *in;
	in = fopen(fileToRead, "r");
	if(in == NULL)
	{
		printf("\nImpossible de lire le fichier %s\n",fileToRead);
		exit(0);
	}
	
	while (fgets(stringIn, 100, in) != NULL) 
	{
		if(stringIn[strlen(stringIn) - 1] == '\n')
			{stringIn[strlen(stringIn) - 1] = '\0';}
  		printf("\nLecture en cours : %s",stringIn);
		processOneFile(stringIn);
	}
	fclose(in);
}

//Prend un nom de fichier, le lit et créer son histogramme de couleur dans img/hist
void processOneFile(char * stringIn)
{
	FILE *out;
	CIMAGE cim;
	char toRead[100]="img/images/";
	char toCreate[100]="img/hist/";
	int cube[WIDTH];
	init(cube,WIDTH);
	
	strcat(toCreate,stringIn);
	out = fopen(toCreate, "w");
	if(out == NULL)
	{
		printf("\nImpossible de créer le fichier %s\n",toCreate);
		exit(0);
	}
	
	strcat(toRead,stringIn);
	read_cimage(toRead,&cim);
	
	colorSection(cube,cim);
	printCube(cube,COL,out);
	
	fclose(out);
}

//Affiche le contenu d'un histogramme avec un saut de ligne toutes les nbCol multiples
void printCube(int cube[],int nbCol,FILE*f)
{
   for (int j = 0; j < WIDTH; j++)
   {
   		fprintf(f,"%i ",cube[j]);
   		if(j!=0 && (j+1)%nbCol==0)
   		{
   			fprintf(f,"\n");
   		}
   }
}

//Initialise un histogramme en le remplissant de 0
void init( int cube[],int length)
{
   for (int j = 0; j < length; j++)
   {
   		cube[j]=0;
   }
}

//Rempli le contenu d'un histogramme à partir d'une CIMAGE
void colorSection(int cube[], CIMAGE cim)
{
	int k;
	for (int i = 0; i < cim.nx; i++) 
	{      
		for (int j = 0; j < cim.ny; j++) 
		{ 
			k = cim.r[i][j]/WIDTH + SIZE*cim.g[i][j]/WIDTH + SIZE*SIZE*cim.b[i][j]/WIDTH;
			cube[k]++;
		}
	}
}


 
