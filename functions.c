#include "functions.h"
#include <stdlib.h>
#include <string.h>

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
		perror("fopen");
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
			if (checkMalloc(binName)) return NULL;
			memcpy(binName, start + 1, length);
			binName[length] = '\0';

			(*binCount)++;

			char **temp = realloc(binList, (*binCount) * (sizeof(char *)));
			if (checkMalloc(temp))
			{
				free(binName);
				return NULL;
			}
			binList = temp;
			binList[(*binCount) - 1] = binName;
		}
	}
	return binList;
}

char *setOutputName(char *path)
{
	char *outputName;
	char *fileName = strrchr(path, '/');

	if (fileName != NULL) fileName += 1;
	else fileName = path;

	size_t outputNameSize = (strlen(fileName) - 4) + 4 + 1;
	outputName = malloc(outputNameSize);
	if (checkMalloc(outputName)) return NULL;
	snprintf(outputName, outputNameSize, "%.*s.bin", (int)(strlen(fileName) - 4),fileName);

	return outputName;
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
