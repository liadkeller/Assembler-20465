#include "utils.h"
#include "table.h"

extern int isError;

extern struct cmd commandsArray[ARRAY_SIZE];
extern struct data dataArray[ARRAY_SIZE];
extern char *entArray[LABELS_ARRAY_SIZE];
extern char *extArray[LABELS_ARRAY_SIZE];

extern int cmdSize;
extern int dataSize;
extern int extSize;
extern int entSize;


void addCommand(char **line)
{
	int i = cmdSize;
	int hasSymbol = hasLabel(line);
	int lineWords = getArraySize(line) - hasSymbol;
	int wordsNum; /* Real words; only extra words (not including this one) */


	commandsArray[i].encode = MAIN_COMMAND;
	commandsArray[i].encodeType = A;
	commandsArray[i].address = i;

	commandsArray[i].hasSymbol = hasSymbol;
	if (commandsArray[i].hasSymbol)
		commandsArray[i].symbol = getCleanLabel(line[LABEL_INDEX]);

	commandsArray[i].opcode = getOpcode(line[OPERTATION_NAME_INDEX]);
	commandsArray[i].group = getGroup(line[OPERTATION_NAME_INDEX]);

	if (lineWords == ZERO_OP_WORDS_NUM)
	{
		wordsNum = 0;
		commandsArray[i].operandsNumber = 0;
	}

	else if (lineWords == ONE_OP_WORDS_NUM)
	{
		if (!isOperand(line[FIRST_OPERAND_INDEX]))
		{
			fprintf(stderr, "Error - %s is not an operand. \n", line[FIRST_OPERAND_INDEX]);
			isError = TRUE;
			return;
		}

		wordsNum = 1;
		commandsArray[i].operandsNumber = 1;
		commandsArray[i].destOperand = line[FIRST_OPERAND_INDEX];
		commandsArray[i].destAddressingType = getAddressingType(commandsArray[i].destOperand);
		commandsArray[i + 1].encode = commandsArray[i].destAddressingType;
		commandsArray[i + 1].operandRole = DEST;
		commandsArray[i + 1].address = i + 1;
		commandsArray[i + 1].hasSymbol = FALSE;
	}

	else if (lineWords == TWO_OP_WORDS_NUM)
	{
		if (!isOperand(line[FIRST_OPERAND_INDEX]))
		{
			fprintf(stderr, "Error - %s is not an operand. \n", line[FIRST_OPERAND_INDEX]);
			isError = TRUE;
			return;
		}

		if (strcmp(line[COMMA_INDEX], ",") != 0)
		{
			fprintf(stderr, "Error - A comma is missing. \n");
			isError = TRUE;
			return;
		}

		if (!isOperand(line[SECOND_OPERAND_INDEX]))
		{
			fprintf(stderr, "Error - %s is not an operand. \n", line[SECOND_OPERAND_INDEX]);
			isError = TRUE;
			return;
		}

		commandsArray[i].operandsNumber = 2;
		commandsArray[i].srcOperand = line[FIRST_OPERAND_INDEX];
		commandsArray[i].srcAddressingType = getAddressingType(commandsArray[i].srcOperand);

		commandsArray[i].destOperand = line[SECOND_OPERAND_INDEX];
		commandsArray[i].destAddressingType = getAddressingType(commandsArray[i].destOperand);

		if (commandsArray[i].srcAddressingType == REGISTER && commandsArray[i].destAddressingType == REGISTER) /* In that case we need only one word */
		{
			wordsNum = 1;
			commandsArray[i + 1].encode = TWO_REGISTERS;
			commandsArray[i + 1].srcReg = commandsArray[i].srcOperand[REG_VALUE_INDEX] - '0';
			commandsArray[i + 1].destReg = commandsArray[i].destOperand[REG_VALUE_INDEX] - '0';
			commandsArray[i + 1].address = i + 1;
			commandsArray[i + 1].hasSymbol = FALSE;
		}

		else
		{
			wordsNum = 2;
			commandsArray[i + 1].encode = commandsArray[i].srcAddressingType;
			commandsArray[i + 1].operandRole = SRC;
			commandsArray[i + 1].address = i + 1;
			commandsArray[i + 1].hasSymbol = FALSE;

			commandsArray[i + 2].encode = commandsArray[i].destAddressingType;
			commandsArray[i + 2].operandRole = DEST;
			commandsArray[i + 2].address = i + 2;
			commandsArray[i + 2].hasSymbol = FALSE;
		}
	}
	cmdSize = i + wordsNum + 1; /* including the main command word */
}


