struct symbol {
        char *label;
        int type;
        int address;
        struct symbol *next;
} *symbolTable = NULL;

addSymbol(char *label, int type, int address)
{
        struct symbol *new;
        new = (struct symbol *) malloc (sizeof (struct symbol)); // malloc - requires to free the allocation
        new->label = label;
        new->type = type;
        new->address = address;
        new->next = NULL;
        
        if(symbolTable == NULL)
                symbolTable = new;
                
        else
        {
                struct symbol *cur;
                cur = symbolTable;
                while(cur->next)
                        cur = cur->next;
                cur->next = new;
        }
}
