#include "table.h"
#include "utils.h"

struct list *table;

void addCmdToList(struct cmd *c, struct list *t)
{
	struct cmd *n;
	n = (struct cmd *) malloc (sizeof struct cmd);
	// !!! to check if this is the right way to use sizeof
	// !!! malloc - to make sure to free the pointer
	// !!! to check - if a struct field is unintaliize or containing a garbage values - can it be copied?
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
	n->next = NULL;
	// !!! to check - if a struct field is unintaliize or containing a garbage values - can it be copied?
	
	if(t->cmdHead == NULL)
		t->cmdHead = n;

	else
	{
		struct cmd *cur;
		cur = t->cmdHead;
		while(cur->next)
			cur = cur->next;
		cur->next = n;
	}
}

void addDataToList(struct data *d, struct list *t)
{
	struct data *n;
	n = (struct data *) malloc (sizeof struct data);
	// !!! to check if this is the right way to use sizeof
	// !!! malloc - to make sure to free the pointer
	
	// !!! to check - if a struct field is unintaliize or containing a garbage values - can it be copied?
	n->isFirst = d->isFirst;
	n->wordsNum = d->wordsNum;
	n->address = d->address;
	n->content = d->content;
	n->isSymbol = d->isSymbol;
	n->symbol = d->symbol;
	
	
	
	if(t->dataHead == NULL)
		t->dataHead = n;

	else
	{
		struct data *cur;
		cur = t->dataHead;
		while(cur->next)
			cur = cur->next;
		cur->next = n;
	}
}

void addExtToList(struct data *e, struct list *t)
{
	struct ext *n;
	n = (struct ext *) malloc (sizeof (struct ext));
	// !!! to check if this is the right way to use sizeof
	// !!! malloc - to make sure to free the pointer
	n->symbol = e->symbol;
	
	if(t->extHead == NULL)
		t->extHead = n;

	else
	{
		struct ext *cur;
		cur = t->extHead;
		while(cur->next)
			cur = cur->next;
		cur->next = n;
	}
}

int addCmd(char *cmd, int address)
{	
	int i;	
	char inst[op_name_size+1]; // inst. = instruction

	struct cmd *new;
	struct cmd *nextWord, *nextNextWord;

	new->encode = MAIN_COMMAND;
	new->address = address;
	new->isSymbol = isSymbol(cmd);
	if(new->isSymbol)
		new->symbol = getSymbol(cmd);

	i = getCmdStart(cmd);
	strncpy(inst, cmd+i, op_name_size);
	inst[op_name_size] = '\0';
	new->opcode = getOpcode(inst);
	new->group = getGroup(inst);

	if(new->group > 0)
	{
		new->firstOperand = getFirstOperand(cmd+i);
		new->firstAddressing = getFirstAddressing(new->firstOperand)	
	}

	if(new->group > 1)
	{
		new->secndOperand = getSecndOperand(cmd+i);
		new->secndAddressing = getSecndAddressing(new->secndOperand)	
	}
	
	new->encodeType = A; // !!! will be defined as constant

	addCmdToList(new, table);

	
	nextWord->address = address+1;
	nextNextWord->address = address+2;

	if(new->group == two_operands && new->firstAddressing = register_addressing && new->secndAddressing == register_addressing)
	{
		new->wordsNum = group-1;
		
		nextWord->encode = TWO_REGISTER;
		addCmdToList(nextWord, table);
	}

	else
		new->wordsNum = group;
	
	if(new->wordsNum = 1)
	{
		nextWord->encode = secndAddressing;
		addCmdToList(nextWord, table);
	}
		
	if(new->wordsNum = 2)
	{
		nextWord->encode = firstAddressing;
		nextNextWord->encode = secndAddressing;
		addCmdToList(nextWord, table);
		addCmdToList(nextNextWord, table);
	}
	
	return wordsNum;
}

int addData(char *cmd, int address)
{
	int i, len = strlen(cmd);
	struct data *new;
	int wordsNum;
	
	// first
	new->isFirst = TRUE;
	new->address = address;
	new->isSymbol = isSymbol(cmd);
	if(new->isSymbol)
		new->symbol = getSymbol(cmd);
	
	i = getCmdStart(cmd);
	wordsNum = countWords(cmd+i);
	new->wordsNum = wordsNum;
	addDataToList(new, table);
	
	/*
	if(i = isData(cmd, i))
	{
		
		
		
	}
	*/
	
	
	return wordsNum;
}

int addStr(char *cmd, int address)
{
	int i, len = strlen(cmd);
	struct data *new;
	int wordsNum;
	
	// first
	new->isFirst = TRUE;
	new->address = address;
	new->isSymbol = isSymbol(cmd);
	if(new->isSymbol)
		new->symbol = getSymbol(cmd);
	
	i = getCmdStart(cmd);
	wordsNum = countWords(cmd+i);
	new->wordsNum = wordsNum;
	addDataToList(new, table);
	
	if(cmd[i] != '"')
		//error	
	i++;
		
	while(cmd[i] != '\0')
	{
		new->isFirst = FALSE;
		new->address++; // !!! לבדוק תקינות של סדר קדימויות
		new->content = cmd[i]; // !!! לבדוק המרה לאינטגר
		addDataToList(new, table);
		
		i++;
	}
	// add zero to the end of the string
	new->address++;
	new->content = 0;
	addDataToList(new, table);
	
	return wordsNum;
}

void addExt(char *cmd)
{
	struct ext *new;
	char *symbol; // (*) check allocation
	int i = getCmdStart(cmd) + extern_length; // !! TO CHECK while debugging
	int len = strlen(cmd);
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t'))
		i++;
	strncpy(symbol, cmd+i, len-i);  // (*) check allocation
	new->symbol = symbol;
	addExtToList(new, table);
}

void fixAddresses(int add) // fix so the data addresses will come right after the cmd addresses
{
	struct data *cur;
	cur = dataHead;
	while(cur->next)
	{
		cur->address += add; 
		cur = cur->next;
	}
}

void buildSymbolTable()
{
	struct cmd *cCur;
	struct data *dCur;
	struct ext *eCur;
	
	cCur = cmdHead;
	while(cCur)
		if(cCur->isSymbol) // !!! To check if isSymbol always TRUE or FALSE
			addSymbol(cCur->symbol, CODE, cCur->address);

	dCur = dataHead;
	while(dCur)
		if(dCur->isSymbol) // !!! To check if isSymbol always TRUE or FALSE
			addSymbol(dCur->symbol, DtSt, dCur->address);
	
	eCur = extHead;
	while(eCur)
		if(eCur->isSymbol) // !!! To check if isSymbol always TRUE or FALSE
			addSymbol(eCur->symbol, EXT, eCur->address);
}
