#define MAX_AS_LINE 100


void firstLoop(FILE *);
void secondLoop();

int main(int argc, char *argv[])
{
	int i;
	FILE *f;
	
	if(argc == 1)
	\\ error 
	
	for(i = 1; i < argc; i++)
	{
		f = fopen(strcat(argv[i], ".as") , "r");
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
	int IC, DC;
	int isSymbol;
	int type; // code or data
	
	
	char asCommand[MAX_AS_LINE];
	int i, c, cmdStart;
	
	while(!feof(f))
	{
		for(i = 0, c = fgetc(f); c != '\n'; i++)
			asCommand[i] = c;
		
		type = findType(asCommand); \\ will find out ifits a code or a data command
		
		isSymbol = cmdStart = 0;
		isSymbol = isSymbol(asCommand);
		if(isSymbol)
			cmdStart = addSymbol(asCommand, (type)?(IC):(DC)) \\ cmdStart gets the start of the *real* command
		
		i = cmdStart;
		
		/*
			...
			will take care of the command
			...
		*/
		
	}
}
