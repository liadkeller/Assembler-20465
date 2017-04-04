#include "utils.h"
#include "binary.h"

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
                                buildOperand(codeCur->next, codeCur->firstOperand)
                                bin = encode(codeCur->next, codeCur->next->encode);
                                addBinary(curAddress+1, bin);
				free(bin);
                        }
                        
                        if(curWords > 1)
                        {
                                buildOperand(codeCur->next->next, codeCur->secndOperand)
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

void buildOperand(struct cmd *c, char *operand)
{
	switch(c->encode)
	{
		case NUMBER: /* #a */
			c->encodeType = A;
			c->number = atoi(++operand); /* copies the number without the # */
			break;
			
		case ADDRESS: /* LABEL */
			{
				struct symbol *symbolCur = symbolTable;
				int isFound = FALSE;
			
				while(symbolCur->next && (!isFound))
				{
					if(strcmp(symbolCur->name, operand))
					{
						isFound = TRUE;
						c->addressNumber = symbolCur->address;
						c->encodeType = (symbolCur->type == EXT && symbolCur->address == 0) ? (E) : (R);
					}
					symbolCur = symbolCur->next;
				}
			}
			break;
			
		case INDEX_REGISTER: /* ra[rb] */
			c->encodeType = A;
			c->reg1 = operand[1]-'0';
			c->reg2 = operand[4]-'0';
			if((c->reg1) % 2 == 0 || (c->reg2) % 2 == 1)
				fprintf(stderr, "Error - First register should be odd and second register should be even");
			break;
			
		case ONE_REGISTER: /* ra */
			c->encodeType = A;
			c->reg1 = operand[1]-'0';
			if(c->operandNumber == FIRST)
				c->whichReg = SOURCE;
			else
				c->whichReg = DEST;
			break;
			
		case TWO_REGISTER:
			/* In the special case of two register addressing we built the operand word in the first loop */
			break;
	}
}
