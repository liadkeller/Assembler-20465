#include "utils.h"
#include "binary.h"/*+table.h */

void firstLoop(FILE *f,int *ICp,int *DCp);
void secondLoop();

void createObject(FILE *f,int IC, int DC);
void createEntry(FILE *f);
void createExtern(FILE *f);

void freeAll();
void deleteEnter(char *assemblyCommand);

#define FILE_ERROR(x)  							  \
if(!f)									  \
{								          \
        fprintf(stderr, "Error - Can't create #x file for %s", fileName); \
        continue;				  		          \
}
