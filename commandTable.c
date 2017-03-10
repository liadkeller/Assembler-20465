#include "table.h"

void addCmd(char *cmd, int address)
{
	int i;	
	char inst[3];

	struct cmd *new;

	new->encode = MAIN_COMMAND;
	new->address = address;
	new->isSymbol = isSymbol(cmd);
	if(new->isSymbol)
		new->symbol = getSymbol(cmd);

	i = getCmdStart(cmd);
	strncpy(inst, cmd+i, 3);
	new->opcode = getOpcode(inst);
	new->group = getGroup(inst);

	if(new->group > 0)
	{
		new->firstOperand = getFirstOperand(cmd, i);
		new->firstAddressing = getFirstAddressing(new->firstOperand)	
	}

	if(new->group > 1)
	{
		new->secndOperand = getSecndOperand(cmd, i);
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

