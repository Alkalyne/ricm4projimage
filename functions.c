#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

void printCube(float cube[], FILE *f);
void readCube(int position, FILE *f);

//Print the cube in binary form into f
printCube(float cube[],FILE *f){
  fwrite(cube,sizeof(float),WIDTH,f);
}

readCube(int position, FILE *f){
  fseek(f,position*WIDTH,SEEK_SET);
  //  fread(f
}
