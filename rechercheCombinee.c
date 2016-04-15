#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "rdjpeg.h"
#include "functions.h"
#include "constantes.h"
#include "proc.h"

#define WIDTH_SIFT 256

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
void createHtml(char*requestFile,int coeffJPEG,int coeffSIFT,int nbAffichage);

char nomFichiers[TOTAL_FICHIER][100];	
	KEY tableauTri[TOTAL_FICHIER];
	
	
	
int printCube_SIFT(float cube[],FILE *f);
int readNextCube_SIFT(FILE *f,float buffer[]);
float euclidean_distance_SIFT(float req[],float curr[]);
void normalise_SIFT(float cube[],float nbPixel);
void createHistograms_SIFT(char * fileToRead,int iterations);
void processOneFile_SIFT(char * stringIn, FILE*outFile);
void init_SIFT(float cube[],int length);
void verifySumCube_SIFT(float buffer[]);
void process_euclidean_distance_SIFT(char *request_image,int nbAffichage);
void setFileNames_SIFT();
	
KEY tableauTri_SIFT[TOTAL_FICHIER];	
char nomFichiers_SIFT[TOTAL_FICHIER][100];	
	
	
int main(int argc, char *argv[])
{
	if(argc >=2)
	{
		if (strcmp(argv[1], "-h") == 0) 
		{
			createHistograms("img/list.txt",atoi(argv[2])); //paramètre "ALL" pour tout lire
			createHistograms_SIFT("img/list_sift.txt",atoi(argv[2]));
		}
		else if (strcmp(argv[1], "-e") == 0 && argc==7) 
		{
			setFileNames();
			process_euclidean_distance(argv[2],atoi(argv[4]));
			setFileNames_SIFT();
			process_euclidean_distance_SIFT(argv[3],atoi(argv[4]));
			createHtml(argv[2],atoi(argv[5]),atoi(argv[6]),atoi(argv[4]));
		}
		else
		{
			printf("\n Histogrammes : ./rechercheCombinee -h [X premiers fichiers]");
			printf("\n EXEMPLE : ./rechercheCombinee -h -1");
			printf("\n Distance euclidiennes : ./rechercheCombinee -e [Fichier requête JPEG] [Fichier requête SIFT] [X premiers fichiers] [Coeff JPG] [coeff SIFT]");
			printf("\n EXEMPLE : ./rechercheCombinee -e img/images/2008_000020.jpg img/1nn/2008_000020.sift 6 2 5");
		}
		//readAllCubes();
		//readCube_i(0);
	}
	else
	{
		printf("\n Histogrammes : ./rechercheCombinee -h [X premiers fichiers]");
		printf("\n EXEMPLE : ./rechercheCombinee -h -1");
		printf("\n Distance euclidiennes : ./rechercheCombinee -e [Fichier requête JPEG] [Fichier requête SIFT] [X premiers fichiers] [Coeff JPG] [coeff SIFT]");
		printf("\n EXEMPLE : ./rechercheCombinee -e img/images/2008_000020.jpg img/1nn/2008_000020.sift 6 2 5");
	}
	printf("\n\n");
	exit(0);
}

