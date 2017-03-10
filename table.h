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

struct list {
	struct cmd *cmdHead;
	struct data *dataHead;
}
