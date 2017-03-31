int encode(struct cmd *code, int encode);
int intToBinary(int num, int size) /* returns the binary presentation of the first "size" digits of num */
{
        int arr[size];
        int i, bin;
        
        for(i = 0; num > 0 && i < size; i++)
        {
                arr[i] = num % 2;
                num = num / 2;
        }
        
        if(i == size)
                i--;
        
        for(bin = 0; i >= 0; i--)
        {
                if(arr[i])
                        bin += pow(10, i);
        }
        
        return bin;
}

/* adds num to bin from the start bit
if bin = 011100000 and num = 11 and start = 2
then bin will be 011101100 */
int makeBin(int bin, int num, int start)
{
        int i = start;
        int j = 0;
        
        int size = getSize(num);
        while(j < size)
        {
                if(getBit(num, j))
                        bin += pow(10, i);

                i++;
                j++;
        }
        
        return bin;
}

int getSize(int num) /* return the number of digits of num */
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

int getDig(int num, int dig) /* return the digit of the number */
{
        num = num / pow(10, dig);
        return num % 10;
}

int pow(int a, int b)
{
        int i, res;
        for(i = 0, res = 1; i < b; i++, res *= a);
        return res;
}
