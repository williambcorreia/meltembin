#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>

int checkMalloc(void *ptr);
int checkFile(FILE *file);
char *setOutputName(char *path);
char *setFullPath(char *dir, char *outputName);
char **parseCue(FILE *cueFile, size_t *binCount);
void copyFile(FILE *source, FILE *destination);

#endif
