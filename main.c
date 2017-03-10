#include <stdio.h>
#include <string.h>
#include "lim.h"

int findStart(char *);
int isCode(char *); // boolean

void firstLoop(FILE *);
void secondLoop();

int main(int argc, char *argv[])
{
	int j;
	FILE *f;
	
	if(argc == 1)
	\\ error 
	
	for(j = 1; j < argc; j++)
	{
		f = fopen(strcat(argv[j], ".as") , "r");
		if(!f)
			\\error		
		firstLoop(f);
		secondLoop(/* params */);
		/* create files */
	}
	
	return 0;
}

void firstLoop(FILE *f)
{
	char assemblyCommand[assembly_line_max+1];
	int i, c;
	int IC = 100, DC = 0;
	
	while(!feof(f))
	{
		for(i = 0, c = fgetc(f); i < assembly_line_max && c != '\n' && c != EOF; i++)
			assemblyCommand[i] = c;
		assemblyCommand[i] = '\0';
		
		// to check: if its a blank/note command
		
		if(isCode(assemblyCommand))
		{
			addCmd(assemblyCommand, IC);
			IC += getWordsNum(assemblyCommand);
		}
		
		//will take care if its a data command
	}
}

int findStart(char *str)
{
	int isSymbol = 0, i = 0;
	
	while(i < strlen(str))
	{
		if(str[i] == ':')
		{
			isSymbol = TRUE;
			break;
		}
		i++;
	}
	
	if(!isSymbol)
		i = 0;
	
	while(i < strlen(str) && (str[i] == ' ' || str[i] == '\t'))
	      i++;
	      
	return i;
}
