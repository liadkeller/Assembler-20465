void addCmdToList(struct cmd *c, struct list *t)
{
	struct cmd *n;
	n = (struct cmd *) malloc (sizeof struct cmd); // to check if this is the right way to use sizeof
	
	n->encode = c->encode;
	n->opcode = c->opcode;
	n->group = c->group;
	n->firstOperand = c->firstOperand;
	n->secndOperand = c->secndOperand;
	n->firstAddressing = c->firstAddressing;
	n->secndAddressing = c->secndAddressing;
	n->wordsNum = c->wordsNum;
	n->address = c->address;
	n->isSymbol = c->isSymbol;
	n->symbol = c->symbol;
	n->number = c->number;
	n->addressNumber = c->addressNumber;
	n->reg1 = c->reg1;
	n->reg2 = c->reg2;
	n->whichReg = c->whichReg;
	n->encodeType = c->encodeType;
	n->next = t->dataHead
	
	if(t->cmdHead == NULL)
		cmdHead = n;

	else
	{
		c = cmdHead; // uses c as temp
		while(c->next != t->dataHead)
			c = c->next;
		c->next = n;
	}
}
