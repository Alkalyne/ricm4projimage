/*-------------------------*/
/* Fichier : post4.c       */
/* Auteur : Georges Quénot */
/*-------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "cgiu.h"

#define  DEBUG 1

main(int argc, char *argv[])
{
    int x,m;
    char *qs,*url;
    entry *entries;

    /* récupération de la chaîne de paramètres */
    qs = get_query_string(argc,argv);

    /* récupération des couples (nom,valeur) */
    entries = get_entries(qs,&m);

    /* émission de l'entête */
    print_html_head("R&eacute;sultat");

    /* affichage éventuel des informations de debug */
    if (DEBUG) print_debug_query(entries,m,qs,argc,argv);

    /* récupération des paramètres */
    url  = getparam(entries,m,"URL");

    /* affichage des paramètres */
    printf("<H1>Param&egrave;tres re&ccedil;u(s) en entr&eacute;e</H1>\n");
    printf("<ul>\n");
    if (url != NULL) {
        printf("<li>URL de l'image requ&ecirc;te : <code>%s</code>\n",url);
        printf("<li>Image requ&ecirc;te : <img src=\"%s\" align=\"center\">\n",url);
    }
    printf("</ul>\n");

    /* émission de la fin de corps et de document */
    print_html_tail();
}
