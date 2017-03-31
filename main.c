#include "main.h"  

int main(int argc, char *argv[])
{
	int i;
	char *fileName;
	FILE *f;
	
	if(argc == 1)
		;/* Error - No files */

	for(i = 1; i < argc; j++)
	{
		fileName = argv[j];
		f = fopen(strcat(fileName, ".as") , "r");
		
		if(!f)
        		/* Error - File doesn't exist */
		
		firstLoop(f);
		buildSymbolTable();
		/* secondLoop(); */
	  	/* create files */
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
			;/* error - illegal command */	
	}
	
	fixAddresses(IC);
}
