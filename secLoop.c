#define BINARY_WORD 15

struct binarycode
{
        int address;
        char binary[BINARY_WORD];
        struct binarycode *next;
}

struct binarycode binaryTable = NULL;

void addBinary(int address, char *bin)
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
        int curWords;
        int curAddress;
        char *bin;
        
        codeCur = table->cmdHead;
        dataCur = table->dataHead;
        
        while(codeCur->next)
        {
                if(codeCur->encode == MAIN_COMMAND)
                {
                        curWords = codeCur->wordsNum;
                        curAddress = codeCur->address;
                        
                        bin = encode(codeCur, codeCur->encode);
                        addBinary(curAddress, bin);
			free(bin);
                        
                        if(curWords > 0)
                        {
                                buildOperand(codeCur->next, codeCur->firstOperand, codeCur->encodeType)
                                bin = encode(codeCur->next, codeCur->next->encode);
                                addBinary(curAddress+1, bin);
				free(bin);
                        }
                        
                        if(curWords > 1)
                        {
                                buildOperand(codeCur->next->next, codeCur->secndOperand, codeCur->encodeType)
                                bin = encode(codeCur->next->next, codeCur->next->next->encode);
                                addBinary(curAddress+2, bin);
				free(bin);
                        }
                }
                codeCur = codeCur->next;
        }
        
        while(dataCur->next)
        {
                curAddress = dataCur->address;
                bin = intToBinary(dataCur->content, BINARY_WORD);
                addBinary(curAddress, bin);
		free(bin);
        }

}

void buildOperand(struct cmd *c, char *operand, int encodeType)
{
	c->encodeType = encodeType;
	
	switch(c->encode)
	{
		case NUMBER: /* #a */
			c->number = atoi(++operand); /* copies the number without the # */
			break;
			
		case ADDRESS: /* LABEL */
			{
				struct symbol *symbolCur = symbolTable;
				int isFound = FALSE;
			
				while(symbolCur->next && (!isFound))
				{
					if(strcmp(symbolCur->label, operand))
					{
						c->addressNumber = symbolCur->address;
						isFound = TRUE;
					}
	
					symbolCur = symbolCur->next;
				}
			}
			break;
			
		case INDEX_REGISTER: /* ra[rb] */
			c->reg1 = atoi(operand[1]);
			c->reg2 = atoi(operand[4]);
			if((c->reg1) % 2 == 0 || (c->reg2) % 2 == 1)
				fprintf(stderr, "Error - First register should be odd and second register should be even");
			break;
			
		case ONE_REGISTER: /* ra */
			c->reg1 = atoi(operand[1]);
			if(c->operandNumber == FIRST)
				c->whichReg = SOURCE;
			else
				c->whichReg = DEST;
			break;
			
		case TWO_REGISTER:
			/* In the special case of two register addressing we built the operand word in the first loop */
			break;
		
		case MAIN_COMMAND: /* Built in the first loop */
			break;
	}
}
