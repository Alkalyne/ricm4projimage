#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "rdjpeg.h"

#define SIZE 4
#define WIDTH (SIZE*SIZE*SIZE)
#define COL 4 //le nombre de valeurs à afficher avant un retour à la ligne

void init( float cube[],int length);
void colorSection(float cube[], CIMAGE cim);
void printCube(float cube[],int nbCol,FILE*f);
void createHistograms(char * fileToRead);
void processOneFile(char * stringIn);
void normalise(float cube[],float nbPixel);

int main(int argc, char *argv[])
{
	createHistograms("img/list.txt");
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



//construit la totalité des histogrammes à partir d'un fichier contenant une liste de noms de fichiers
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
	float cube[WIDTH];
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
	
	colorSection(cube,cim); // Construit le cube
	normalise(cube,cim.nx*cim.ny); // Normalise le cube 
	printCube(cube,COL,out); // Ecrit le contenu du cube dans le fichier
	
	fclose(out);
}

//Affiche le contenu d'un histogramme avec un saut de ligne toutes les nbCol multiples
void printCube(float cube[],int nbCol,FILE*f)
{
   for (int j = 0; j < WIDTH; j++)
   {
   		fprintf(f,"%f ",cube[j]);
   		if(j!=0 && (j+1)%nbCol==0)
   		{
   			fprintf(f,"\n");
   		}
   }
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

    
 
