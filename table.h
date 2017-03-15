/*
נסביר את השימוש בקיצורים הבאים

cmd - command - פקודה
מצד אחד משתמשים כדי לתאר משפטי פקודה/משפטי פעולה המורכבים ממהוראה ואופרנדים, לדוגמה
mov, inc, cmp, ...
מצד שני משתמשים כדי לתאר פקודה כלשהי, באופן כללי, הן פקודות שהן משפטי פעולה והן פקודות שהן משפטי הוראה
Code Command, Data Command, ...

data - נתונים
מצד אחד משתמשים כדי לתאר משפטי הוראה של מספרים שלמים המתחילות ב
.data
מצד שני משתמשים כדי לתאר באופן כללי משפטי הוראה המכילים נתונים, גם מספרים שלמים וגם מחרוזות, כלומר
.data and .string
*/

enum encode {NUMBER, ADDRESS, NUM2, ONE_REGISTER, TWO_REGISTER, MAIN_COMMAND}
enum encodeType {A, R, E}

void addCmdToList(struct cmd *c, struct list *t);
int addCmd(char *cmd, int address);
void addDataToList(struct cmd *c, struct list *t);
int addData(char *cmd, int address);
int addStr(char *cmd, int address);

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

struct data {
	int isFirst;
	int wordsNum;
	int address;
	int content;
	int isSymbol;
	char *symbol;
	
	struct data *next;
}

struct list {
	struct cmd *cmdHead;
	struct data *dataHead;
}
