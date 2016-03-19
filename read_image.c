#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "rdjpeg.h"
#include "functions.h"
#include "constantes.h"

#define SIZE 4
#define WIDTH (SIZE*SIZE*SIZE)
#define COL 4 //le nombre de valeurs à afficher avant un retour à la ligne

void init( float cube[],int length);
void colorSection(float cube[], CIMAGE cim);
void createHistograms(char * fileToRead);
void processOneFile(char * stringIn, FILE*outFile);
void normalise(float cube[],float nbPixel);
void readAllCubes();

int main(int argc, char *argv[])
{
	createHistograms("img/list.txt");
	readAllCubes();
	exit(0);
}

//normalise le cube pour transformer les valeurs entre 0 et 1
void normalise(float cube[],float nbPixel)
{ 
	for (int i = 0; i < WIDTH; i++) 
	{      
		cube[i]/=nbPixel;
	}
}

void readAllCubes()
{
	int i=1;
 	FILE *in;
	in = fopen("img/histograms", "r");
	while (!feof(in))
	{
		printf("Reading line : %i\n",i);
		readNextCube(in);
		i++;
	}
	fclose(in);
}


//construit la totalité des histogrammes à partir d'un fichier contenant une liste de noms de fichiers
void createHistograms(char * fileToRead)
{
	char stringIn[100];	
	FILE *in;
	FILE *out;
	in = fopen(fileToRead, "r");
	out = fopen("img/histograms", "w");
	if(out == NULL)
	{
		printf("\nImpossible de créer le fichier histogramme\n");
		exit(0);
	}
	
	if(in == NULL)
	{
		printf("\nImpossible de lire le fichier %s\n",fileToRead);
		exit(0);
	}
	
	while (fgets(stringIn, 100, in) != NULL)  // On lit toute la liste des fichiers
	{
		if(stringIn[strlen(stringIn) - 1] == '\n')
			{stringIn[strlen(stringIn) - 1] = '\0';}
  		printf("\nLecture en cours : %s",stringIn); // On récupère le nom du fichier à lire
		processOneFile(stringIn,out); // Lit le fichier StringIn et écrit son histogramme dans out
	}
	fclose(in);
	fclose(out);
}

//Prend un fichier, le lit et écrit son histogramme dans un fichier de sortie
void processOneFile(char * stringIn, FILE*outFile)
{
	CIMAGE cim;
	char toRead[100]="img/images/";
	float cube[WIDTH];
	init(cube,WIDTH);

	strcat(toRead,stringIn);
	read_cimage(toRead,&cim);
	
	colorSection(cube,cim); // Construit le cube
	normalise(cube,cim.nx*cim.ny); // Normalise le cube 
	printCube(cube,outFile); // Ecrit le contenu du cube dans le fichier
}



//Initialise un histogramme en le remplissant de 0
void init(float cube[],int length)
{
   for (int j = 0; j < length; j++)
   {
   		cube[j]=0;
   }
}

//Rempli le contenu d'un histogramme à partir d'une CIMAGE
void colorSection(float cube[], CIMAGE cim)
{
	int k=0;
	for (int i = 0; i < cim.nx; i++) 
	{      
		for (int j = 0; j < cim.ny; j++) 
		{ 
			int R = cim.r[i][j]/64;
			int G = cim.g[i][j]/64;
			int B = cim.b[i][j]/64;

			k = R + G *4 + B*16;
			cube[k]++;
		}
	}
}

    
 
