#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"
#include "constantes.h"


//Print the cube in binary form into f
void printCube(float cube[],FILE *f){
  fwrite(cube,sizeof(float),WIDTH,f);
}

void readCube(int position, FILE *f){
  fseek(f,position*WIDTH,SEEK_SET);
  //  fread(f
}
