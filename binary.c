#include "utils.h"
#include "binary.h"

extern int isError;

char *encode(struct cmd *code, int encode)
{
	int i;
	char *bin; 
	bin = (char *) malloc ((binary_word)*sizeof(char));
        
	for(i = 0; i < binary_word; i++)
		bin[i] = '0';
	bin[binary_word]=0;

        switch(encode)
        {        
                case NUMBER:
                        addBin(bin, intToBinary(code->encodeType, 2), 0, 2);
                        addBin(bin, intToBinary(code->number, 13), 2, 13);
                        break;
                        
                case ADDRESS:
                        addBin(bin, intToBinary(code->encodeType, 2), 0, 2);
                        addBin(bin, intToBinary(code->addressNumber, 13), 2, 13);
                        break;
                        
                case INDEX_REGISTER:
                        addBin(bin, intToBinary(code->encodeType, 2), 0, 2);
                        addBin(bin, intToBinary(code->reg1, 6), 2, 6);
                        addBin(bin, intToBinary(code->reg2, 6), 8, 6);
                        break;
                        
                case ONE_REGISTER:
                        addBin(bin, intToBinary(code->encodeType, 2), 0, 2);
                        if(code->whichReg == DEST)
                                addBin(bin, intToBinary(code->reg1, 6), 2, 6);
                        if(code->whichReg == SOURCE)
                                addBin(bin, intToBinary(code->reg1, 6), 8, 6);
                        break;
                        
                case TWO_REGISTER:
                        addBin(bin, intToBinary(code->encodeType, 2), 0, 2);
                        addBin(bin, intToBinary(code->reg2, 6), 2, 6);
                        addBin(bin, intToBinary(code->reg1, 6), 8, 6);
                        break;
                        
                case MAIN_COMMAND:
                        addBin(bin, intToBinary(code->encodeType, 2), 0, 2);
                        if(code->group > 0)
                                addBin(bin, intToBinary(code->firstAddressing, 2), 2, 2);
                        if(code->group > 1)
                                addBin(bin, intToBinary(code->secndAddressing, 2), 4, 2);
                        addBin(bin, intToBinary(code->opcode, 4), 6, 4);
                        addBin(bin, intToBinary(code->group, 2), 10, 2);
                        addBin(bin, "111", 12, 3);
                        break;
        }
	reverse(bin);
        return bin;
}

char *intToBinary(int num, int size) /* returns the binary presentation of the first "size" digits of num */
{
        int arr[binary_word];
        char *bin = (char *) malloc ((binary_word)*sizeof(char));
        int i, j, sign;

	for(i = 0; i < size; i++)
		bin[i] = '0';
	
	if(num == 0)
		return bin;

	if(num > power(2, size) - 1 || num < -1 * power(2, size)) /* i is not in the range 2^(size-1) <= i <= 2^(size-1) - 1 */
	{
		fprintf(stderr, "Error - number is too large \n");
		isError = TRUE;
	}

        sign = (num >= 0) ? 1 : -1;
        num = sign * num; /* num = |num| */
        
        for(i = 0; num > 0 && i < size; i++)
        {
                arr[i] = num % 2;
                num = num / 2;
        }
        
	j = i-1;

        for(i = 0; i < j+1; i++)
        {
                if(arr[j-i])
                        bin[i + size-1-j] = '1';
                else
                        bin[i + size-1-j] = '0';
        }
        
        if(sign < 0)
        {
                i = size-1;
                while(i >= 0 && bin[i] != '1')
                        i--;
                i--;
                while(i >= 0)
                {
                        if(bin[i] == '1')
                                bin[i] = '0';
                        
                        else if(bin[i] == '0')
                                bin[i] = '1';
			i--;
                } 
        }
        
        return bin;
}

/* adds num to bin from the start bit 
if bin = 011100000 and num = 11 and start = 2
then bin will be 011101100 */
void addBin(char *bin, char *num, int start, int size)
{
        int i = start + size - 1;
        int j = 0;
        
        while(j < size)
        {
                if(num[j] == '1')
                        bin[i] = '1';
                i--;
                j++;
        }
}

int power(int a, int b)
{
        int i, pow;
        for(i = 0, pow = 1; i < b; i++, pow *= a);
        return pow;
}

void reverse(char *s)
{
	int c, i, j;
	for (i = 0, j = binary_word-1; i < j; i++, j--)
	{
        	c = s[i];
        	s[i] = s[j];
        	s[j] = c;
	}
}

char *binaryToHexa(char *bin)
{
        int i, j,sum;
        char fixedBin[16]; /* bin has 15 bits, we will fix it to 16 bits */
        char *hexa = (char *) malloc (4*sizeof(char)); /* !!! to change*/
	
     
        fixedBin[0] = '0';
        for(i = 1; i < binary_word; i++)
                fixedBin[i+1] = bin[i];
        
        for(i = 0; i < 4; i++)
        {
                sum = 0;
                for(j = 0; j < 4; j++)
                {
                        if(fixedBin[4*i+j]=='1')
                                sum += power(2, 3-j);
                }
                
                hexa[i] = getHexa(sum);
        }
        return hexa;
}

int getHexa(int num)
{
        if (num >= 10 && num <= 15)
                return 'A' + num - 10;
        return '0' + num; /* if num >= 0 || num <= 9 */
}
