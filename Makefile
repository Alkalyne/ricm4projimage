all :
	gcc -c -std=c99 functions.c 
	gcc post4.c cgiu.c -o post4.cgi
	gcc -D_GNU_SOURCE -std=c99 rdjpeg.c proc.c rechercheCouleur.c functions.o -o rechercheCouleur
	gcc -D_GNU_SOURCE -std=c99 proc.c rechercheSift.c -o rechercheSift
	gcc -D_GNU_SOURCE -std=c99 rdjpeg.c proc.c rechercheCombinee.c functions.o -o rechercheCombinee
	gcc traitement_sift.c -o traitement_sift
	
