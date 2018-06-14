#include "utils.h"
#include "table.h"

extern int isError;

struct opcode {
	char *name;
	int group;
} opr[] = { { "mov", TWO_OPERANDS },
{ "cmp", TWO_OPERANDS },
{ "add", TWO_OPERANDS },
{ "sub", TWO_OPERANDS },
{ "not", ONE_OPERAND },
{ "clr", ONE_OPERAND },
{ "lea", TWO_OPERANDS },
{ "inc", ONE_OPERAND },
{ "dec", ONE_OPERAND },
{ "jmp", ONE_OPERAND },
{ "bne", ONE_OPERAND },
{ "red", ONE_OPERAND },
{ "prn", ONE_OPERAND },
{ "jsr", ONE_OPERAND },
{ "rts", ZERO_OPERANDS },
{ "stop", ZERO_OPERANDS } };


/* returns TRUE iff the first word ends with ':' */
int hasLabel(char **line)
{
	int i;
	for (i = 0; line[0][i] != '\0'; i++);
	return line[0][--i] == ':';
}

/* deletes the ':' in the end of the label */
char *getCleanLabel(char *label)
{
	int i;
	char *new = (char *)malloc(LINE_WORD * sizeof(char));

	for (i = 0; label[i] != '\0'; i++)
		new[i] = label[i];

	if (new[--i] == ':')
		new[i] = 0;

	if (!checkSymbol(new))
	{
		fprintf(stderr, "Error - %s is not a legal label. \n", new);
		isError = TRUE;
	}

	return new;
}

char **getCleanArray(char *cmd)
{
	int i, j, k;
	int inWord = TRUE;

	char **arr = malloc(MAX_WORDS * sizeof(char *));
	for (j = 0; j < MAX_WORDS; j++)
		arr[j] = malloc(LINE_WORD * sizeof(char));

	i = j = k = 0;

	while (cmd[i] != '\n') /* copies words and spearate each word into diffrent cells in the array */
	{
		if (cmd[i] != ' ' && cmd[i] != '\t')
		{
			if (k >= LINE_WORD)
				arr[j][--k] = '\0';

			if (j >= MAX_WORDS)
			{
				arr[--j][0] = '\n';
				arr[j][1] = '\0';
			}

			if (inWord && cmd[i] == ',') /* We did not have any white chars before the comma  */
			{
				arr[j][k] = '\0';
				j++;

				k = 0;
				arr[j][k] = cmd[i];
				/* We do not set  arr[j][1] = '\0'  since it will do so next iteration */

				inWord = FALSE;
			}

			else if (!inWord) /* means we were not int a word and this is the first letter of a words */
			{
				if (k != 0) /* why should we put the next word in the next cell if the current cell is empty? */
				{
					arr[j][k] = '\0';
					j++;
					k = 0;
				}
				inWord = TRUE;
			}

			arr[j][k] = cmd[i];
			k++;
		}

		else
			inWord = FALSE;

		i++;
	}

	/* Set the last word to be "\n" */
	j++;
	if (j >= 60)
		j--;
	arr[j][0] = '\n';
	arr[j][1] = '\0';
	return arr;
}

int getArraySize(char **array)
{
	int i;
	for (i = 0; strcmp(array[i], "\n") != 0; i++);
	return i;
}


int isOperation(char *op)
{
	int i;
	for (i = 0; i < OPERATIONS_NUMBER; i++)
		if (strcmp(op, opr[i].name) == 0)
			return TRUE;
	return FALSE;
}

int getOpcode(char *op)
{
	int i;
	for (i = 0; i < OPERATIONS_NUMBER; i++)
		if (strncmp(op, opr[i].name, strlen(opr[i].name)) == 0)
			return i;
	/* opcode wasnt found */
	fprintf(stderr, "Error - Illegal opcode. \n");
	isError = TRUE;
	return ERROR;
}

int getGroup(char *op)
{
	int i;
	for (i = 0; i < OPERATIONS_NUMBER; i++)
		if (strncmp(op, opr[i].name, strlen(opr[i].name)) == 0)
			return opr[i].group;
	/* opcode wasnt found, error already printed */
	isError = TRUE;
	return ERROR;
}

int isOperand(char *operand)
{
	int addressingType = getAddressingType(operand);
	if (addressingType != ADDRESS)
		return TRUE; /* getAddressingType() returns the addressing type if it identified it, returns ADDRESS otherwise */
	return checkSymbol(operand); /* It is legal operand iff it is a legal symbol/label. */
}

int isNumber(char *str)
{
	if (*str == '+' || *str == '-') /* skip a sign if such exist */
		str++;

	while (*str != '\0')
	{
		if (!(*str >= '0' && *str <= '9'))
			return FALSE;
		str++;
	}
	return TRUE;
}

int checkSymbol(char *cmd)
{
	int i = 0;
	int length = strlen(cmd);
	if (!length)
		return FALSE;

	if (cmd[i] < 'A' || cmd[i] > 'z' || (cmd[i] < 'a' && cmd[i] > 'Z')) /* first char isn't letter*/
		return FALSE;

	if (cmd[i] == 'r' && length == 2)/* the symbol have a name of register*/
		for (i = 0; i < 8; i++)
			if ((cmd[1] - '0') == i)
				return FALSE;

	for (i = 1; i < length; i++)
		if (cmd[i] < '0' || (cmd[i] > '9' && cmd[i] < 'A') || ((cmd[i] < 'a' && cmd[i] > 'Z')) || cmd[i] > 'z') /* cmd[i] isn't a char or a letter*/
			return FALSE;

	for (i = 0; i < OPERATIONS_NUMBER; i++)
		if (strcmp(opr[i].name, cmd) == 0)
			return FALSE;
	return TRUE;
}

int getAddressingType(char *operand)
{
	if (operand[0] == '#')
	{
		if (strlen(operand) == 2 && operand[1] == '0');

		else if (atoi(operand + 1) == 0)
		{
			fprintf(stderr, "Error - The operand is not a number. \n");
			/* operand isnt "#0" but atoi returns 0
			-> the operand isnt a number */
			isError = TRUE;
			return ADDRESS;
		}

		return NUMBER;
	}

	if (strlen(operand) == 2 && operand[0] == 'r')
	{
		if (operand[1] >= '0' && operand[1] <= '7')
			return REGISTER;
	}

	if (strlen(operand) == 6 && operand[0] == 'r' && operand[2] == '[' && operand[3] == 'r' && operand[5] == ']')
	{
		if (operand[1] >= '0' && operand[1] <= '7' && operand[4] >= '0' && operand[4] <= '7')
			return INDEX_REGISTER;
	}

	return ADDRESS;
	/* Address is the default addressing since it has the largest amount of legal operands */
}
