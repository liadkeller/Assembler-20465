#include "utils.h"
#include "table.h"

extern int isError;

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
	if(!checkSymbol(new))
	{
		fprintf(stderr, "Error - Illegal symbol \n");
		isError = TRUE;
	}
	
	return new;
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
	{
	        while(i < strlen(cmd))
                {
		        if(cmd[i] == ':')
			{
				i++;
			        break;
			}
			i++;
                }
	}
	
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t'))
		i++;
	      
	return i;
}

int isBlankOrComment(char *cmd)
{
	int i = 0;
	
	if(cmd[i] == ';')
		return TRUE;
	
	while(i < strlen(cmd))
	{  
		if(cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n')
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
	
	while(j < strlen(cmd) && cmd[j] != '\0' && cmd[j] != ' ' && cmd[j] != '\t' && cmd[j] != '\n')
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
	int i = getCmdStart(cmd);
	if(strncmp(cmd+i, ".data", data_length) == 0)
		return TRUE;
	return FALSE;
}

int isStr(char *cmd) /* CMD = code OR data*/
{
	int i = getCmdStart(cmd);
	if(strncmp(cmd+i, ".string", string_length) == 0)
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
        /* opcode wasnt found */
	fprintf(stderr, "Error - Illegal opcode \n");
	isError = TRUE;
	return 0;
}

int getGroup(char *op)
{
        int i;
        for(i = 0; i < op_num; i++)
                if(strncmp(op, opr[i].name, strlen(opr[i].name)) == 0)
                        return opr[i].group;
        /* opcode wasnt found, error already printed */
	isError = TRUE;
	return 0;
}          

char *getFirstOperand(char *cmd)
{
	int i = getCmdStart(cmd), len = strlen(cmd);
	int start, end, size;
	char *operand;
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t') /* skip the first word (opr) until the first space*/
		i++;
	
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces until the comma*/
		i++;
	
	if(i == len || cmd[i] == '\0')
	{
		fprintf(stderr, "Error - No operands \n");
		isError = TRUE;
		return NULL;
	}
	
	start = i;
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != ',') /* skip first operand until the first space/comma*/
		i++;
	
	end = i;
	size = end-start;
	operand = (char *) malloc ((size+1) * sizeof(char)); 
        strncpy(operand, cmd+start, size);
        operand[size] = '\0';

	return operand;
}

char *getSecndOperand(char *cmd)
{
	int i = getCmdStart(cmd), len = strlen(cmd);
	int start, end, size;
	char *operand;
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t') /* skip the first word (opr) until the first space*/
		i++;
	
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces until the comma*/
		i++;
	
	if(i == len || cmd[i] == '\0')
	{
		fprintf(stderr, "Error - No operands \n");
		isError = TRUE;
		return NULL;
	}
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != ',') /* skip first operand until the first space/comma*/
		i++;
	
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces until the comma*/
		i++;
	
	if(i == len || cmd[i] == '\0')
	{
		fprintf(stderr, "Error - Only one operand \n");
		isError = TRUE;
		return NULL;
	}
	
	if(cmd[i] != ',')
	{
		fprintf(stderr, "Error - No comma \n");
		isError = TRUE;
		return NULL;
	}
	i++;
	
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces until the second operand*/
		i++;
	
	if(i == len || cmd[i] == '\0')
	{
		fprintf(stderr, "Error - Only one operand \n");
		isError = TRUE;
		return NULL;
	}
	
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
	if(operand[0] == '#')
	{
		if(strlen(operand) == 2 && operand[1] == '0');
		
		else if(atoi(operand+1) == 0)
		{
			fprintf(stderr, "Error - Operand is not a number \n"); 
			/* operand isnt "#0" but atoi returns 0
			-> the operand isnt a number */	
			isError = TRUE;
			return ADDRESS;
		}
		
		return NUMBER;
	}
	
	if(strlen(operand) == 2 && operand[0] == 'r')
	{
		if(operand[1] >= '0' && operand[1] <= '7')
			return ONE_REGISTER;
	}
	
	if(strlen(operand) == 6 && operand[0] == 'r' && operand[2] == '[' && operand[3] == 'r' && operand[5] == ']')
	{
		if(operand[1] >= '0' && operand[1] <= '7' && operand[4] >= '0' && operand[4] <= '7')
			return INDEX_REGISTER;
	}
				
	return ADDRESS;
	/* Address is the default addressing since it has the largest amount of legal operands */
}

int countWords(char *cmd)
 {
	int i = 0, num = 0, len = strlen(cmd);
 	
 	if(isStr(cmd))
 	{
 		i += string_length;
 	
 		while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skip spaces */
 			i++;
 	
 		if(cmd[i] != '"')
 			return num;
 		i++;
		
		while(i < len && cmd[i] != '"')
		{
			num++;
			i++;
		}
		
		num++; /* include 0 */
 	}
 	
 	else if(isData(cmd))
 	{
 		i += data_length;
 		
 		while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) /* skips first space */
 			      i++;
 		
 		while(i < len)
 		{
 			while(i < len && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != ',')
 				i++;
			num++;
			
			while(i < len && (cmd[i] == ' ' || cmd[i] == '\t' || cmd[i] == ','))
				i++;
 		}
	}
 	
	return num;
}

int skipSpaces(int i, char *str)
{
	for(; (str[i] != 0) && (str[i] == ' ' || str[i] == '\t' ); i++);
	return i;
}
