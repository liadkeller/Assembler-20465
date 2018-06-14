void secondPass();
struct cmd buildOperand(char *operand, struct cmd command);

char *encode(struct cmd code);
char *intToBinaryStr(int num, int size);
int binaryStrToInt(char *bin);
void addToBinStr(char *bin, char *num, int start, int size);

int power(int a, int b);
void reverse(char *s);

void addBinary(int address, char *bin);
void copyBinary(char *dest, char *src);

struct binarycode
{
        int address;
        char binary[BINARY_WORD+1];
        struct binarycode *next;
};
