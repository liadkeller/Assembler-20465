#include "main.h"

int isError;
extern struct list table;
extern struct binarycode *binaryTable;
extern struct symbol *symbolTable;

int main(int argc, char *argv[])
{
	int IC, DC;
	char *fileName;
	FILE *f;
	int i, len;
	
	if(argc == 1)
	{
		fprintf(stderr, "Error - No files entered \n");
		return 0;
	}

	for(i = 1; i < argc; i++)
	{
		isError = FALSE;
		fileName = argv[i];
		len = strlen(fileName);
		
		f = fopen(strcat(fileName, ".as") , "r");
		fileName[len] = 0;
		
		if(!f)
		{
        		fprintf(stderr, "Error - File %s doesn't exist \n", fileName);
			continue;
		}
		
		firstLoop(f, &IC, &DC); /* we send IC and DC to the first loop and the first loop will put in it the right values */
		secondLoop();
		
		if(!isError)
		{
			f = fopen(strcat(fileName, ".ob") , "w");
			fileName[len] = 0;
			FILE_ERROR(object)
			createObject(f,IC,DC);
		 
			if(table.extHead)
			{
				f = fopen(strcat(fileName, ".ext") , "w"); 
				fileName[len] = 0;
				FILE_ERROR(extern)
				createExtern(f);
			}
			
			if(table.entHead)
			{
				f = fopen(strcat(fileName, ".ent") , "w");  
				fileName[len] = 0;
				FILE_ERROR(entry)
				createEntry(f);
			}
		}
		
		freeAll();
		fclose(f);
	}
	return 0;
}

void firstLoop(FILE *f, int *ICp, int *DCp)
{
	char assemblyCommand[assembly_line_max+1];
	int IC = IC_start, DC = 0;
	
	while(fgets(assemblyCommand,assembly_line_max,f))
	{			
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
  			fprintf(stderr, "Error - Illegal Command \n");
 			isError = TRUE;
 		}
	}
	fixAddresses(IC);
	
	*ICp = IC;
	*DCp = DC;
}

void freeAll() /* free all allocations made during the program */
{
	struct binarycode *binaryCur = binaryTable;
        struct ent *entCur = table.entHead;  
	struct ext *extCur = table.extHead;
	struct cmd *cmdCur = table.cmdHead; 
	struct data *dataCur = table.dataHead;

	
	while(binaryCur)
	{
		struct binarycode *temp = binaryCur;
		binaryCur = temp->next;
		free(temp);
	}
	binaryTable=NULL;
	
	while(extCur)
	{
		struct ext *temp;		
		free(extCur->symbol);
		temp = extCur;
		extCur = temp->next;
		free(temp);
	}
	table.extHead=NULL;
	
	while(entCur)
	{
		struct ent *temp;		
		free(entCur->symbol);
		temp = entCur;
		entCur = temp->next;
		free(temp);
	}
	table.entHead=NULL;
	
	while(cmdCur)
	{
		struct cmd *temp = cmdCur;
		cmdCur = temp->next;
		free(temp);
	}
	table.cmdHead=NULL;

	while(dataCur)
	{
		struct data *temp = dataCur;
		dataCur = temp->next;
		free(temp);
	}
	table.dataHead=NULL;
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
        fprintf(f, "%X   %X \n", IC - IC_start, DC);
        
        while(cur)
        {
                fprintf(f,"%X   %X \n", cur->address, binToDec(cur->binary));
                cur = cur->next;
        }
}
		
void createEntry(FILE *f)
{
        struct ent *cur = table.entHead;
        while(cur)
        {
		int ent = getEntryAddress(cur->symbol); /* entry address */
                if(ent >= 0)
                        fprintf(f,"%s   %X \n", cur->symbol, ent);
                cur = cur->next;
        }
}

void createExtern(FILE *f)
{
        struct cmd *cur = table.cmdHead;
        while(cur)
        {
                if(cur->group > 0)
                        if(cur->firstAddressing == ADDRESS)
                                if(getEntryAddress(cur->firstOperand) == 0)
                                	fprintf(f,"%s   %X \n", cur->firstOperand, cur->address+1);
        
                if(cur->group > 1)
                        if(cur->secndAddressing == ADDRESS)
                                if(getEntryAddress(cur->secndOperand) == 0)
                                        fprintf(f,"%s   %X \n", cur->secndOperand, cur->address+2);
		            
		cur = cur->next;
        }
}
