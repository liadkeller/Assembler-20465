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
	char asCommand[MAX_AS_LINE];
	int i, c;
	
	while(!feof(f))
	{
		for(i = 0, c = fgetc(f); c != '\n'; i++)
		{
			
		}
	}
}
