#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum op {ZERO_OPERANDS, ONE_OPERAND, TWO_OPERANDS};
#define OPERATIONS_NUMBER 16

#define ASSEMBLY_LINE_MAX 100
#define IC_START 100

enum BOOL {FALSE , TRUE};

enum encode {NUMBER, ADDRESS, INDEX_REGISTER, REGISTER, TWO_REGISTERS, MAIN_COMMAND};
enum encodeType {A, E, R};
enum operandType {SRC, DEST};

#define BINARY_WORD 15
#define LINE_WORD 30
#define MAX_WORDS 60
/* 60 words is the maximum words number, means .data can have maximum of 29 numbers, 30 chars is the maximum characters number for one word. */

#define ARRAY_SIZE 1000
#define LABELS_ARRAY_SIZE 100 /* extArray, entArray */


#define LABEL_INDEX 0
#define OPERTATION_NAME_INDEX hasSymbol + 0
#define FIRST_OPERAND_INDEX hasSymbol + 1
#define SECOND_OPERAND_INDEX hasSymbol + 3
#define COMMA_INDEX hasSymbol + 2
#define CONTENT_INDEX hasSymbol + 1

#define FIRST_NUM_INDEX 1 /* We start from 1 since we skip on '.data' */
#define FIRST_CHAR_INDEX 1 /* We start from 1 since we skip on the quotes */
#define LAST_CHAR_INDEX stringLen - 2 /* notice it depends on the var 'stringLen' */
#define FIRST_QUOTE_INDEX 0
#define LAST_QUOTE_INDEX stringLen - 1

#define ZERO_OP_WORDS_NUM 1
#define ONE_OP_WORDS_NUM 2
#define TWO_OP_WORDS_NUM 4
#define STRING_CMD_WORDS_NUM 2 /* The .string command words number */

#define REG_VALUE_INDEX 1 /* e.g. r3 */

#define ERROR 0
#define EXTERN_SYMBOL_ADDRESS 0
