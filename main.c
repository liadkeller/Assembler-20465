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
		/*
		if there is no error

		f = fopen(strcat(fileName, ".ob") , "w");
		FILE_ERROR(object)
		createObject(f);
		 
		f = fopen(strcat(fileName, ".ext") , "w"); 
		FILE_ERROR(extern)
		createExtern(f);
		
		f = fopen(strcat(fileName, ".ent") , "w");  
		FILE_ERROR(entry)
		createEntry(f);
		*/
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