void addData(char **line)
{
	int i = dataSize;
	int j = FIRST_NUM_INDEX;
	int loopFlag = TRUE;
	int hasSymbol = hasLabel(line);

	dataArray[i].address = i;
	dataArray[i].hasSymbol = hasSymbol;
	if (!hasSymbol)
		fprintf(stderr, "Warning - No label was given to the data. It won't be accessible. \n");
	if (dataArray[i].hasSymbol)
		dataArray[i].symbol = getCleanLabel(line[LABEL_INDEX]);

	while (loopFlag)
	{
		if (strcmp(line[hasSymbol + j], "") == 0 || strcmp(line[hasSymbol + j], "\n") == 0)
		{
			if (j == 1)
				fprintf(stderr, "Error - No number was entered. \n"); /* Not a global error */
			else
				fprintf(stderr, "Error - Extra Comma. \n"); /* Not a global error */
			loopFlag = FALSE;
			break;
		}

		else if (strcmp(line[hasSymbol + j], ",") == 0)
		{
			fprintf(stderr, "Error - A number was expected. \n");
			isError = TRUE;
			loopFlag = FALSE;
			break;
		}

		if (isNumber(line[hasSymbol + j]))
		{
			dataArray[i].address = i;
			dataArray[i].value = atoi(line[hasSymbol + j]);
			if (j != FIRST_NUM_INDEX)
				dataArray[i].hasSymbol = FALSE;
		}

		else
		{
			fprintf(stderr, "Error - %s is not a number. \n", line[hasSymbol + j]);
			i--; /* line[hasSymbol + j] is not a number but it's not a global error, so we'll ignore it and go backwards so we'll be promoted to dataArray[i] agaom in the end of this iteration */
		}

		if (strcmp(line[hasSymbol + j + 1], "\n") == 0 || line[hasSymbol + j + 1] == '\0')
		{
			loopFlag = FALSE;
			break;
		}

		else if (strcmp(line[hasSymbol + j + 1], ",") != 0)
		{ /* Notice that not only a comma is missing but it is also surely not the end of the loop */
			fprintf(stderr, "Error - A comma is missing. \n");
			isError = TRUE;
			loopFlag = FALSE;
			break;
		}

		j += 2;
		i++;
	}

	dataSize = ++i;
}


void addStr(char **line)
{
	int i = dataSize, j;
	int hasSymbol = hasLabel(line);
	int lineWords = getArraySize(line) - hasSymbol;
	int stringLen;

	if (lineWords != STRING_CMD_WORDS_NUM)
	{
		fprintf(stderr, "Error - Illegal .string command. \n");
		isError = TRUE;
		return;
	}

	stringLen = strlen(line[CONTENT_INDEX]);

	if (line[CONTENT_INDEX][FIRST_QUOTE_INDEX] != '"' || line[CONTENT_INDEX][LAST_QUOTE_INDEX] != '"')
	{
		fprintf(stderr, "Error - Illegal quotes. \n");
		isError = TRUE;
		return;
	}

	dataArray[i].address = i;
	dataArray[i].hasSymbol = hasSymbol;
	if (!hasSymbol)
		fprintf(stderr, "Warning - No label was given to the data. It won't be accessible. \n");
	if (dataArray[i].hasSymbol)
		dataArray[i].symbol = getCleanLabel(line[LABEL_INDEX]);

	for (j = FIRST_CHAR_INDEX; j <= LAST_CHAR_INDEX; i++, j++)
	{
		dataArray[i].address = i;
		dataArray[i].value = line[CONTENT_INDEX][j];
		if (j != FIRST_CHAR_INDEX)
			dataArray[i].hasSymbol = FALSE;
	}

	/* adds zero to the end of the string */
	dataArray[i].address = i;
	dataArray[i].value = 0;
	dataArray[i].hasSymbol = FALSE;


	dataSize = ++i;
}


void addExt(char **line)
{
	int hasSymbol = hasLabel(line);
	if (extSize < LABELS_ARRAY_SIZE) /* LABELS_ARRAY_SIZE is the size of the externs array */
		extArray[extSize] = getCleanLabel(line[CONTENT_INDEX]);
	extSize++;
}

void addEnt(char **line)
{
	int hasSymbol = hasLabel(line);
	if (entSize < LABELS_ARRAY_SIZE) /* LABELS_ARRAY_SIZE is the size of the entries array */
		entArray[entSize] = getCleanLabel(line[CONTENT_INDEX]);
	entSize++;
}


void incAddresses(int add) /* fix the data addresses so it will come right after the cmd addresses */
{
	int i;
	for (i = 0; i < cmdSize; i++)
		commandsArray[i].address += IC_START;

	for (i = 0; i < dataSize; i++)
		dataArray[i].address += IC_START + cmdSize;
}

int getSymbolAddress(char *symbol)
{
	int i;
	for (i = 0; i < cmdSize; i++)
		if (commandsArray[i].hasSymbol)
			if (strcmp(commandsArray[i].symbol, symbol) == 0)
				return commandsArray[i].address;

	for (i = 0; i < dataSize; i++)
		if (dataArray[i].hasSymbol)
			if (strcmp(dataArray[i].symbol, symbol) == 0)
				return dataArray[i].address;

	for (i = 0; i < extSize; i++)
		if (strcmp(extArray[i], symbol) == 0)
			return EXTERN_SYMBOL_ADDRESS;

	fprintf(stderr, "Error - Symbol %s does not exist \n", symbol);
	isError = TRUE;
	return ERROR;
}
