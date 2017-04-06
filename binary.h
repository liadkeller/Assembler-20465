
void secondLoop();
void buildOperand(struct cmd *c, char *operand);

char *encode(struct cmd *code, int encode);
char *intToBinary(int num, int size);
void addBin(char *bin, char *num, int start, int size);
char *binaryToHexa(char *bin);
int getHexa(int num);

void addBinary(int address, char *bin);

struct binarycode
{
        int address;
        char binary[BINARY_WORD];
        struct binarycode *next;
}
