#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "rdjpeg.h"
#include "functions.h"
#include "constantes.h"
#include "proc.h"

//Calcul la distance euclidienne entre 2 histogrammes
float euclidean_distance(float req[],float curr[]);
//Calcul la distance euclidienne entre un fichier requête et tous les histogrammes de "img/histograms" pour les nb_iterations premiers histogrammes
void process_euclidean_distance(char *request_image,int nbAffichage);
//normalise le cube pour transformer les valeurs entre 0 et 1
void normalise(float cube[],float nbPixel);
//Lit tous les histogrammes
void readAllCubes();
//construit la totalité des histogrammes à partir d'un fichier contenant une liste de noms de fichiers
void createHistograms(char * fileToRead,int iterations);
//Prend un fichier, le lit et écrit son histogramme dans un fichier de sortie
void processOneFile(char * stringIn, FILE*outFile);
//Initialise un histogramme en le remplissant de 0
void init(float cube[],int length); 
//Rempli le contenu d'un histogramme à partir d'une CIMAGE
void colorSection(float cube[], CIMAGE cim);
void setFileNames();

char nomFichiers[TOTAL_FICHIER][100];	
	KEY tableauTri[TOTAL_FICHIER];
	
	
int main(int argc, char *argv[])
{
	if(argc >=2)
	{
		if (strcmp(argv[1], "-h") == 0) 
		{
			createHistograms("img/list.txt",atoi(argv[2])); //paramètre "ALL" pour tout lire
		}
		else if (strcmp(argv[1], "-e") == 0 && argc==4) 
		{
			setFileNames();
			process_euclidean_distance(argv[2],atoi(argv[3]));
		}
		else
		{
			printf("\n Histogrammes : ./read_image -h [X premiers fichiers]");
			printf("\n Distance euclidiennes : ./read_image -e [Fichier requête] [X premiers fichiers]");
		}
		//readAllCubes();
		//readCube_i(0);
	}
	else
	{
		printf("\n Histogrammes : ./read_image -h [X premiers fichiers]");
		printf("\n Distance euclidiennes : ./read_image -e [Fichier requête] [X premiers fichiers]");
	}
	exit(0);
}

void setFileNames()
{
	char stringIn[100];	
	FILE *in = fopen("img/list.txt", "r");
	int i=0;
	while ( fgets(stringIn, 100, in) != NULL)  
	{
		if(stringIn[strlen(stringIn) - 1] == '\n')
			{stringIn[strlen(stringIn) - 1] = '\0';}
		strcpy(nomFichiers[i],stringIn);
		i++;
	}
	fclose(in);
}


//Calcul la distance euclidienne entre 2 histogrammes
float euclidean_distance(float req[],float curr[])
{
	float res=0;
	for (int i = 0; i < WIDTH; i++) 
	{      
		 res +=(req[i]-curr[i])*(req[i]-curr[i]);
	}
	return res;
}



	
//Calcul la distance euclidienne entre un fichier requête et tous les histogrammes de "img/histograms"
void process_euclidean_distance(char *request_image,int nbAffichage)
{
	CIMAGE request_cim;
	float request_cube[WIDTH],current_cube[WIDTH];
	init(request_cube,WIDTH);
	init(current_cube,WIDTH);
	// On récupère d'abord l'histogramme requête
	read_cimage(request_image,&request_cim); // lit l'image requête
	colorSection(request_cube,request_cim); // Construit le cube
	normalise(request_cube,(float)request_cim.nx*request_cim.ny); // Normalise le cube
	
	int i=0;
	float res;
 	FILE *in;
	in = fopen("img/histograms", "rb"); 
	while (!feof(in)) // On parcours les histogrammes
	{
		printf("\nCalculating euclidean distance with line : %i",i);
		readNextCube(in,current_cube); // On récupère l'histogramme de la ligne i du fichier
		res=euclidean_distance(request_cube,current_cube); //On calcule la distance euclidienne
		tableauTri[i].d=res;
		printf(" --- %f",res);
		tableauTri[i].k=i;
		i++;	
	}
	qsort(tableauTri,TOTAL_FICHIER,sizeof(KEY),keyCompare);
	printf("\nRésultats de la comparaison avec %s",request_image);
	for(int i =0;i<nbAffichage;i++)
	  {
	    printf("\nrang : %i ---- fichier : %s , distance : %f",i+1,nomFichiers[tableauTri[i].k],tableauTri[i].d);
	  }
	fclose(in);
	free_cimage(request_image,&request_cim);
}



//normalise le cube pour transformer les valeurs entre 0 et 1
void normalise(float cube[],float nbPixel)
{ 
	for (int i = 0; i < WIDTH; i++) 
	{      
		cube[i]/=nbPixel;
	}
}

//Lit tous les histogrammes
void readAllCubes()
{
	int i=1;
	float current_cube[WIDTH];
	init(current_cube,WIDTH);
 	FILE *in;
	in = fopen("img/histograms", "rb");
	while (readNextCube(in,current_cube)!=0)
	{
		printf("\nEnd of reading histogram : %i",i);
		i++;
	}
	fclose(in);
}


//construit la totalité des histogrammes à partir d'un fichier contenant une liste de noms de fichiers
void createHistograms(char * fileToRead,int iterations)
{
	char stringIn[100];	
	int i=0;
	FILE *in;
	FILE *out;
	in = fopen(fileToRead, "r");
	out = fopen("img/histograms", "wb");
	if(out == NULL)
	{
		printf("\nImpossible de créer le fichier \"histograms\"\n");
		exit(0);
	}
	
	if(in == NULL)
	{
		printf("\nImpossible de lire le fichier %s\n",fileToRead);
		exit(0);
	}
	
	//S'il n'y a pas un nombre d'itération en particulier, on lit tout
	while ((iterations==ALL && fgets(stringIn, 100, in) != NULL)
			|| (iterations!=ALL && fgets(stringIn, 100, in) != NULL && i<iterations))  
	{
		if(stringIn[strlen(stringIn) - 1] == '\n')
			{stringIn[strlen(stringIn) - 1] = '\0';}
  		printf("\nCréation de l'histogramme: %s",stringIn); // On récupère le nom du fichier à lire
		processOneFile(stringIn,out); // Lit le fichier StringIn et écrit son histogramme dans out
		strcpy(nomFichiers[i],stringIn);
		i++;
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
	free_cimage(toRead,&cim);
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
