#include "utils.h"
#include "table.h"

extern int isError;
struct list table;
struct symbol *symbolTable = NULL;

void addCmdToList(struct cmd *c)
{
	struct cmd *n;
	n = (struct cmd*) malloc(sizeof(struct cmd));
	/* !!! malloc - to make sure to free the pointer */
	/* !!! to check - if a struct field is unintaliize or containing a garbage values - can it be copied? */
	n->encode = c->encode;
	n->opcode = c->opcode;
	n->group = c->group;
	n->firstOperand = c->firstOperand;
	n->secndOperand = c->secndOperand;
	n->firstAddressing = c->firstAddressing;
	n->secndAddressing = c->secndAddressing;
	n->operandNumber = c->operandNumber;
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
	/* !!! to check - if a struct field is unintaliize or containing a garbage values - can it be copied? */
	
	if(table.cmdHead == NULL)
		table.cmdHead = n;

	else
	{
		struct cmd *cur;
		cur = table.cmdHead;
		while(cur->next)
			cur = cur->next;
		cur->next = n;
	}
}

void addDataToList(struct data *d) /* DATA = data OR string */
{
	struct data *n;
	n = (struct data*) malloc (sizeof(struct data));
	/* !!! malloc - to make sure to free the pointer */	
	/* !!! to check - if a struct field is unintaliize or containing a garbage values - can it be copied? */
	n->isFirst = d->isFirst;
	n->wordsNum = d->wordsNum;
	n->address = d->address;
	n->content = d->content;
	n->isSymbol = d->isSymbol;
	n->symbol = d->symbol;
	n->next=NULL;	
	
	if(table.dataHead == NULL)
		table.dataHead = n;

	else
	{
		struct data *cur;
		cur = table.dataHead;
		while(cur->next)
			cur = cur->next;
		cur->next = n;
	}
}

void addExtToList(struct ext *e)
{
	struct ext *n;
	n = (struct ext*) malloc(sizeof(struct ext));
	/* !!! malloc - to make sure to free the pointer */
	n->symbol = e->symbol;
	n->next = NULL;
	
	if(table.extHead == NULL)
		table.extHead = n;

	else
	{
		struct ext *cur;
		cur = table.extHead;
		while(cur->next)
			cur = cur->next;
		cur->next = n;
	}
}

void addEntToList(struct ent *e)
{
	struct ent *n;
	n = (struct ent*) malloc(sizeof(struct ent));
	/* !!! malloc - to make sure to free the pointer */
	n->symbol = e->symbol;
	n->next = NULL;
	
	if(table.entHead == NULL)
		table.entHead = n;

	else
	{
		struct ent *cur;
		cur = table.entHead;
		while(cur->next)
			cur = cur->next;
		cur->next = n;
	}
}

