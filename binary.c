char *encode(struct cmd *code, int encode)
{
        char bin[15]; /* malloc */
        
        switch(encode)
        {        
                case NUMBER:
                        addBin(bin, intToBinary(code->encodeType, 2), 0, 2);
                        addBin(bin, intToBinary(code->number, 13), 2, 13);
                        
                case ADDRESS:
                        addBin(bin, intToBinary(code->encodeType, 2), 0, 2);
                        addBin(bin, intToBinary(code->addressNumber, 13), 2, 13);
                        
                case INDEX_REGISTER:
                        addBin(bin, intToBinary(code->encodeType, 2), 0, 2);
                        addBin(bin, intToBinary(code->reg1, 6), 2, 6);
                        addBin(bin, intToBinary(code->reg2, 6), 8, 6);
                        
                case ONE_REGISTER:
                        addBin(bin, intToBinary(code->encodeType, 2), 0, 2);
                        if(code->whichReg == DEST)
                                addBin(bin, intToBinary(code->reg1, 6), 2, 6);
                        if(code->whichReg == SOURCE)
                                addBin(bin, intToBinary(code->reg1, 6), 8, 6);
                        
                case TWO_REGISTER:
                        addBin(bin, intToBinary(code->encodeType, 2), 0, 2);
                        addBin(bin, intToBinary(code->reg1, 6), 2, 6);
                        addBin(bin, intToBinary(code->reg2, 6), 8, 6);
        }
        
}

int intToBinary(int num, int size) /* returns the binary presentation of the first "size" digits of num */
{
        int arr[size];
        int i;
        char bin[size];
        
        for(i = 0; num > 0 && i < size; i++)
        {
                arr[i] = num % 2;
                num = num / 2;
        }
        
        if(i == size)
                i--;
        
        for(; i >= 0; i--)
        {
                if(arr[i])
                        bin[i] = '1';
                else
                        bin[i] = '0';
        }
        
        return bin;
}

/* adds num to bin from the start bit
if bin = 011100000 and num = 11 and start = 2
then bin will be 011101100 */
void *addBin(char *bin, char *num, int start, int size)
{
        int i = start;
        int j = 0;
        
        int size = getSize(num);
        while(j < size)
        {
                if(num[j] == '1')
                        bin[i] = '1';

                i++;
                j++;
        }
}

/*int getSize(char *num) // return the number of digits of num
{
        int i = 0;
        int x = num;
        
        while(x > 0)
        {
               x = x/10;
               i++;
        }

        return i;
}

int pow(int a, int b)
{
        int i, res;
        for(i = 0, res = 1; i < b; i++, res *= a);
        return res;
}*/

/* ==================== 
      HEXADECIMAL
===================== */

char *codeToHexa(int bin) /* gets 15-digits binary num and returns hexa string */
{
        int i;
        char hex[4];
        char ans[5]; /* !!! requires allocation */
        
        int t;
        
        for(i = 0; i < 4; i++)
        {
                t = (i / pow(10, 4*i)) % pow(10, 4);
                hex[i] = singleHex(t);
        }
        
        for(i = 0; i < 4; i++)
                ans[i] = hex[4-i];
        ans[i] = '\0';
        
        return ans;
}

char singleHexa(int bin) /* gets 4-digits binary num and returns hexa char */
{
        int num = 0;
        
        for(i = 0; i < 4; i++)
        {
                if(bin % 10)
                        num += pow(2, i);
        }   
        
        if(num >= 0 && num <= 9)
                return '0' + num;
        
        if(num >= 10 && num <= 15)
                return 'A' + num - 10;
}

char *addressToHexa(int num) /* gets decimal num smaller ehan 256 and returns hexa string */
{
        char hex[3]; /* !!! requires allocation */
        
        hex[2] = '\0';
        hex[1] = intToHexa(num % 16);
        hex[0] = intToHexa(num / 16);
        
        return hex;
}

char intToHexa(int num)
{
        if(num >= 0 && num <= 9)
                return '0' + num;
        
        if(num >= 10 && num <= 15)
                return 'A' + num - 10;
}
