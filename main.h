#include <stdio.h>
#include <string.h>

#define assembly_line_max 100
#define IC_start 100
enum BOOL {FALSE , TRUE}

int isCode(char *cmd); // from analysis.c

void firstLoop(FILE *);
void secondLoop();
