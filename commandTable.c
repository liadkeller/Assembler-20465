
enum encode {NUMBER, ADDRESS, NUM2, ONE_REGISTER, TWO_REGISTER, MAIN_COMMAND}

struct cmd {
	int encode;
	int opcode;
	int group;
	char *firstOperand; // source
	char *secndOperand; // destniation
	int firstAddressing;
	int secndAddressing;
	int wordsNum;	
	int address;
	int isSymbol;
	char *symbol;
	
	int number;
	int addressNumber;
	
	int reg1;
	int reg2;
	int whichReg; // SOURCE / DEST

	int encodeType; // A R E

	struct cmd *next;
}

void addCmd(char *cmd)
{
	int i;	
	char inst[3];

	struct cmd *new; // malloc needed; connection to the list needed;
	// maybe will add command field

	new->encode = MAIN_COMMAND
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

	if(new->group == 2 && new->firstAddressing = 3 && new->secndAddressing == 3) // 2 = TWO OPERANDS / 3 = ADRESSING MIUN
	{
		new->wordsNum = group-1;
		new->next = addNext(TWO_REGISTER, getFirstOperandContent(new), getSecndOperandContent(new));
	}

	else
		new->wordsNum = group;

	if(new->wordsNum = 1)
		new->next = addNext(secndAddressing, getSecndOperandContent(new), DEST); // define DEST

	if(new->wordsNum = 2)
	{
		new->next = addNext(firstAdressing, getFirstOperandContent(new), SOURCE);
		new->next->next = addNextNext(secndAddressing, getSecndOperandContent(new), DEST);
	}

	new->address = incAdress();
	new->encodeType = A; // will be defined as constant
}

struct cmd *addNext(int encode, int first, int secnd) // secnd - source or dest
{
	struct cmd *next; // malloc needed;
	next->encode = encode;

	switch(encode)
	{
		case NUMBER:
			next->number = first;
			next->encodeType = A;
			break;
		case ADDRESS:
			next->addressNumber = first;
			next->encodeType = R;
			break;
		case ONE_REGISTER:
			next->r1 = first;
			next->whichReg = secnd;
			next->encodeType = A
			break;
		case TWO_REGISTER:
			next->r1 = first;
			next->r2 = secnd;
			next->encodeType = A;
			break;
	}	

	return next;
}

struct cmd *addNextNext(int encode, int num)
{
	struct cmd *nextNext; // malloc needed;
	nextNext->encode = encode;

	switch(encode)
	{
		case NUMBER:
			nextNext->number = num;
			nextNext->encodeType = A;
			break;
		case ADDRESS:
			nextNext->addressNumber = num;
			nextNext->encodeType = R;
			break;
		case ONE_REGISTER:
			nextNext->r1 = num;
			nextNext->whichReg = DEST;
			nextNext->encodeType = A
			break;
	}

	return nextNext;	
}

