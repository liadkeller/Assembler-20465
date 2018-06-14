#include "utils.h"
#include "table.h"
#include "binary.h"

void firstPass(FILE *f);
void secondPass();

void createObject(FILE *f);
void createEntry(FILE *f);
void createExtern(FILE *f);

#define FILE_ERROR(x)  							  \
if(!f)									  \
{								          \
        fprintf(stderr, "Error - Can't create #x file for %s", fileName); \
        continue;				  		          \
}
