#include "main.h"

int main(int argc, char *argv[])
{
	int i;
	char *fileName;
	FILE *f;
	
	if(argc == 1)
	{
		fprintf(stderr, "Error - No files entered");
		return 0;
	}

	for(i = 1; i < argc; i++)
	{	
		fileName = argv[i];
		f = fopen(strcat(fileName, ".as") , "r");
		
		if(!f)
		{
        		fprintf(stderr, "Error - File %s doesn't exist", fileName);
			continue;
		}
		
		firstLoop(f);
		buildSymbolTable();
		secondLoop();
		
		/* !!! if there is no error */
		f = fopen(strcat(fileName, ".ob") , "w");
		FILE_ERROR(object)
		createObject(f);
		 
		f = fopen(strcat(fileName, ".ext") , "w"); 
		FILE_ERROR(extern)
		createExtern(f);
		
		f = fopen(strcat(fileName, ".ent") , "w");  
		FILE_ERROR(entry)
		createEntry(f);
		 
		fclose(f);
	}
	
	return 0;
}

void firstLoop(FILE *f)
{
	char assemblyCommand[assembly_line_max+1];
	int IC = IC_start, DC = 0;
	
	while(fgets(assemblyCommand,assembly_line_max,f))
	{		
		if(isBlankOrComment(assemblyCommand)||isEnt(assemblyCommand))
			continue;

		else if(isCode(assemblyCommand))
			IC += addCmd(assemblyCommand, IC);	

		else if(isData(assemblyCommand))
		  	DC += addData(assemblyCommand, DC);
		
		else if(isStr(assemblyCommand))
		  	DC += addStr(assemblyCommand,DC);
	
		else if(isExt(assemblyCommand))
		  	addExt(assemblyCommand);
		
		else
			fprintf(stderr, "Error - Illegal Command");
	}
	
	fixAddresses(IC);
}
