#include <string.h>

#define data_length 5 // .data
#define string_length 7 // .string
#define extern_length 7 // .extern

#define op_name_size 3
#define op_num 16

enum BOOL {FALSE , TRUE}

int isSymbol(char *cmd); // CMD = code OR data
char *getSymbol(char *cmd); // CMD = code OR data
int getCmdStart(char *cmd); // CMD = code OR data
int isCode(char *cmd);
int isData(char *cmd); // CMD = code OR data
int isStr(char *cmd); // CMD = code OR data
int isExt(char *cmd);
int getOpcode(char *op);
int getGroup(char *op);
char *getFirstOperand(char *cmd);
char *getSecondOperand(char *cmd);
