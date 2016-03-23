#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

 
#define ALL -1
void processSifts(char * fileToRead,int iterations);
void processOneFile(char * stringIn,FILE *sample);

int main(int argc, char *argv[])
{
	processSifts("img/list_sift.txt",ALL); //paramètre "ALL" pour tout lire
	exit(0);
}

//construit la totalité des histogrammes à partir d'un fichier contenant une liste de noms de fichiers
void processSifts(char * fileToRead,int iterations)
{
	char stringIn[100];	
	int i=0;
	FILE *in;
	FILE *sample ;
	sample = fopen("img/samples.txt", "w");
	
	in = fopen(fileToRead, "r");
	
	if(in == NULL)
	{
		printf("\nImpossible de lire le fichier sift %s\n",fileToRead);
		exit(0);
	}
	
	//S'il n'y a pas un nombre d'itération en particulier, on lit tout
	while ((iterations==ALL && fgets(stringIn, 100, in) != NULL)
			|| (iterations!=ALL && fgets(stringIn, 100, in) != NULL && i<iterations))  
	{
		if(stringIn[strlen(stringIn) - 1] == '\n')
			{stringIn[strlen(stringIn) - 1] = '\0';}
  		printf("\nTraitement du fichier: %s",stringIn); // On récupère le nom du fichier à lire
		processOneFile(stringIn,sample); // Lit le fichier StringIn et écrit son résultat
		i++;
	}
	fclose(in);
	fclose(sample);
}


void processOneFile(char * stringIn,FILE *sample)
{
	FILE *out,*in;
	char toRead[100]="img/sift/";
	char toWrite[100]="img/simplified_sift/";
	char stringRead[1000];
	strcat(toRead,stringIn);
	strcat(toWrite,stringIn);
	
	in = fopen(toRead, "r");
	if(in == NULL)
	{
		printf("\nImpossible de lire le fichier %s\n",toRead);
		exit(0);
	}
	
	out = fopen(toWrite, "w");
	if(out == NULL)
	{
		printf("\nImpossible de créer le fichier %s",toWrite);
		exit(0);
	}
   
	//supprime les 3 premières lignes
	fgets(stringRead, 1000, in);
	fgets(stringRead, 1000, in);
	fgets(stringRead, 1000, in);
	
	char *ret;
	const char toFind = ';';
	int i=0;
	while (fgets(stringRead, 1000, in) != NULL) 
	{
		if(i%150==0) // une ligne toutes les 150
		{
		    ret = strchr(stringRead, toFind);
		    ret+=2*sizeof(char); // on se déplace de 2 caractères pour enlever l'espace et le ";"
		    strcpy(stringRead,ret); // On copie la chaine pour pouvoir modifier les derniers caractères
		    stringRead[strlen(stringRead) - 2] = '\n'; // on remplace le ";" par un saut de ligne
		    stringRead[strlen(stringRead) - 1] = '\0'; // on remplace le "\n" par une fin de chaîne
			fputs(stringRead, out);
			fputs(stringRead, sample);
		}
		
		i++;		
	}
	
	fclose(out);
	fclose(in);
}
