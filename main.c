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
			printf("%s","error");
		
	}
	fixAddresses(IC);
	buildSymbolTable();
}
