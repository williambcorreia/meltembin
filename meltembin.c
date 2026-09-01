// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("usage: meltembin [.cue] [destDir]\n");
		return 1;
	}

	int status = 1;

	FILE *cueFile = NULL;
	FILE *outputBin = NULL;
	FILE *outputCue = NULL;
	FILE *onCopyBin = NULL;

	char *outputName = NULL;
	char *fullPath = NULL;
	char *srcDir = NULL;
	char *tempCueName = NULL;
	char *outputCuePath = NULL;

	size_t binCount = 0;
	char **binList = NULL;
	size_t *binSizes = NULL;

	char *inputPath = argv[1];
	char *destDir = (argv[2] == NULL) ? "." : argv[2];

	// open cue file
	cueFile = fopen(inputPath, "r");
	if (checkFile(cueFile)) goto cleanup;

	// parse cue
	binList = parseCue(cueFile, &binCount);
	if (!binList) goto cleanup;

	// set output name
	outputName = setOutputName(inputPath, ".bin");
	if (!outputName) goto cleanup;

	// define output bin path
	fullPath = setFullPath(destDir, outputName);
	if (!fullPath) goto cleanup;

	outputBin = fopen(fullPath, "wb");
	if (checkFile(outputBin)) goto cleanup;

	free(outputName);
	outputName = NULL;

	free(fullPath);
	fullPath = NULL;

	printf("\nMelting bins to single file...\n\n");

	// define source directory
	if (strrchr(inputPath, '/') == NULL)
	{
		srcDir = malloc(2);
		if (!srcDir) goto cleanup;

		strcpy(srcDir, ".");
	}
	else
	{
		char *slash = strrchr(inputPath, '/');
		size_t srcDirSize = slash - inputPath;

		srcDir = malloc(srcDirSize + 1);
		if (!srcDir) goto cleanup;

		memcpy(srcDir, inputPath, srcDirSize);
		srcDir[srcDirSize] = '\0';
	}

	// copy files
	for (size_t i = 0; i < binCount; i++)
	{
		fullPath = setFullPath(srcDir, binList[i]);
		if (!fullPath) goto cleanup;

		onCopyBin = fopen(fullPath, "rb");
		if (checkFile(onCopyBin)) goto cleanup;

		copyFile(onCopyBin, outputBin);

		fclose(onCopyBin);
		onCopyBin = NULL;

		free(fullPath);
		fullPath = NULL;
	}

	// get bin sizes
	binSizes = getBinSizes(srcDir, binList, binCount);
	if (!binSizes) goto cleanup;

	// generate output cue name
	tempCueName = setOutputName(inputPath, ".cue");
	if (!tempCueName) goto cleanup;

	// define output cue path
	outputCuePath = setFullPath(destDir, tempCueName);
	if (!outputCuePath) goto cleanup;

	free(tempCueName);
	tempCueName = NULL;

	// open output cue
	outputCue = fopen(outputCuePath, "w");
	if (checkFile(outputCue)) goto cleanup;

	free(outputCuePath);
	outputCuePath = NULL;

	// generate cue
	rewind(cueFile);
	generateCue(cueFile, outputCue, binSizes);

	status = 0;

cleanup:

	if (onCopyBin != NULL)fclose(onCopyBin);
	if (cueFile != NULL) fclose(cueFile);
	if (outputCue != NULL) fclose(outputCue);
	if (outputBin != NULL) fclose(outputBin);
	free(outputName);
	free(fullPath);
	free(srcDir);
	free(tempCueName);
	free(outputCuePath);
	free(binSizes);
	freeBinList(binList, binCount);

	return status;
}
