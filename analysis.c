#include "utils.h"
#include "table.h"

extern struct symbol *symbolTable;
struct opcode {
        char *name;
        int group;
} opr[] = {{"mov", 2},
           {"cmp", 2},
           {"add", 2},
           {"sub", 2},
           {"not", 1},
           {"clr", 1},
           {"lea", 2},
           {"inc", 1},
           {"dec", 1},
           {"jmp", 1},
           {"bne", 1},
           {"red", 1},
           {"prn", 1},
           {"jsr", 1},
           {"rts", 0},
           {"stop", 0}};
     

int isSymbol(char *cmd) /* CMD = code OR data*/
{
        int i = 0;
        while(i < strlen(cmd))
	{
		if(cmd[i] == ':')
			return TRUE;
		i++;
	}
        return FALSE;
}

char *getSymbol(char *cmd) /* CMD = code OR data*/
{
        int i = 0;
        char *new;
        
        while(i < strlen(cmd))
        {
		if(cmd[i] == ':')
			break;
		i++;
        }
        
        new = (char *) malloc ((i+1)*sizeof(char));
        strncpy(new, cmd, i); /* copy i chars from 0 to i-1 */
        new[i] = '\0'; /* the ':' sign*/	
				
	/* !!! to free the allocation */
	if(checkSymbol(new))
      		return new;
	else
		/* error */return new;/*Temporary! */
}

int checkSymbol(char *cmd)
{
	int i = 0;
	int length = strlen(cmd);
	if(!length)
		return FALSE;
	
	if(cmd[i] < 'A' || cmd[i] > 'z' || (cmd[i] < 'a' && cmd[i] > 'Z')) /* first char isn't letter*/
		return FALSE;
	
	if(cmd[i] == 'r' && length == 2)/* the symbol have a name of register*/ 
		for(i = 0; i < 8; i++)
			if((cmd[1] - '0') == i)
				return FALSE;
	
	for(i = 1; i < length; i++)
		if(cmd[i] < '0' || (cmd[i] > '9' && cmd[i] < 'A') || ((cmd[i] < 'a' && cmd[i] > 'Z')) || cmd[i] > 'z') /* cmd[i] isn't a char or a letter*/
			return FALSE;
	
	for(i = 0; i < op_num; i++)
	    if(strcmp(opr[i].name, cmd) == 0)
			return FALSE;
	return TRUE;
	
}
int getCmdStart(char *cmd) /* CMD = code OR data*/
{
        int i = 0;
	
        if(isSymbol(cmd))
	        while(i < strlen(cmd))
                {
		        if(cmd[i] == ':')
			        break;
		        i++;
                }
	i = skipSpaces(i,cmd);
	      
	return i;
}
int isBlankOrComment(char *cmd)
{
	int i=0;
	if(cmd[i]==';')
		return TRUE;
	while(i<strlen(cmd))
	{  
		if(cmd[i]!=' ' && cmd[i]!='\t' && cmd[i]!='\n')
			return FALSE;
	    i++;
 	}
	return TRUE;
}

int isCode(char *cmd)
{
	char op[op_name_max+1];
	int i = getCmdStart(cmd);
	int j = i;
	
	while(j < strlen(cmd) && cmd[j] != '\0' cmd[j] != ' ' && cmd[j] != '\t' && cmd[j] != '\t')
		j++;
	
	if(j-i > op_name_max) /* the first word (op) is bigger than an op */
		return FALSE;
	
	strncpy(op, cmd+i, j-i);
	op[j-i] = '\0';
	
	for(i = 0; i < op_num; i++)
                if(strncmp(op, opr[i].name, strlen(opr[i].name)) == 0)
			return TRUE;
	return FALSE;
}

int isData(char *cmd) /* CMD = code OR data*/
{
	if(strncmp(cmd, ".data", data_length) == 0)
		return TRUE;
	return FALSE;
}

int isStr(char *cmd) /* CMD = code OR data*/
{
	if(strncmp(cmd, ".string", string_length) == 0)
		return TRUE;
	return FALSE;
}

int isExt(char *cmd)
{
	if(strncmp(cmd, ".extern", extern_length) == 0)
		return TRUE;
	return FALSE;
}
int isEnt(char *cmd)
{
	if(strncmp(cmd, ".entry", entry_length) == 0)
		return TRUE;
	return FALSE;
}

