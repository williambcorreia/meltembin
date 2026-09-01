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
			memcpy(binName, start + 1, length);
			binName[length] = '\0';

			(*binCount)++;

			char **temp = realloc(binList, (*binCount) * (sizeof(char *)));
			binList = temp;
			binList[(*binCount) - 1] = binName;
		}
	}
	return binList;
}

size_t *getBinSizes(char *srcDir, char **binList, size_t binCount)
{
	size_t *binSizes = malloc(binCount * sizeof(size_t));
	struct stat st;

	for (size_t i = 0; i < binCount; i++)
	{
		char *fullPath = setFullPath(srcDir, binList[i]);
		stat(fullPath, &st);
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
	snprintf(outputName, outputNameSize, "%.*s%s", (int)(strlen(fileName) - 4), fileName, extension);

	return outputName;
}

char *setFullPath(char *dir, char *outputName)
{
	size_t fullPathSize = strlen(dir) + strlen(outputName) + 2;
	char *fullPath = malloc(fullPathSize);

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

void copyFile(FILE *source, FILE *destination)
{
	size_t bytesRead;
	unsigned char fileBuffer[8192];

	while ((bytesRead = fread(fileBuffer, 1, sizeof(fileBuffer), source)) > 0)
	{
		size_t bytesWritten = 0;
		while(bytesWritten < bytesRead)
		{
			bytesWritten += fwrite(fileBuffer + bytesWritten, 1, bytesRead - bytesWritten, destination);
		}
	}
}

void generateCue(FILE *srcCue, FILE *dstCue, size_t *binSizes)
{
	char line[1024];
	size_t track = 0;
	size_t binStart = 0;
	size_t binEnd = 0;

	while(fgets(line, sizeof(line), srcCue) != NULL)
	{
		if (strncmp(line, "FILE", 4) == 0)
		{
			binStart = binEnd;
			char *start = strchr(line, '"');
			if (start == NULL) continue;
			char *trackStart = strchr(start, '(');
			if (trackStart == NULL) continue;

			char *end = strchr(start + 1, '"');
			if (end == NULL) continue;
			char *trackEnd = strchr(trackStart, ')');
			if (trackEnd == NULL) continue;

			fputs("FILE ", dstCue);
			fwrite(start, sizeof(char), trackStart - start - 1, dstCue);
			fwrite(trackEnd + 1, sizeof(char), end - trackEnd - 1, dstCue);
			fputs(end, dstCue);
			
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