int addCmd(char *cmd, int address)
{	
	int i;	
	char inst[op_name_max+1]; /* inst. = instruction */
	struct cmd *new;
	struct cmd *nextWord, *nextNextWord;

	new = (struct cmd*) malloc(sizeof(struct cmd));
	nextWord = (struct cmd*) malloc(sizeof(struct cmd));
	nextNextWord = (struct cmd*) malloc(sizeof(struct cmd));
	
	new->encode = MAIN_COMMAND;
	new->operandNumber = CMD;
	new->encodeType = A;
	new->address = address;
	
	new->isSymbol = isSymbol(cmd);
	if(new->isSymbol)
		new->symbol = getSymbol(cmd);

	i = getCmdStart(cmd);
	strncpy(inst, cmd+i, op_name_max);
	inst[op_name_max] = '\0';
	new->opcode = getOpcode(inst);
	new->group = getGroup(inst);

	if(new->group > 0)
	{
		new->firstOperand = getFirstOperand(cmd+i);
             	new->firstAddressing = getAddressing(new->firstOperand);	
        }

	if(new->group > 1)
	{
		new->secndOperand = getSecndOperand(cmd+i);
		new->secndAddressing = getAddressing(new->secndOperand);
	}

	addCmdToList(new);

	nextWord->address = address+1;
	nextNextWord->address = address+2;

	if(new->group == two_operands && new->firstAddressing == register_addressing && new->secndAddressing == register_addressing)
	{
		new->wordsNum = new->group-1;
		nextWord->encode = TWO_REGISTER;
		earlyBuild(nextWord, strcat(new->firstOperand, new->secndOperand));
		/*
			Usually we build at the first loop only the main commands words,
			And we build the operands words only at the second loop.
			In the special case of two register addressing, we will build the operand word
			in the first loop with the function earlyBuild
		*/
		addCmdToList(nextWord);
		return (new->wordsNum) + 1; /* including the main command word */
	}

	new->wordsNum = new->group;
	
	if(new->wordsNum == 1)
	{
		nextWord->encode = new->firstAddressing;
		nextWord->operandNumber = LAST;
		addCmdToList(nextWord);
	}
		
	if(new->wordsNum == 2)
	{
		nextWord->encode = new->firstAddressing;
		nextWord->operandNumber = FIRST;
		addCmdToList(nextWord);
		
		nextNextWord->encode = new->secndAddressing;
		nextNextWord->operandNumber = LAST;
		addCmdToList(nextNextWord);
	}
	free(new);
	free(nextWord);
	free(nextNextWord);
	return (new->wordsNum) + 1; /* including the main command word */
}

int addData(char *cmd, int address)
{
	int i, len = strlen(cmd);
	struct data *new;
	int wordsNum, count = 0;
	int start, end, num;
	char *tempNum;
	new = (struct data *) malloc (sizeof (struct data)); 
	
	/* first */
	new->isFirst = TRUE;
	new->address = address;
	new->isSymbol = isSymbol(cmd);
	if(new->isSymbol)
		new->symbol = getSymbol(cmd);
	
	i = getCmdStart(cmd);
	wordsNum = countWords(cmd+i);
	new->wordsNum = wordsNum;

	i += data_length;
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces */
		i++;
	
	if(i == len || cmd[i] == '\0')
	{
		fprintf(stderr, "Error - No number entered \n");
		isError = TRUE;
		return 0;
	}
	
	start = i;
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t') /* skip the number */
		i++;
	end = i-1;

	tempNum = (char *) malloc((end-start+1)* sizeof(char));
	strncpy(tempNum, cmd+start, end-start+1);
	
	num = atoi(tempNum);
	if(strlen(tempNum) == 1 && tempNum[0] == '0');
		/* num is 0 */
	else if(num == 0)
	{
		fprintf(stderr, "Error - %s isn't a number \n", tempNum);
		isError = TRUE;
		return 0;
		/* tempNum is not "0" but atoi returns 0
		-> tempNum is not a number */
	}
	
	new->content = num;
	addDataToList(new);
	count++;
	
	new->isFirst = FALSE;
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces*/
		i++;
	
	while(i < len && cmd[i] != '\0')
	{
		if(cmd[i] != ',')
		{
			fprintf(stderr, "Error - No comma \n");
			isError = TRUE;
		}
		
		while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces*/
			i++;
		
		if(i == len || cmd[i] == '\0')
		{
			fprintf(stderr, "Error - Number Expected \n");
			isError = TRUE;
		}
			
		start = i;
		while(i < len && cmd[i] != ' ' && cmd[i] != '\t') /* skip the number*/
			i++;
		end = i-1;
		strncpy(tempNum, cmd+start, end-start+1);
		
		num = atoi(tempNum);
		if(strlen(tempNum) == 1 && tempNum[0] == '0');
			/* num is 0*/
		else if(num == 0)
		{
			fprintf(stderr, "Error - %s isn't a number \n", tempNum);
			isError = TRUE;
			return count;
			/* tempNum is not "0" but atoi returns 0
			-> tempNum is not a number */
		}
		
		new->address++;
		new->content = num;
		addDataToList(new);
		count++;
		
		while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces */
			i++;
	}
	free(new);
	return wordsNum;
}

