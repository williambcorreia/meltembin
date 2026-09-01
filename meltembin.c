// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main(int argc, char **argv)
{
	// check if has enough args
	if (argc < 2)
	{
		printf("usage: meltembin [.cue] [destDir]\n");
		return 1;
	}

	// open file for copy
	FILE *cueFile;
	FILE *outputBin;

	cueFile = fopen(argv[1], "r");
	if (checkFile(cueFile)) return 1;

	size_t binCount = 0;
	char **binList = parseCue(cueFile, &binCount);
	if (!binList) return 1;
	char *inputPath = argv[1];
	char *destDir = (argv[2] == NULL) ? "." : argv[2];
	char *outputName;

	// set output name (dir/game.cue -> dir/game.bin)
	outputName = setOutputName(inputPath, ".bin");

	// define file output path
	char *fullPath;
	fullPath = setFullPath(destDir, outputName);
	outputBin = fopen(fullPath, "wb");
	if (checkFile(outputBin)) return 1;

	free(outputName);
	free(fullPath);

	printf("\nMelting bins to single file...\n\n");

	char *srcDir;
	if (strrchr(inputPath, '/') == NULL)
	{
		srcDir = ".";
	}
	else
	{
		char *slash = strrchr(inputPath, '/');
		size_t srcDirSize = slash - inputPath;
		srcDir = malloc(srcDirSize + 1);
		if (checkMalloc(srcDir)) return 1;
		srcDir[srcDirSize] = '\0';

		memcpy(srcDir, inputPath, srcDirSize);
	}

	// copy files
	FILE *onCopyBin;

	for (int i = 0; i < binCount; i++)
	{
		fullPath = setFullPath(srcDir, binList[i]);
		onCopyBin = fopen(fullPath, "rb");
		if (checkFile(onCopyBin)) return 1;
		copyFile(onCopyBin, outputBin);
		fclose(onCopyBin);
		free(fullPath);
	}

	// generate cue file
	size_t *binSizes = getBinSizes(srcDir, binList, binCount);
	char *tempCueName = setOutputName(inputPath, ".cue");

	char *outputCuePath = setFullPath(destDir, tempCueName);
	FILE *outputCue;
	outputCue = fopen(outputCuePath, "w");

	if (checkFile(outputCue)) return 1;
	rewind(cueFile);
	generateCue(cueFile, outputCue, binSizes);

	// cleanup
	for (int i = 0; i < binCount; i++)
	{
		free(binList[i]);
	}

	free(binList);
	free(binSizes);
	free(tempCueName);
	if (strcmp(srcDir, ".") != 0) free(srcDir);
	fclose(cueFile);
	fclose(outputBin);
	fclose(outputCue);

	return 0;
}