void createHtml(char*requestFile,int coeffJPEG,int coeffSIFT,int nbAffichage)
{
	for(int i =0;i<TOTAL_FICHIER;i++)
	{
		tableauTri[i].d=(coeffJPEG*tableauTri[i].d+coeffSIFT*tableauTri_SIFT[i].d)/(coeffJPEG+coeffSIFT);
	}
	qsort(tableauTri,TOTAL_FICHIER,sizeof(KEY),keyCompare); 
	

 	FILE *htmlFile;
	htmlFile = fopen("resHistogrammesCombinee.html", "w"); 
	fprintf(htmlFile,"\n<html><head><meta charset=\"UTF-8\"><title>Recherche Combinée</title></head>");
	fprintf(htmlFile,"\n\t<body>\n");
	fprintf(htmlFile,"\n<a href='resHistogrammesCombinee.html'>Recherche combinée</a><br>");
	fprintf(htmlFile,"\n<a href='resHistogrammesJPEG.html'>Recherche par couleur</a><br>");
	fprintf(htmlFile,"\n<a href='resHistogrammesSift.html'>Recherche par formes</a>");
	fprintf(htmlFile,"<h1>\nRequête image n° \"%s\"</h1>\n", requestFile);
	fprintf(htmlFile,"\t\t<img src=\"%s\" height='300px' width='400px'></br>\n", requestFile);
	fprintf(htmlFile,"<h1>\n Résultats combinés : nombre %i</h1>\n",nbAffichage);
	for(int i =0;i<nbAffichage;i++)
	{
		fprintf(htmlFile,"\t\t<img src=\"img/images/%s\"height='300px' width='400px'>\n", nomFichiers[tableauTri[i].k]);
	}
	fprintf(htmlFile,"\t</body>\n</html>\n");
	fclose(htmlFile);
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



/********************************************************/

void setFileNames_SIFT()
{
	char stringIn[100];	
	FILE *in = fopen("img/list_sift.txt", "r");
	int i=0;
	while ( fgets(stringIn, 100, in) != NULL)  
	{
		if(stringIn[strlen(stringIn) - 1] == '\n')
			{stringIn[strlen(stringIn) - 1] = '\0';}
		strcpy(nomFichiers_SIFT[i],stringIn);
		i++;
	}
	fclose(in);
}

void verifySumCube_SIFT(float buffer[])
{
	float sum=0;
	for (int i=0;i<WIDTH_SIFT;i++)
	{
		sum+=buffer[i];
	}
	printf("%.20f\n\n",sum);
}

int printCube_SIFT(float cube[],FILE *f)
{
	int res = fwrite(cube,sizeof(float),WIDTH_SIFT,f);
	return res;
}

int readNextCube_SIFT(FILE *f,float buffer[])
{
	int res =fread(buffer,sizeof(float),WIDTH_SIFT,f);
	//verifySumCube_SIFT(buffer);
	if(res!=0) // on n'affiche pas s'il n'y avait rien à lire
	{
		//verifySumCube_SIFT(buffer);
	}
	return res;
}


//Calcul la distance euclidienne entre 2 histogrammes
float euclidean_distance_SIFT(float req[],float curr[])
{
	float res=0;
	for (int i = 0; i < WIDTH_SIFT; i++) 
	{      
		 res +=(req[i]-curr[i])*(req[i]-curr[i]);
	}
	return res;
}



	
//Calcul la distance euclidienne entre un fichier requête et tous les histogrammes de "img/histograms"
void process_euclidean_distance_SIFT(char *request_image,int nbAffichage)
{
	float request_cube[WIDTH_SIFT],current_cube[WIDTH_SIFT];
	init_SIFT(request_cube,WIDTH_SIFT);
	init_SIFT(current_cube,WIDTH_SIFT);
	// On récupère d'abord l'histogramme requête
	int totalValues=0,temp;

	FILE*siftFile = fopen(request_image, "r");
	while ( fscanf (siftFile, "%i", &temp)==1)
    {  
      request_cube[temp-1]++;
      totalValues++;     
    }
  	fclose (siftFile);
	normalise_SIFT(request_cube,totalValues); // normalise_SIFT le cube 

	
	int i=0;
	float res;
 	FILE *in;
	in = fopen("img/histogramsSift", "rb"); 
	while (!feof(in)) // On parcours les histogrammes
	{
		printf("\nCalculating euclidean distance with line : %i",i);
		readNextCube_SIFT(in,current_cube); // On récupère l'histogramme de la ligne i du fichier
		res=euclidean_distance_SIFT(request_cube,current_cube); //On calcule la distance euclidienne
		tableauTri_SIFT[i].d=res;
		printf(" --- %f",res);
		tableauTri_SIFT[i].k=i;
		i++;	
	}
	fclose(in);
}



//normalise_SIFT le cube pour transformer les valeurs entre 0 et 1
void normalise_SIFT(float cube[],float nbPixel)
{ 
	for (int i = 0; i < WIDTH_SIFT; i++) 
	{      
		cube[i]/=nbPixel;
	}
}


//construit la totalité des histogrammes à partir d'un fichier contenant une liste de noms de fichiers
void createHistograms_SIFT(char * fileToRead,int iterations)
{
	char stringIn[100];	
	int i=0;
	FILE *in;
	FILE *out;
	in = fopen(fileToRead, "r");
	out = fopen("img/histogramsSift", "wb");
	if(out == NULL)
	{
		printf("\nImpossible de créer le fichier \"histogramsSift\"\n");
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
		processOneFile_SIFT(stringIn,out); // Lit le fichier StringIn et écrit son histogramme dans out
		i++;
	}
	fclose(in);
	fclose(out);
}

//Prend un fichier, le lit et écrit son histogramme dans un fichier de sortie
void processOneFile_SIFT(char * stringIn, FILE*outFile)
{
	char toRead[100]="img/1nn/";
	float cube[WIDTH_SIFT];
	int totalValues=0,temp;
	init_SIFT(cube,WIDTH_SIFT);

	strcat(toRead,stringIn);
	FILE*siftFile = fopen(toRead, "r");
	
	while ( fscanf (siftFile, "%i", &temp)==1)
    {   
      cube[temp-1]++;
      totalValues++;     
    }
  	fclose (siftFile);
	normalise_SIFT(cube,totalValues); // normalise_SIFT le cube 
	printCube_SIFT(cube,outFile); // Ecrit le contenu du cube dans le fichier
}


//init_SIFTialise un histogramme en le remplissant de 0
void init_SIFT(float cube[],int length)
{
   for (int j = 0; j < length; j++)
   {
   		cube[j]=0;
   }
}




