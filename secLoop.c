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
        int curWords;
        int curAddress;
        int bin;
        
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
                        
                        if(curWords > 0)
                        {
                                buildOperand(codeCur->next, codeCur->firstOperand, codeCur->encodeType)
                                bin = encode(codeCur->next, codeCur->next->encode);
                                addBinary(curAddress+1, bin);
                        }
                        
                        if(curWords > 1)
                        {
                                buildOperand(codeCur->next->next, codeCur->secndOperand, codeCur->encodeType)
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
			break;
			
		case ONE_REGISTER: /* ra */
			c->reg1 = atoi(operand[1]);
			break;
			
		case TWO_REGISTER:
			/* In the special case of two register addressing we build the operand word in the first loop */
			break;
		
		case MAIN_COMMAND: /* Built in the first loop */
			break;
	}
}

int encode(struct cmd *code, int encode);
int intToBinary(int num);