int addStr(char *cmd, int address)
{
	int i, len = strlen(cmd);
	struct data *new;
	int wordsNum;
	
	new = (struct data *) malloc (sizeof (struct data)); 
	/* first */
	new->isFirst = TRUE;
	new->address = address;
	new->isSymbol = isSymbol(cmd);
	if(new->isSymbol)
		new->symbol = getSymbol(cmd);
	
	i = getCmdStart(cmd);
	wordsNum = countWords(cmd+i);
	new->wordsNum = wordsNum;
	
	i += string_length;
	
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces */
		i++;
	
	if(cmd[i] != '"')
	{
		fprintf(stderr, "Error - String without quotation marks \n");
		isError = TRUE;
		return 0;
	}	
	i++;
	
 	new->content = cmd[i];
 	addDataToList(new); /* add the first char */
	i++;
	new->isFirst = FALSE;
		
	while(i < len && cmd[i] != '"')
	{
		(new->address)++;
		new->content = cmd[i];
		addDataToList(new);
		i++;
	}
	/* add zero to the end of the string */
	new->address++;
	new->content = 0;
	addDataToList(new);
	
	if(i == len)
	{
		fprintf(stderr, "Error - No quotation marks at the end \n");
		isError = TRUE;
	}
	
	return wordsNum;
}

void addExt(char *cmd)
{
	struct ext *new;
	char *symbol;
	int i = getCmdStart(cmd) + extern_length;
	int len = strlen(cmd);

	new = (struct ext *) malloc (sizeof (struct ext)); 
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces */
		i++;
	
	symbol = (char *)malloc((len-i) * sizeof(char));

	strncpy(symbol, cmd+i, len-i);
	new->symbol = symbol;
	addExtToList(new);
	free(new);
}

void addEnt(char *cmd)
{
	struct ent *new;
	char *symbol;
	int i = getCmdStart(cmd) + entry_length;
	int len = strlen(cmd);

	new = (struct ent *) malloc (sizeof (struct ent)); 
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces */
		i++;
	
	symbol = (char *)malloc((len-i) * sizeof(char));

	strncpy(symbol, cmd+i, len-i);
	new->symbol = symbol;
	addEntToList(new);
	free(new);
}

void addSymbol(char *name, int type, int address)
{
        struct symbol *new;
        new = (struct symbol *) malloc (sizeof (struct symbol)); /* malloc - requires to free the allocation*/
        new->name = name;
        new->type = type;
        new->address = address;
        new->next = NULL;
        
        if(symbolTable == NULL)
                symbolTable = new;                
        else
        {
            struct symbol *cur=symbolTable;
	    while(cur->next)
		 cur=cur->next;
	    cur->next=new;
        }
}

void fixAddresses(int add) /* fix so the data addresses will come right after the cmd addresses */
{
	struct data *cur;
	cur = table.dataHead;
	while(cur && cur->next)
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
	
	cCur = table.cmdHead;
	while(cCur)
	{
		if(cCur->isSymbol)
			addSymbol(cCur->symbol, CODE, cCur->address);
		else	
			/*error*/;
		cCur = cCur->next;
	}

	dCur = table.dataHead;
	while(dCur)
	{
		if(dCur->isSymbol)
			addSymbol(dCur->symbol, DtSt, dCur->address);
		else
			/* error */;
		dCur = dCur->next;
	}
	
	eCur = table.extHead;
	while(eCur)
	{
			addSymbol(eCur->symbol, EXT,0);
			eCur = eCur->next;
	}
	
}

/* Build the operand word in case of two register addressing */
void earlyBuild(struct cmd *c, char *reg) /* rarb*/
{
	c->encodeType = A;
	c->reg1 = reg[1] - '0';
	c->reg2 = reg[3] - '0';
}
