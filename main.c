#include "main.h"

int isError;
int main(int argc, char *argv[])
{
	int i;
	char *fileName;
	FILE *f;
	
	if(argc == 1)
	{
		fprintf(stderr, "\n Error - No files entered");
		return 0;
	}

	for(i = 1; i < argc; i++)
	{
		isError = FALSE;
		fileName = argv[i];
		f = fopen(strcat(fileName, ".as") , "r");
		
		if(!f)
		{
        		fprintf(stderr, "\n Error - File %s doesn't exist", fileName);
			continue;
		}
		
		firstLoop(f);
		buildSymbolTable();
		secondLoop();
		
		if(!isEroor)
		{
			f = fopen(strcat(fileName, ".ob") , "w");
			FILE_ERROR(object)
			createObject(f);
		 
			if(table.extHead)
			{
				f = fopen(strcat(fileName, ".ext") , "w"); 
				FILE_ERROR(extern)
				createExtern(f);
			}
			
			if(table.entHead)
			{
				f = fopen(strcat(fileName, ".ent") , "w");  
				FILE_ERROR(entry)
				createEntry(f);
			}
		}
		fclose(f);
	}
	
	return 0;
}

void firstLoop(FILE *f)
{
	char assemblyCommand[assembly_line_max+1];
	int IC = IC_start, DC = 0;
	int i=1; /* temp index */
	while(fgets(assemblyCommand,assembly_line_max,f))
	{	
		printf("\n line: %d",i); /* temp */
		i++;	/* temp*/
		
		deleteEnter(assemblyCommand);
		
		if(isBlankOrComment(assemblyCommand))
			continue;

		else if(isExt(assemblyCommand))
		  	addExt(assemblyCommand);
		
		else if(isEnt(assemblyCommand))
		  	addEnt(assemblyCommand);
		
		else if(isCode(assemblyCommand))
			IC += addCmd(assemblyCommand, IC);	

		else if(isData(assemblyCommand))
		  	DC += addData(assemblyCommand, DC);
		
		else if(isStr(assemblyCommand))
		  	DC += addStr(assemblyCommand,DC);
		
		else
		{
  			fprintf(stderr, "\n Error - Illegal Command");
 			isError = TRUE;
 		}
	}
	printf("\n %d,%d \n",IC,DC); /* !!! TEMP ONLY */
	
	fixAddresses(IC);
}

void deleteEnter(char *assemblyCommand)
{
	int i = 0;
	while(assemblyCommand[i] != '\0')
		i++;
	if(assemblyCommand[--i] == '\n')
		assemblyCommand[i] = '\0';

}

void createObject(FILE *f)
{
        fprintf(f, "%X   %X\n, IC - IC_start, DC);
        
        struct binarycode *cur = binaryTable;
        while(cur && cur->next)
        {
                fprintf("%X   %s", cur->address, cur->binary);
                cur = cur->next;
        }
}
		
void createEntry(FILE *f)
{
        struct ent cur = table.entHead;
        while(ent && ent->next)
        {
                struct symbol symbolCur = symbolTable;
                while(symbolCur && symbolCur->next)
                {
                        if(strcmp(symbolCur->name, cur->symbol) == 0)
                                fprintf("%s   %X", cur->symbol, symbolCur->address);
                        symbolCur = symbolCur->next;
                }
                ent = ent->next;
        }
}
		
void createExtern(FILE *f)
{
        struct cmd cur = table.cmdHead;
        while(cur && cur->next)
        {
                if(cur->group > 0)
                {
                        if(cur->firstAddressing == ADDRESS)
                        {
                                struct symbol symbolCur = symbolTable;
                                while(symbolCur && symbolCur->next)
                                {
                                        if(strcmp(symbolCur->name, cur->firstOperand) == 0)
                                                if(symbolCur->type == EXT)
                                                        fprintf("%s   %X", symbolCur->name, cur->address);
                                        symbolCur = symbolCur->next;
                                }
                        }
                }
        
                if(cur->group > 1)
                {
                        if(cur->secndAddressing == ADDRESS)
                        {
                                struct symbol symbolCur = symbolTable;
                                while(symbolCur && symbolCur->next)
                                {
                                        if(strcmp(symbolCur->name, cur->secndOperand) == 0)
                                                if(symbolCur->type == EXT)
                                                        fprintf("%s   %X", symbolCur->name, cur->address);
                                        symbolCur = symbolCur->next;
                                }
                        }
                }  
                cur = cur->next;
        }
}
