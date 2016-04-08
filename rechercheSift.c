#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "proc.h"

#define WIDTH_SIFT 256
#define ALL -1
#define TOTAL_FICHIER 9637
	

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
void createHtml(char*requestFile,int nbAffichage);

KEY tableauTri_SIFT[TOTAL_FICHIER];	
char nomFichiers_SIFT[TOTAL_FICHIER][100];	

	
int main(int argc, char *argv[])
{
	if(argc >=2)
	{
		if (strcmp(argv[1], "-h") == 0) 
		{
			createHistograms_SIFT("img/list_sift.txt",atoi(argv[2])); //paramètre "ALL" pour tout lire
		}
		else if (strcmp(argv[1], "-e") == 0 && argc==4) 
		{
			setFileNames_SIFT();
			process_euclidean_distance_SIFT(argv[2],atoi(argv[3]));
			createHtml(argv[2],atoi(argv[3]));
		}
		else
		{
			printf("\n Histogrammes : ./rechercheSift -h [X premiers fichiers]");
			printf("\n EXEMPLE : ./rechercheSift -h -1");
			printf("\n Distance euclidiennes : ./rechercheSift -e [Fichier requête] [X premiers fichiers]");
			printf("\n EXEMPLE : ./rechercheSift -e img/1nn/2008_000020.sift 6");
		}
	}
	else
	{	
		printf("\n Histogrammes : ./rechercheSift -h [X premiers fichiers]");
		printf("\n EXEMPLE : ./rechercheSift -h -1");
		printf("\n Distance euclidiennes : ./rechercheSift -e [Fichier requête] [X premiers fichiers]");
		printf("\n EXEMPLE : ./rechercheSift -e img/1nn/2008_000020.sift 6");
	}

	printf("\n\n");
	exit(0);
}

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
	qsort(tableauTri_SIFT,TOTAL_FICHIER,sizeof(KEY),keyCompare);
	
	printf("\nRésultats de la comparaison avec %s",request_image);
	for(int i =0;i<nbAffichage;i++)
	  {
	    printf("\nrang : %i ---- fichier : %s , distance : %f",i+1,nomFichiers_SIFT[tableauTri_SIFT[i].k],tableauTri_SIFT[i].d);
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


void createHtml(char*requestFile,int nbAffichage)
{
 	FILE *htmlFile;
	htmlFile = fopen("resHistogrammesSift.html", "w"); 
	
	//On fait des opérations sur les strings pour récupérer les bons noms de fichier
	char cpy[100];

	char *token;
	token = strtok(requestFile, "/");
	while( token != NULL )
	{
	 	strcpy(cpy,token);
		token = strtok(NULL, "/");	
	}
	cpy[strlen(cpy)-5]='\0';
	strcat(cpy,".jpg");
	
	fprintf(htmlFile,"\n<html><head><meta charset=\"UTF-8\"></head>");
	fprintf(htmlFile,"\n\t<body>\n");
	fprintf(htmlFile,"<h1>\nRequête :</h1>\n");
	fprintf(htmlFile,"\t\t<img src=\"img/images/%s\"></br>\n", cpy);
	fprintf(htmlFile,"<h1>\n Résultats par formes : %i</h1>\n",nbAffichage);
	
	for(int i =0;i<nbAffichage;i++)
	{
		strcpy(cpy,nomFichiers_SIFT[tableauTri_SIFT[i].k]);
		cpy[strlen(nomFichiers_SIFT[tableauTri_SIFT[i].k])-5]='\0';
		strcat(cpy,".jpg");

		fprintf(htmlFile,"\t\t<img src=\"img/images/%s\">\n",cpy );
	}
	fprintf(htmlFile,"\t</body>\n</html>\n");
	fclose(htmlFile);
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

