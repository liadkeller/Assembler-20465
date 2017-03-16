struct symbol {
        char *label;
        int address;
        struct symbol *next;
} *symbolTable = NULL;

addSymbol(char *label, int address)
{
        struct symbol *new;
        new = (struct symbol *) malloc (sizeof (struct symbol));
        new->label = label;
        new->address = address;
        new->next = NULL;
        
        if(symbolTable == NULL)
                symbolTable = new;
                
        else
        {
                struct symbol cur = symbolTable;
                while(cur->next)
                        cur = cur->next;
                cur->next = new;
        }
}
