char *encode(struct cmd *code, int encode)
{
        char bin[BINARY_WORD]; /* malloc */
        
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
                        addBin(bin, intToBinary(code->opcode, 2), 10, 2);
                        addBin(bin, "111", 12, 3);
                        break;
        }
        
}

char *intToBinary(int num, int size) /* returns the binary presentation of the first "size" digits of num */
{
        /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !!!!! REQIRES TREATMENT OF NEGATIVE NUMBERS !!!!!
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
        
        int arr[size];
        int i;
        char bin[size];  /* malloc */
        
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
void addBin(char *bin, char *num, int start, int size)
{
        int i = start;
        int j = 0;
        
        while(j < size)
        {
                if(num[j] == '1')
                        bin[i] = '1';

                i++;
                j++;
        }
}
