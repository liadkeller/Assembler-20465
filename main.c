#include "main.h"

int main(int argc, char *argv[])
{
	int j;
	FILE *f;
	char* fileName;
	if(argc == 1)
    {
       	printf("No files to compile\n");
        return 0;
    }

	for(j = 1; j < argc; j++)
	{
		fileName=argv[j];
		f = fopen(strcat(fileName, ".as") , "r");
		if(!f)
        {
            printf("File %s doesn't exist!\n",fileName);
            return 0;
        }	
		firstLoop(f);
		/*	secondLoop( params );  */
	  /*  create files */
	}
	
	return 0;
}

void firstLoop(FILE *f)
{
	char assemblyCommand[assembly_line_max+1];
	int i, c;
	int IC = IC_start, DC = 0;
	
	while(!feof(f))
	{
		for(i = 0, c = fgetc(f); i < assembly_line_max && c != '\n' && c != EOF; i++)
			assemblyCommand[i] = c;
		assemblyCommand[i] = '\0';
		
		if(isBlankOrComment(assemblyCommand))
			continue;
		
		if(isCode(assemblyCommand))
			IC += addCmd(assemblyCommand, IC);
		
		if(isData(assemblyCommand))
		 	DC += addData(assemblyCommand, DC);
		
		if(isStr(assemblyCommand))
		 	DC += addStr(assemblyCommand,DC);
		
		/*	if(isExt(assemblyCommand))
		  		addExt(assemblyCommand);
		
			if isEntry, do nothing  */
	}
	
	fixAddresses(IC);
	buildSymbolTable();
}
