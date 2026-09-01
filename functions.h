#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stddef.h>

int checkMalloc(void *ptr);
int checkFile(FILE *file);
char *setOutputName(char *path, char *extension);
char *setFullPath(char *dir, char *outputName);
char **parseCue(FILE *cueFile, size_t *binCount);
void generateCue(FILE *srcCue, FILE *dstCue, size_t *binSizes, char *outputBinName);
void freeBinList(char **binList, size_t binCount);
void copyFile(FILE *source, FILE *destination, size_t totalSize, size_t *totalCopied);
size_t *getBinSizes(char *srcDir, char **binList, size_t binCount);

#endif
