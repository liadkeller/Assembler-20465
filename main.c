#include "main.h"

int isError;
extern struct list table;
extern struct binarycode *binaryTable;
extern struct symbol *symbolTable;

int main(int argc, char *argv[])
{
	int i,IC,DC,len;
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
		len=strlen(fileName);
		
		f = fopen(strcat(fileName, ".as") , "r");
		fileName[len]=0;
		
		if(!f)
		{
        		fprintf(stderr, "\n Error - File %s doesn't exist", fileName);
			continue;
		}
		
		firstLoop(f,&IC,&DC);
		buildSymbolTable();
		secondLoop();
		
		if(!isError)
		{
			f = fopen(strcat(fileName, ".ob") , "w");
			fileName[len]=0;
			FILE_ERROR(object)
			createObject(f,IC,DC);
		 
			if(table.extHead)
			{
				f = fopen(strcat(fileName, ".ext") , "w"); 
				fileName[len]=0;
				FILE_ERROR(extern)
				createExtern(f);
			}
			
			if(table.entHead)
			{
				f = fopen(strcat(fileName, ".ent") , "w");  
				fileName[len]=0;
				FILE_ERROR(entry)
				createEntry(f);
			}
		}
		fclose(f);
	}
	
	return 0;
}

void firstLoop(FILE *f,int *ICp,int *DCp)
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
	*ICp=IC;
	*DCp=DC;
}

void deleteEnter(char *assemblyCommand)
{
	int i = 0;
	while(assemblyCommand[i] != '\0')
		i++;
	if(assemblyCommand[--i] == '\n')
		assemblyCommand[i] = '\0';

}

void createObject(FILE *f,int IC, int DC)
{
	struct binarycode *cur = binaryTable;
        fprintf(f, "%X   %X\n", IC - IC_start, DC);
        
        while(cur)
        {
                fprintf(f,"%X   %s \n", cur->address, cur->binary);
                cur = cur->next;
        }
}
		
void createEntry(FILE *f)
{
        struct ent *cur = table.entHead;
        while(cur)
        {
                struct symbol *symbolCur = symbolTable;
                while(symbolCur)
                {
                    if(strcmp(symbolCur->name, cur->symbol) == 0)
                        fprintf(f,"%s   %X \n ", symbolCur->name, symbolCur->address);
                        symbolCur = symbolCur->next;
                }
                cur = cur->next;
        }
}
		
void createExtern(FILE *f)
{
        struct cmd *cur = table.cmdHead;
        while(cur)
        {
                if(cur->group > 0)
                {
                        if(cur->firstAddressing == ADDRESS)
                        {
                                struct symbol *symbolCur = symbolTable;
                                while(symbolCur)
                                {
                                        if(strcmp(symbolCur->name, cur->firstOperand) == 0)
                                                if(symbolCur->type == EXT)
                                                        fprintf(f,"%s   %X \n", symbolCur->name, cur->address);
                                        symbolCur = symbolCur->next;
                                }
                        }
                }
        
                if(cur->group > 1)
                {
                        if(cur->secndAddressing == ADDRESS)
                        {
                                struct symbol *symbolCur = symbolTable;
                                while(symbolCur)
                                {
                                        if(strcmp(symbolCur->name, cur->secndOperand) == 0)
                                                if(symbolCur->type == EXT)
                                                        fprintf(f,"%s   %X \n", symbolCur->name, cur->address);
                                        symbolCur = symbolCur->next;
                                }
                        }
                }  
                cur = cur->next;
        }
}
