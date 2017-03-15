#include <stdio.h>
#include <string.h>

#define assembly_line_max 100
#define IC_start 100
enum BOOL {FALSE , TRUE}

int isCode(char *cmd); // from analysis.c

int addCmd(char *cmd, int address);
int addData(char *cmd, int address);

void firstLoop(FILE *);
void secondLoop();
