all :
	gcc -c functions.c 
	gcc -D_GNU_SOURCE -std=c99 rdjpeg.c read_image.c functions.o -o read_image
