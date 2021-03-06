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
//Calcul la distance euclidienne entre un fichier requ�te et tous les histogrammes de "img/histograms" pour les nb_iterations premiers histogrammes
void process_euclidean_distance(char *request_image,int nbAffichage);
//normalise le cube pour transformer les valeurs entre 0 et 1
void normalise(float cube[],float nbPixel);
//Lit tous les histogrammes
void readAllCubes();
//construit la totalit� des histogrammes � partir d'un fichier contenant une liste de noms de fichiers
void createHistograms(char * fileToRead,int iterations);
//Prend un fichier, le lit et �crit son histogramme dans un fichier de sortie
void processOneFile(char * stringIn, FILE*outFile);
//Initialise un histogramme en le remplissant de 0
void init(float cube[],int length); 
//Rempli le contenu d'un histogramme � partir d'une CIMAGE
void colorSection(float cube[], CIMAGE cim);
void setFileNames();
void createHtml(char*requestFile,int nbAffichage);

char nomFichiers[TOTAL_FICHIER][100];	
	KEY tableauTri[TOTAL_FICHIER];
	
	
int main(int argc, char *argv[])
{
	if(argc >=2)
	{
		if (strcmp(argv[1], "-h") == 0) 
		{
			createHistograms("img/list.txt",atoi(argv[2])); //param�tre "ALL" pour tout lire
		}
		else if (strcmp(argv[1], "-e") == 0 && argc==4) 
		{
			setFileNames();
			process_euclidean_distance(argv[2],atoi(argv[3]));
			createHtml(argv[2],atoi(argv[3]));
		}
		else
		{
			printf("\n Histogrammes : ./rechercheCouleur -h [X premiers fichiers]");
			printf("\n EXEMPLE : ./rechercheCouleur -h -1");
			printf("\n Distance euclidiennes : ./rechercheCouleur -e [Fichier requ�te] [X premiers fichiers]");
			printf("\n EXEMPLE : ./rechercheCouleur -e img/images/2008_000020.jpg 6 ");
		}
		//readAllCubes();
		//readCube_i(0);
	}
	else
	{
		printf("\n Histogrammes : ./rechercheCouleur -h [X premiers fichiers]");
		printf("\n EXEMPLE : ./rechercheCouleur -h -1");
		printf("\n Distance euclidiennes : ./rechercheCouleur -e [Fichier requ�te] [X premiers fichiers]");
		printf("\n EXEMPLE : ./rechercheCouleur -e img/images/2008_000020.jpg 6 ");
	}
	printf("\n\n");
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



	
//Calcule la distance euclidienne entre un fichier requ�te et tous les histogrammes de "img/histograms"
void process_euclidean_distance(char *request_image,int nbAffichage)
{
	CIMAGE request_cim;
	float request_cube[WIDTH],current_cube[WIDTH];
	//On initialise nos histogrammes
	init(request_cube,WIDTH);
	init(current_cube,WIDTH);
	// On r�cup�re d'abord l'histogramme requ�te
	read_cimage(request_image,&request_cim); // lit l'image requ�te
	colorSection(request_cube,request_cim); // Construit le cube associ� � l'image requ�te
	normalise(request_cube,(float)request_cim.nx*request_cim.ny); // Normalise le cube requ�te
	
	int i=0;
	float res;
 	FILE *in;
	in = fopen("img/histograms", "rb"); 
	while (!feof(in)) // On parcours tous les histogrammes
	{
		printf("\nCalculating euclidean distance with line : %i",i);
		readNextCube(in,current_cube); // On r�cup�re l'histogramme suivant
		res=euclidean_distance(request_cube,current_cube); //On calcule la distance euclidienne entre la requ�tre et l'histogramme actuel
		tableauTri[i].d=res;
		printf(" --- %f",res);
		tableauTri[i].k=i;
		i++;	
	}
	//On tri les r�sultats obtenus selon leur distance euclidienne
	qsort(tableauTri,TOTAL_FICHIER,sizeof(KEY),keyCompare);
	printf("\nR�sultats de la comparaison avec %s",request_image);
	//On affiche les X premiers r�sultats dans la console
	for(int i =0;i<nbAffichage;i++) 
	{
		printf("\nrang : %i ---- fichier : %s , distance : %f",i+1,nomFichiers[tableauTri[i].k],tableauTri[i].d);
	}
	fclose(in); // on ferme le fichier
	free_cimage(request_image,&request_cim); // on lib�re l'image
}

void createHtml(char*requestFile,int nbAffichage)
{
 	FILE *htmlFile;
	htmlFile = fopen("resHistogrammesJPEG.html", "w"); 
	
	fprintf(htmlFile,"\n<html><head><meta charset=\"UTF-8\"><title>Recherche par Couleur</title></head>");
	fprintf(htmlFile,"\n\t<body>\n");
	fprintf(htmlFile,"\n<a href='resHistogrammesCombinee.html'>Recherche combinee</a><br>");
	fprintf(htmlFile,"\n<a href='resHistogrammesJPEG.html'>Recherche par couleur</a><br>");
	fprintf(htmlFile,"\n<a href='resHistogrammesSift.html'>Recherche par formes</a>");
	fprintf(htmlFile,"<h1>\nRequete image numero \"%s\"</h1>\n", requestFile);
	fprintf(htmlFile,"\t\t<img src=\"%s\" height='300px' width='400px'></br>\n", requestFile);
	fprintf(htmlFile,"<h1>\n Resultats par couleur : nombre %i</h1>\n",nbAffichage);
	
	for(int i =0;i<nbAffichage;i++)
	{
		fprintf(htmlFile,"\t\t<img src=\"img/images/%s\"height='300px' width='400px'>\n", nomFichiers[tableauTri[i].k]);
	}
	fprintf(htmlFile,"\t</body>\n</html>\n");
	fclose(htmlFile);
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


//construit la totalit� des histogrammes � partir d'un fichier contenant une liste de noms de fichiers
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
		printf("\nImpossible de cr�er le fichier \"histograms\"\n");
		exit(0);
	}
	
	if(in == NULL)
	{
		printf("\nImpossible de lire le fichier %s\n",fileToRead);
		exit(0);
	}
	
	//S'il n'y a pas un nombre d'it�ration en particulier, on lit tout
	while ((iterations==ALL && fgets(stringIn, 100, in) != NULL)
			|| (iterations!=ALL && fgets(stringIn, 100, in) != NULL && i<iterations))  
	{
		if(stringIn[strlen(stringIn) - 1] == '\n')
			{stringIn[strlen(stringIn) - 1] = '\0';}
  		printf("\nCr�ation de l'histogramme: %s",stringIn); // On r�cup�re le nom du fichier � lire
		processOneFile(stringIn,out); // Lit le fichier StringIn et �crit son histogramme dans out
		i++;
	}
	fclose(in);
	fclose(out);
}

//Prend un fichier, le lit et �crit son histogramme dans un fichier de sortie
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

//Rempli le contenu d'un histogramme � partir d'une CIMAGE
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
