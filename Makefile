all :
	gcc -c -std=c99 functions.c 
	gcc post4.c cgiu.c -o post4.cgi
	gcc -D_GNU_SOURCE -std=c99 rdjpeg.c proc.c read_image.c functions.o -o read_image
	gcc traitement_sift.c -o traitement_sift
