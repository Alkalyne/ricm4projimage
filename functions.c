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
  float buffer[WIDTH];
  
  //fseek(f,position*WIDTH,SEEK_SET);
  fread(buffer,sizeof(buffer),1,f);
  float sum=0;
  for (int i=0;i<WIDTH;i++)
  {
  sum+=buffer[i];

  }
    printf("%f\n",sum);
}
