struct binarycode
{
        int address;
        int binary;
        struct binarycode *next;
}

struct binarycode binaryTable = NULL;

void addBinary(int address, int bin)
{
	struct binarycode *n;
	n = (struct binarycode*) malloc(sizeof(struct binarycode));
	/* !!! malloc - to make sure to free the pointer*/
	n->address = address;
        n->binary = bin;
        n->next = NULL;
	
	if(binaryTable == NULL)
		binaryTable = n;

	else
	{
		struct binarycode *cur;
		cur = binaryTable;
		while(cur->next)
			cur = cur->next;
		cur->next = n;
	}
}


void secondLoop()
{
        struct cmd *codeCur;
        struct data *dataCur;
        int curGroup;
        int curAddress;
        int bin;
        
        codeCur = table->cmdHead;
        dataCur = table->dataHead;
        
        while(codeCur->next)
        {
                if(codeCur->encode == MAIN_COMMAND)
                {
                        curGroup = codeCur->group;
                        curAddress = codeCur->address;
                        
                        bin = encode(codeCur, codeCur->encode);
                        addBinary(curAddress, bin);
                        
                        if(curGroup > 0)
                        {
                                buildFirstOperand(codeCur, codeCur->firstOperand, codeCur->firstAddressing, codeCur->encodeType)
                                bin = encode(codeCur->next, codeCur->next->encode);
                                addBinary(curAddress+1, bin);
                        }
                        
                        if(curGroup > 1)
                        {
                                buildSecndOperand(codeCur, codeCur->secndOperand, codeCur->secndAddressing, codeCur->encodeType)
                                bin = encode(codeCur->next->next, codeCur->next->next->encode);
                                addBinary(curAddress+2, bin);
                        }
                }
                codeCur = codeCur->next;
        }
        
        while(dataCur->next)
        {
                curAddress = dataCur->address;
                bin = intToBinary(dataCur->content);
                addBinary(curAddress, bin);
        }

}


