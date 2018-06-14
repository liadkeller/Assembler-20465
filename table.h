struct cmd {
	int encode;
	int opcode;
	int group;
	char *srcOperand;
	char *destOperand;
	int srcAddressingType;
	int destAddressingType;
	int address;
	int hasSymbol;
	char *symbol;
	int operandsNumber; /*How many operands*/

	int operandRole; /* Source / Dest */
	int numberVal; /* When the word represents a number, the number's value */
	int addressVal; /* When the word represents an address, the address value */
	char *externSymbol; /* When the word represents and external symbol, the symbol's string */
	int srcReg; /* for reguler register/two registers */
	int destReg; /* for reguler register/two registers */
	int reg1; /* for register indexing*/
	int reg2; /* for register indexing*/

	int encodeType; /*A R E*/
};

struct data {	/* DATA = data OR string*/
	int address;
	int value;
	int hasSymbol;
	char *symbol;
};

struct ext {
	char *symbol;
};

struct ent {
	char *symbol;
};


void addCommand(char **line);
void addData(char **line);
void addStr(char **line);
void addExt(char **line);
void addEnt(char **line);
void incAddresses();
int getSymbolAddress(char *label);

char **getCleanArray(char *cmd);
char *getCleanLabel(char *label);
int hasLabel(char **line);
int getArraySize(char **array);
int isOperation(char *op);
int getOpcode(char *op);
int getGroup(char *op);
int checkOperand(char *operand);
int checkSymbol(char *cmd);
int getAddressingType(char *operand);

int isNumber(char *);
int isOperand(char *);
