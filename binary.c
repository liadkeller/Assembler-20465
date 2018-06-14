#include "utils.h"
#include "table.h"
#include "binary.h"

extern int isError;

char *encode(struct cmd code)
{
	int i;
	char *bin;
	int encode = code.encode;
	bin = (char *)malloc((BINARY_WORD)*sizeof(char));

	for (i = 0; i < BINARY_WORD; i++)
		bin[i] = '0';
	bin[BINARY_WORD] = 0;

	switch (encode)
	{
	case NUMBER:
		addToBinStr(bin, intToBinaryStr(code.encodeType, 2), 0, 2);
		addToBinStr(bin, intToBinaryStr(code.numberVal, 13), 2, 13);
		break;

	case ADDRESS:
		addToBinStr(bin, intToBinaryStr(code.encodeType, 2), 0, 2);
		addToBinStr(bin, intToBinaryStr(code.addressVal, 13), 2, 13);
		break;

	case INDEX_REGISTER:
		addToBinStr(bin, intToBinaryStr(code.encodeType, 2), 0, 2);
		addToBinStr(bin, intToBinaryStr(code.reg1, 6), 2, 6);
		addToBinStr(bin, intToBinaryStr(code.reg2, 6), 8, 6);
		break;

	case REGISTER:
		addToBinStr(bin, intToBinaryStr(code.encodeType, 2), 0, 2);
		if (code.operandRole == DEST)
			addToBinStr(bin, intToBinaryStr(code.reg1, 6), 2, 6);
		if (code.operandRole == SRC)
			addToBinStr(bin, intToBinaryStr(code.reg1, 6), 8, 6);
		break;

	case TWO_REGISTERS:
		addToBinStr(bin, intToBinaryStr(code.encodeType, 2), 0, 2);
		addToBinStr(bin, intToBinaryStr(code.destReg, 6), 2, 6);
		addToBinStr(bin, intToBinaryStr(code.srcReg, 6), 8, 6);
		break;

	case MAIN_COMMAND:
		addToBinStr(bin, intToBinaryStr(code.encodeType, 2), 0, 2);
		if (code.operandsNumber == 1)
			addToBinStr(bin, intToBinaryStr(code.destAddressingType, 2), 2, 2);

		if (code.operandsNumber == 2)
		{
			addToBinStr(bin, intToBinaryStr(code.destAddressingType, 2), 2, 2);
			addToBinStr(bin, intToBinaryStr(code.srcAddressingType, 2), 4, 2);
		}

		addToBinStr(bin, intToBinaryStr(code.opcode, 4), 6, 4);
		addToBinStr(bin, intToBinaryStr(code.group, 2), 10, 2);
		addToBinStr(bin, "111", 12, 3);
		break;
	}
	reverse(bin);
	return bin;
}

char *intToBinaryStr(int num, int size) /* returns the binary presentation of the first "size" digits of num */
{
	int arr[BINARY_WORD];
	char *bin = (char *)malloc((BINARY_WORD)*sizeof(char));
	int i, j, sign;

	for (i = 0; i < size; i++)
		bin[i] = '0';

	if (num == 0)
		return bin;

	if (num > power(2, size) - 1 || num < -1 * power(2, size)) /* i is not in the range 2^(size-1) <= i <= 2^(size-1) - 1 */
	{
		fprintf(stderr, "Error - number %d is too large (%d) \n", num, size);
		isError = TRUE;
	}

	sign = (num >= 0) ? 1 : -1;
	num = sign * num; /* num = |num| */

	for (i = 0; num > 0 && i < size; i++)
	{
		arr[i] = num % 2;
		num = num / 2;
	}

	j = i - 1;

	for (i = 0; i < j + 1; i++)
	{
		if (arr[j - i])
			bin[i + size - 1 - j] = '1';
		else
			bin[i + size - 1 - j] = '0';
	}

	if (sign < 0)
	{
		i = size - 1;
		while (i >= 0 && bin[i] != '1')
			i--;
		i--;
		while (i >= 0)
		{
			if (bin[i] == '1')
				bin[i] = '0';

			else if (bin[i] == '0')
				bin[i] = '1';
			i--;
		}
	}

	return bin;
}

/* adds num to the bin (binary) string represents the code.
adds from the start bit e.g.
if bin = 011100000 and num = 11 and start = 2
then bin will be 011101100 */
void addToBinStr(char *bin, char *num, int start, int size)
{
	int i = start + size - 1;
	int j = 0;

	while (j < size)
	{
		if (num[j] == '1')
			bin[i] = '1';
		i--;
		j++;
	}
}

int power(int a, int b)
{
	int i, pow;
	for (i = 0, pow = 1; i < b; i++, pow *= a);
	return pow;
}

void reverse(char *s)
{
	int c, i, j;
	for (i = 0, j = BINARY_WORD - 1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

/* To be converted to hexadecimal */
int binaryStrToInt(char *bin)
{
	char *a = bin;
	int num = 0;
	do {
		int b = (*a == '1') ? 1 : 0;
		num = (num << 1) | b;
		a++;
	} while (*a);

	return num;
}
