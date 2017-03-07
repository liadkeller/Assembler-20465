#define MAX_AS_LINE 100

int findStart(char *);
int isCode(char *); // boolean

void firstLoop(FILE *);
void secondLoop();

int main(int argc, char *argv[])
{
	int i;
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
	char assemblyCommand[MAX_AS_LINE];
	int i, c;
	
	while(!feof(f))
	{
		for(i = 0, c = fgetc(f); c != '\n' && !feof(f); i++)
			assemblyCommand[i] = c;
		
		i = findStart(assemblyCommand)
		// to check: if its a blank/note command
		if(isCode(assemblyCommand+i))
			addCmd(assemblyCommand+i);
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
			isSymbol = 1;
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
	
