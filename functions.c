#include "functions.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int checkMalloc(void *ptr)
{
	if (ptr == NULL)
	{
		perror("malloc");
		return 1;
	}
	return 0;
}

int checkFile(FILE *file) 
{
	if (file == NULL)
	{
		perror("file");
		return 1;
	}
	return 0;
}

void freeBinList(char **binList, size_t binCount)
{
	if (binList == NULL) return;
	for (size_t i = 0; i < binCount; i++)
	{
		free(binList[i]);
	}

	free(binList);
}

char *setFullPath(char *dir, char *outputName)
{
	size_t fullPathSize = strlen(dir) + strlen(outputName) + 2;
	char *fullPath = malloc(fullPathSize);
	if (checkMalloc(fullPath)) return NULL;

	if (dir[strlen(dir) - 1] == '/') 
	{
		snprintf(fullPath, fullPathSize, "%s%s", dir, outputName);
	} 
	else 
	{
		snprintf(fullPath, fullPathSize, "%s/%s", dir, outputName);
	}

	return fullPath;
}

char **parseCue(FILE *cueFile, size_t *binCount)
{
	char line[1024];
	char **binList = NULL;

	while(fgets(line, sizeof(line), cueFile) != NULL)
	{
		if (strncmp(line, "FILE", 4) == 0)
		{
			char *start = strchr(line, '"');
			if (start == NULL) continue;

			char *end = strchr(start + 1, '"');
			if (end == NULL) continue;

			size_t length = end - start - 1;

			char *binName = malloc(length + 1);
			if (checkMalloc(binName)) { freeBinList(binList, *binCount); return NULL; }
			memcpy(binName, start + 1, length);
			binName[length] = '\0';

			char **temp = realloc(binList, (*binCount + 1) * (sizeof(char *)));
			if (checkMalloc(temp)) { free(binName); freeBinList(binList, *binCount); return NULL; }
			binList = temp;
			binList[*binCount] = binName;

			(*binCount)++;
		}
	}
	return binList;
}

size_t *getBinSizes(char *srcDir, char **binList, size_t binCount)
{
	size_t *binSizes = malloc(binCount * sizeof(size_t));
	if (checkMalloc(binSizes)) return NULL;
	struct stat st;

	for (size_t i = 0; i < binCount; i++)
	{
		char *fullPath = setFullPath(srcDir, binList[i]);
		if (checkMalloc(fullPath)) { free(binSizes); return NULL; }
		if (stat(fullPath, &st) != 0) { free(binSizes); free(fullPath); return NULL; }
		binSizes[i] = st.st_size;
		free(fullPath);
	}
	return binSizes;
}

char *setOutputName(char *path, char *extension)
{
	char *outputName;
	char *fileName = strrchr(path, '/');

	if (fileName != NULL) fileName += 1;
	else fileName = path;

	size_t outputNameSize = (strlen(fileName) - 4) + 4 + 1;
	outputName = malloc(outputNameSize);
	if (checkMalloc(outputName)) return NULL;
	snprintf(outputName, outputNameSize, "%.*s%s", (int)(strlen(fileName) - 4), fileName, extension);

	return outputName;
}

void copyFile(FILE *source, FILE *destination)
{
	size_t bytesRead;
	unsigned char fileBuffer[8192];

	while ((bytesRead = fread(fileBuffer, 1, sizeof(fileBuffer), source)) > 0)
	{
		size_t bytesWritten = 0;
		while(bytesWritten < bytesRead)
		{
			size_t written = fwrite(fileBuffer + bytesWritten, 1, bytesRead - bytesWritten, destination);

			if (written == 0)
			{
				perror("fwrite");
				return;
			}

			bytesWritten += written;
		}
	}
}

void generateCue(FILE *srcCue, FILE *dstCue, size_t *binSizes, char *outputBinName)
{
	char line[1024];
	size_t track = 0;
	size_t binStart = 0;
	size_t binEnd = 0;

	while(fgets(line, sizeof(line), srcCue) != NULL)
	{
		if (strncmp(line, "FILE", 4) == 0)
		{
			fprintf(dstCue, "FILE \"%s\" BINARY\n", outputBinName);
			
			binEnd += binSizes[track];
			track++;
		}
		else if(strncmp(line, "INDEX", 5) == 0)
		{
			int index, min, sec, frame;

			if (sscanf(line, "INDEX %d %d:%d:%d", &index, &min, &sec, &frame) == 4)
			{
				size_t totalFrames = binStart / 2352;
				totalFrames += (min * 60 + sec) * 75 + frame;

				min = totalFrames / (60 * 75);
				sec = (totalFrames / 75) % 60;
				frame = totalFrames % 75;

				fprintf(dstCue, "\t\tINDEX %02d %02d:%02d:%02d\n", index, min, sec, frame);
			}
		}
		else
		{
			fputs(line, dstCue);
		}
	}
}
