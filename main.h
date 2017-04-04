int isBlankOrComment(char *cmd);
int isCode(char *cmd);
int isData(char *cmd);
int isStr(char *cmd);
int isExt(char *cmd);
int isEnt(char *cmd);

int addCmd(char *cmd, int address);
int addExt(char *cmd);
int addData(char *cmd, int address);
int addStr(char *cmd, int address);

void fixAddresses(int add);
void buildSymbolTable(void);

void firstLoop(FILE *);
void secondLoop();
