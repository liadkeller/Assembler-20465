struct cmd {
	int encode;
	int opcode;
	int group;
	char *firstOperand; /* source*/
	char *secndOperand; /* destniation*/
	int firstAddressing;
	int secndAddressing;
	int wordsNum;	
	int address;
	int isSymbol;
	char *symbol;
	
	int operandNumber;
	int number;
	int addressNumber;
	
	int reg1;
	int reg2;
	int whichReg; /* SOURCE  DEST*/

	int encodeType; /*A R E*/

	struct cmd *next;
};

struct data {	/* DATA = data OR string*/
	int isFirst;
	int wordsNum;
	int address;
	int content;
	int isSymbol;
	char *symbol;
	
	struct data *next;
};

struct ext {
	char *symbol;
	struct ext *next;
};

struct ent {
	char *symbol;
	struct ext *next;
};

struct list {
	struct cmd *cmdHead;
	struct data *dataHead;
	struct ext *extHead;
	struct ent *entHead;
};

struct symbol {
        char *name;
        int type;
        int address;
        struct symbol *next;
};

void addCmdToList(struct cmd *c);
void addDataToList(struct data *d); /* DATA = data OR string*/
void addExtToList(struct ext *e);
void addEntToList(struct ext *e);
int addCmd(char *cmd, int address);
int addData(char *cmd, int address);
int addStr(char *cmd, int address);
void addExt(char *cmd);
void addEnt(char *cmd)
int addSymbol(char *name, int type, int address);

void fixAddresses(int add);
void buildSymbolTable();
void earlyBuild(struct cmd *c, char *reg);
	

int isSymbol(char *cmd); /* CMD = code OR data*/
char *getSymbol(char *cmd); /* CMD = code OR data*/
int checkSymbol(char *cmd);
int getCmdStart(char *cmd); /* CMD = code OR data*/
int isBlankOrComment(char *cmd);
int isCode(char *cmd);
int isData(char *cmd); /* CMD = code OR data*/
int isStr(char *cmd); /* CMD = code OR data*/
int isExt(char *cmd);
int isEnt(char *cmd);
int getOpcode(char *op);
int getGroup(char *op);
char *getFirstOperand(char *cmd);
char *getSecndOperand(char *cmd);
int getAddressing(char *operand);
int countWords(char *cmd);
int skipSpaces(int i,char *str);


