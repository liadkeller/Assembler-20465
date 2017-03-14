#include "utils.h"

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
           {"sto", 0}};
          
int getOpcode(char *op)
{
        int i;
        for(i = 0; i < op_num; i++)
                if(strncmp(op, opr[i].name, op_name_size) == 0)
                        return i;
        // error
        return -1;
}

int getGroup(char *op)
{
        int i;
        for(i = 0; i < op_num; i++)
                if(strncmp(op, opr[i].name, op_name_size3) == 0)
                        return op[i].group;
        // error
        return -1;
}          

int isSymbol(char *cmd)
{
        int i = 0;
        while(i < strlen(cmd))
	{
		if(cmd[i] == ':')
		{
			return TRUE;
		}
		i++;
	}
        return FALSE;
}

char *getSymbol(char *cmd)
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
        strncpy(new, cmd, i) // copy i chars from 0 to i-1
        new[i] = '\0';
        
	// !!! to check the symbol and report errors
	// !!! to free the allocation
	
        return new;
}

int getCmdStart(char *cmd)
{
        int i = 0;
	
        if(isSymbol(cmd))
	        while(i < strlen(cmd))
                {
		        if(cmd[i] == ':')
			        break;
		        i++;
                }
	
	while(i < strlen(cmd) && (cmd[i] == ' ' || cmd[i] == '\t'))
	      i++;
	      
	return i;
}

int isCode(char *cmd)
{
	char op[op_name_size+1];
	int i = getCmdStart(cmd);
	
	strncpy(op, cmd+i, op_name_size);
	op[op_name_size] = '\0';
	
	for(i = 0; i < op_num; i++)
                if(strncmp(op, opr[i].name, op_name_size) == 0)
			return TRUE;
	return FALSE;
}

char *getFirstOperand(char *cmd)
{
	int i = getCmdStart(cmd), len = strlen(cmd);
	int start, end, size;
	char *operand;
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t') // skip the first word (opr) until the first space
		i++;
	
	while(i < len && cmd[i] == ' ' || cmd[i] == '\t') // skip spaces until the second word, first operand
		i++;
	
	if(i == len || cmd[i] == '\0')
		// error - cmd is too short
	
	start = i;
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != ',') // skip first operand until the first space/comma
		i++;
	
	end = i-1;
	size = end-start+1;
	operand = (char *) malloc ((size+1)*sizeof(char));
        strncpy(operand, cmd+start, size)
        operand[size] = '\0';
	// !!! to free the allocation
	return operand;
}

char *getSecondOperand(char *cmd)
{
	int i = getCmdStart(cmd), len = strlen(cmd);
	int start, end, size;
	char *operand;
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t') // skip the first word (opr) until the first space
		i++;
	
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) // skip spaces until the second word, first operand
		i++;
	
	if(i == len || cmd[i] == '\0')
		// error - cmd is too short
	
	while(i < len && cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != ',') // skip first operand until the first space/comma
		i++;
	
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) // skip spaces until the comma
		i++;
	
	if(i == len || cmd[i] == '\0')
		// error - cmd is too short
	
	if(cmd[i] != ',')
		// error - no comma
	i++;
	
	while(i < len && (cmd[i] == ' ' || cmd[i] == '\t')) // skip spaces until the second operand
		i++;
	
	if(i == len || cmd[i] == '\0')
		// error - cmd is too short
	
	start = i;
	end = len - 1;
	size = end-start+1;
	operand = (char *) malloc ((size+1)*sizeof(char));
        strncpy(operand, cmd+start, size)
        operand[size] = '\0';
	// !!! to free the allocation
	return operand;
}
