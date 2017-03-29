struct binarycode
{
        int address;
        int binary;
        struct binarycode *next;
}

struct binarycode binaryTable = NULL;


void secondLoop()
{
        struct cmd *cur;
        struct cmd *next;
        struct cmd *nextNext;
        int curGroup;
        int curAddress;
        
        cur = table->cmdHead;
        
        while(cur->next)
        {
                if(cur->encode == MAIN_COMMAND)
                {
                        curGroup = cur->group;
                        curAddress = cur->address;
                        
                        int bin = encode(/*par*/);
                        addBinary(curAddress, bin);
                        
                        if(curGroup > 0)
                        {
                                bin = encode(/*curNextOperand, curNextAddressing, curNextEncodeType, curNextEncode(A/R/E)*/);
                                addBinary(curAddress+1, bin);
                        }
                        
                        if(curGroup > 1)
                        {
                                bin = encode(/*curNextOperand, curNextAddressing, curNextEncodeType, curNextEncode(A/R/E)*/);
                                addBinary(curAddress+2, bin);
                        }
                }
                cur = cur->next;
        }

}


