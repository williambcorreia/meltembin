#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stddef.h>

int checkMalloc(void *ptr);
int checkFile(FILE *file);
char *setOutputName(char *path, char *extension);
char *setFullPath(char *dir, char *outputName);
char **parseCue(FILE *cueFile, size_t *binCount);
void copyFile(FILE *source, FILE *destination);
void generateCue(FILE *srcCue, FILE *dstCue, size_t *binSizes);
void freeBinList(char **binList, size_t binCount);
size_t *getBinSizes(char *srcDir, char **binList, size_t binCount);

#endif
