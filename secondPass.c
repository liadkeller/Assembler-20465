#include "utils.h"
#include "table.h"
#include "binary.h"

extern int isError;

struct cmd commandsArray[ARRAY_SIZE];
struct data dataArray[ARRAY_SIZE];
struct binarycode binaryArray[ARRAY_SIZE];

extern int cmdSize;
extern int dataSize;
extern int binarySize;


void addBinary(int address, char *bin)
{
	binaryArray[binarySize].address = address;
	copyBinary(binaryArray[binarySize++].binary, bin);
}

void copyBinary(char *dest, char *src)
{
	int i;
	for (i = 0; i <= BINARY_WORD; i++)
		dest[i] = src[i];
}

void secondPass()
{
	int i;
	char *bin;

	for (i = 0; i < cmdSize; i++)
	{

		if (commandsArray[i].encode == MAIN_COMMAND)
		{
			bin = encode(commandsArray[i]);
			addBinary(commandsArray[i].address, bin);

			if (commandsArray[i].operandsNumber > 0 && commandsArray[i + 1].encode == TWO_REGISTERS)
			{
				bin = encode(commandsArray[i + 1]);
				addBinary(commandsArray[i].address + 1, bin);
			}

			else if (commandsArray[i].operandsNumber == 1)
			{
				commandsArray[i + 1] = buildOperand(commandsArray[i].destOperand, commandsArray[i + 1]);
				bin = encode(commandsArray[i + 1]);
				addBinary(commandsArray[i].address + 1, bin);
			}

			else if (commandsArray[i].operandsNumber == 2)
			{
				commandsArray[i + 1] = buildOperand(commandsArray[i].srcOperand, commandsArray[i + 1]);
				bin = encode(commandsArray[i + 1]);
				addBinary(commandsArray[i].address + 1, bin);

				commandsArray[i + 2] = buildOperand(commandsArray[i].destOperand, commandsArray[i + 2]);
				bin = encode(commandsArray[i + 2]);
				addBinary(commandsArray[i].address + 2, bin);
			}
		}
	}

	for (i = 0; i < dataSize; i++)
	{
		bin = intToBinaryStr(dataArray[i].value, BINARY_WORD);
		addBinary(dataArray[i].address, bin);
	}
}

/* returns a cmd structure built and suited to the given operand */
struct cmd buildOperand(char *operand, struct cmd command)
{
	struct cmd c;
	c.encode = command.encode;
	c.operandRole = command.operandRole;
	switch (c.encode)
	{
	case NUMBER: /* #a */
		c.encodeType = A;
		c.numberVal = atoi(++operand); /* copies the number without the # */
		break;

	case ADDRESS: /* LABEL */
		c.addressVal = getSymbolAddress(operand);
		c.encodeType = (c.addressVal > 0) ? R : E;
		break;

	case INDEX_REGISTER: /* ra[rb] */
		c.encodeType = A;
		c.reg1 = operand[1] - '0';
		c.reg2 = operand[4] - '0';
		if ((c.reg1) % 2 == 0 || (c.reg2) % 2 == 1)
			fprintf(stderr, "Error - First register should be odd and second register should be even");
		break;

	case REGISTER: /* ra */
		c.encodeType = A;
		c.reg1 = operand[1] - '0';
		break;

	case TWO_REGISTERS:
		/* In the special case of two register addressing we built the operand word in the first pass */
		break;
	}
	return c;
}
