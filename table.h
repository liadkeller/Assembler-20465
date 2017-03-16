/*
נסביר את השימוש בקיצורים הבאים

cmd - command - פקודה
מצד אחד משתמשים כדי לתאר משפטי פקודה/משפטי פעולה המורכבים ממהוראה ואופרנדים, לדוגמה
mov, inc, cmp, ...
מצד שני משתמשים כדי לתאר פקודה כלשהי, באופן כללי, הן פקודות שהן משפטי פעולה והן פקודות שהן משפטי הוראה
Code Command, Data Command, ...
בכל פעם שנשתמש בפירוש השני - הכללי, נציין בהערה
// CMD = code OR data

data - נתונים
מצד אחד משתמשים כדי לתאר משפטי הוראה של מספרים שלמים המתחילות ב
.data
מצד שני משתמשים כדי לתאר באופן כללי משפטי הוראה המכילים נתונים, גם מספרים שלמים וגם מחרוזות, כלומר
.data and .string
בכל פעם שנשתמש בפירוש השני - הכללי, נציין בהערה
// DATA = data OR string

*/

enum encode {NUMBER, ADDRESS, NUM2, ONE_REGISTER, TWO_REGISTER, MAIN_COMMAND}
enum encodeType {A, R, E}
#define two_operands 2
#define register_addressing 3

void addCmdToList(struct cmd *c, struct list *t);
void addDataToList(struct cmd *d, struct list *t); // DATA = data OR string
void addExtToList(struct cmd *e, struct list *t);
int addCmd(char *cmd, int address);
int addData(char *cmd, int address);
int addStr(char *cmd, int address);
void addExt(char *cmd)

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

struct data {	// DATA = data OR string
	int isFirst;
	int wordsNum;
	int address;
	int content;
	int isSymbol;
	char *symbol;
	
	struct data *next;
}

struct ext {
	char *symbol;
	struct ext *next;
}

struct list {
	struct cmd *cmdHead;
	struct data *dataHead;
	struct ext *extHead;
}


enum symbolType {CODE , DtSt , EXT};

void addSymbol(char *label, int type, int address);

struct symbol {
        char *label;
        int type;
        int address;
        struct symbol *next;
};
