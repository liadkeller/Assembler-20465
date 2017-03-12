#include "table.h"
#include "utils.h"

struct list *table;

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
	n->next = NULL;
	
	if(t->cmdHead == NULL)
		cmdHead = n;

	else
	{
		c = cmdHead; // uses c as temp
		while(c->next)
			c = c->next;
		c->next = n;
	}
}

void addCmd(char *cmd, int address)
{
	int i;	
	char inst[op_name_size+1];

	struct cmd *new;

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
	
	new->encodeType = A; // will be defined as constant

	addCmdToList(new, table);



	if(new->group == 2 && new->firstAddressing = 3 && new->secndAddressing == 3) // 2 = TWO OPERANDS / 3 = ADRESSING MIUN
	{
		new->wordsNum = group-1;
		addCmdToList(makeNext(TWO_REGISTER, address+1), table);
	}

	else
		new->wordsNum = group;
	
	

	if(new->wordsNum = 1)
		addCmdToList(makeNext(secndAddressing, address+1), table);

	if(new->wordsNum = 2)
	{
		addCmdToList(makeNext(firstAddressing, address+1), table);
		addCmdToList(makeNext(secndAddressing, address+2), table);
	}
}

struct cmd *makeNext(int encode, int address)
{
	struct cmd *next; // malloc *check* needed;
	next->encode = encode;	
	next->address = address;

	return next;
}
