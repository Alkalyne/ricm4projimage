all :
	gcc -c functions.c 
	gcc -std=c99 rdjpeg.c read_image.c functions.o -o read_image
