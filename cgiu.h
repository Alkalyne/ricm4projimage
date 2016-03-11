#ifndef _CGIU_H
#define _CGIU_H

#include <stdio.h>
#ifndef NO_STDLIB_H
#include <stdlib.h>
#else
char *getenv();
#endif
#include <string.h>

typedef struct {
    char *name;
    char *val;
} entry;

int countword(char *line, char stop);
char *getword(char *line, char stop);

void print_html_head(char *title);
void print_debug_query(entry *entries, int m, char *qs, int argc, char *argv[]);
void print_html_tail();

char *get_query_string(int argc, char *argv[]);
entry *get_entries(char *query_string, int *nb_entries);

char *getparam(entry *entries, int m, char *name);
char **getparams(entry *entries, int m, char *name, int *nb_params);
int testparamvalue(entry *entries, int m, char *name, char *val);

#endif
