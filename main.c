#include "main.h"

int isError;

struct cmd commandsArray[ARRAY_SIZE];
struct data dataArray[ARRAY_SIZE];
struct binarycode binaryArray[ARRAY_SIZE];
char *entArray[LABELS_ARRAY_SIZE];
char *extArray[LABELS_ARRAY_SIZE];

int cmdSize;
int dataSize;
int binarySize;
int extSize;
int entSize;

int main(int argc, char *argv[])
{
	char *fileName;
	FILE *f;
	int i, len;

	cmdSize = dataSize = binarySize = extSize = entSize = 0;

	if (argc == 1)
	{
		fprintf(stderr, "Error - No file was entered. \n");
		return 0;
	}

	for (i = 1; i < argc; i++)
	{
		isError = FALSE;
		fileName = argv[i];
		len = strlen(fileName);

		f = fopen(strcat(fileName, ".as"), "r");
		fileName[len] = 0;

		if (!f)
		{
			fprintf(stderr, "Error - File %s doesn't exist. \n", fileName);
			continue;
		}

		firstPass(f); /* we send IC and DC to the first pass and the first pass will set it the right values */
		secondPass();

		if (!isError)
		{
			f = fopen(strcat(fileName, ".ob"), "w");
			fileName[len] = 0;
			FILE_ERROR(object)
				createObject(f);

			if (entSize > 0)
			{
				f = fopen(strcat(fileName, ".ent"), "w");
				fileName[len] = 0;
				FILE_ERROR(entry)
					createEntry(f);
			}

			if (extSize > 0)
			{
				f = fopen(strcat(fileName, ".ext"), "w");
				fileName[len] = 0;
				FILE_ERROR(extern)
					createExtern(f);
			}
		}

		fclose(f);
	}
	return 0;
}

void firstPass(FILE *f)
{
	char assLine[ASSEMBLY_LINE_MAX + 1];
	char **line;

	while (fgets(assLine, ASSEMBLY_LINE_MAX, f))
	{
		char *firstWord;
		int arrSize, arrWords, hasSymbol;

		line = getCleanArray(assLine);
		arrSize = getArraySize(line);

		hasSymbol = hasLabel(line);

		arrWords = arrSize - hasSymbol; /* will remove 1 word only if there's a label */

		if (arrWords <= 0)
		{
			fprintf(stderr, "Error - Empty command. \n");
			isError = TRUE;
		}

		firstWord = line[hasSymbol]; /* (0 + hasLabel) is the index of the first word after the label if such exist */

		if (line[0][0] == '\0' || line[0][0] == ';') /* Will continue since it's a blank sent. or a comment sent. */
			continue;

		else if (isOperation(firstWord))
			addCommand(line);

		else if (strcmp(firstWord, ".data") == 0)
			addData(line);

		else if (strcmp(firstWord, ".string") == 0)
			addStr(line);

		else if (strcmp(firstWord, ".extern") == 0)
			addExt(line);

		else if (strcmp(firstWord, ".entry") == 0)
			addEnt(line);

		else
		{
			fprintf(stderr, "Error: %s is an llegal command.\n", assLine);
			isError = TRUE;
		}
	}

	incAddresses();
	free(line);
}


void createObject(FILE *f)
{
	int i;
	fprintf(f, "%X   %X \n", cmdSize, dataSize);
	for (i = 0; i < binarySize; i++)
		fprintf(f, "%02X   %04X \n", binaryArray[i].address, binaryStrToInt(binaryArray[i].binary));
}

void createEntry(FILE *f)
{
	int i;
	for (i = 0; i < entSize; i++)
		fprintf(f, "%s   %X\n", entArray[i], getSymbolAddress(entArray[i]));
}

void createExtern(FILE *f)
{
	int i;
	for (i = 0; i < cmdSize; i++)
	{
		if (commandsArray[i].encode == MAIN_COMMAND)
		{
			if (commandsArray[i].operandsNumber == 1)
			{
				if (commandsArray[i + 1].encodeType == E)
					fprintf(f, "%s   %X\n", commandsArray[i].destOperand, commandsArray[i].address + 1);
			}

			if (commandsArray[i].operandsNumber == 2)
			{
				if (commandsArray[i + 1].encodeType == E)
					fprintf(f, "%s   %X\n", commandsArray[i].srcOperand, commandsArray[i].address + 1);

				if (commandsArray[i + 2].encodeType == E)
					fprintf(f, "%s   %X\n", commandsArray[i].destOperand, commandsArray[i].address + 2);
			}
		}
	}
}