int getOpcode(char *op)
{
        int i;
        for(i = 0; i < op_num; i++)
                if(strncmp(op, opr[i].name, strlen(opr[i].name)) == 0)
                        return i;
        /* error*/
        return -1;
}

int getGroup(char *op)
{
        int i;
        for(i = 0; i < op_num; i++)
                if(strncmp(op, opr[i].name, strlen(opr[i].name)) == 0)
                        return opr[i].group;
        /* error*/
        return -1;
}          

char *getFirstOperand(char *cmd)
{
	int i = getCmdStart(cmd), len = strlen(cmd);
	int start, end, size;
	char *operand;
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t') /* skip the first word (opr) until the first space*/
		i++;
	
	i=skipSpaces(i,cmd);
	
	if(i == len || cmd[i] == '\0')
		/* error - cmd is too short*/
	
	start = i;
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != ',') /* skip first operand until the first space/comma*/
		i++;
	
	end = i-1;
	size = end-start+1;
	operand = (char *) malloc ((size+1)*sizeof(char));
        strncpy(operand, cmd+start, size);
        operand[size] = '\0';
	/* !!! to free the allocation*/
	return operand;
}

char *getSecndOperand(char *cmd)
{
	int i = getCmdStart(cmd), len = strlen(cmd);
	int start, end, size;
	char *operand;
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t') /* skip the first word (opr) until the first space*/
		i++;
	
	i=skipSpaces(i,cmd);
	
	if(i == len || cmd[i] == '\0')
		/* error - cmd is too short*/;
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != ',') /* skip first operand until the first space/comma*/
		i++;
	
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces until the comma*/
		i++;
	
	if(i == len || cmd[i] == '\0')
		/* error - cmd is too short*/;
	
	if(cmd[i] != ',')
		/* error - no comma*/;
	i++;
	
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces until the second operand*/
		i++;
	
	if(i == len || cmd[i] == '\0')
		/* error - cmd is too short*/;
	
	start = i;
	end = len - 1;
	size = end-start+1;
	operand = (char *) malloc ((size+1)*sizeof(char));
        strncpy(operand, cmd+start, size);
        operand[size] = '\0';
	/* !!! to free the allocation*/
	return operand;
}

int getAddressing(char *operand)
{
	struct symbol *symbolCur = symbolTable;
	if(operand[0] == '#')
	{
		if(strlen(operand) == 2 && operand[1] == '0');
		
		else if(atoi(operand+1) == 0)
			fprintf(stderr, "Error - Operand is not a number"); 
			/* operand isnt "#0" but atoi returns 0
			-> the operand isnt a number */		
		
		return NUMBER;
	}
	
	
	if(strlen(operand) == 2 && operand[0] == 'r')
	{
		if(operand[1] >= '0' && operand[1] <= '7')
			return ONE_REGISTER;
	}
	
	if(strlen(operand) == 6 && operand[0] == 'r' && operand[2] == '[' && operand[3] == 'r' && operand[5] == ']')
	{
		if(!(operand[1] >= '0' && operand[1] <= '7' && operand[4] >= '0' && operand[4] <= '7'))
			fprintf(stderr, "Error - Operand is not an exist lable/illegal operand"); 
		return INDEX_REGISTER;
	}
				
	while(symbolCur && symbolCur->next)
	{
		if(strcmp(symbolCur->name, operand))
			return ADDRESS;
					
		symbolCur = symbolCur->next;
	}
	/* label wasnt found */
	fprintf(stderr, "Error - Operand is not an exist lable/illegal operand"); 
	return ADDRESS;
}

int countWords(char *cmd)
 {
	int i = 0, num = 0, len = strlen(cmd);
 	
 	if(isStr(cmd))
 	{
 		i += string_length;
 	
 		while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces*/
 			i++;
 	
 		if(cmd[i] != '"')
 			return num;
 		i++;
 		
 		while(cmd[i] != '\0')
 			num++;
 	}
 	
 	if(isData(cmd))
 	{
 		i += data_length;
 		
 		while(i < len && (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == ','))
 			      i++;
 		/* skips first space*/
 		
 		while(i < len && cmd[i] != '\0')
 		{
 			while(i < len && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != ',')
 			      i++;
 			num++;
 		}
	}
 		return num;
}

int skipSpaces(int i, char *str)
{
	for(; (str[i] != 0) && (str[i] == ' ' || str[i] == '\t' ); i++);
	return i;
}
