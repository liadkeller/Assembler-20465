#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define data_length 5 /* .data*/
#define string_length 7 /* .string*/
#define extern_length 7 /* .extern*/
#define entry_length 6 /* .entry*/

#define op_name_max 4 /* maximum is 'stop' - 4 */
#define op_num 16

#define assembly_line_max 100
#define IC_start 100

enum BOOL {FALSE , TRUE};

#define two_operands 2
#define register_addressing 3
enum encode {NUMBER, ADDRESS, INDEX_REGISTER, ONE_REGISTER, TWO_REGISTER, MAIN_COMMAND};
enum encodeType {A, E, R};
enum regType {SOURCE, DEST};
enum operandNumber {CMD, FIRST, LAST}; /* FIRST - 1/2, LAST - 1/1 or 2/2 */
enum symbolType {CODE , DtSt , EXT};

#define binary_word 15
