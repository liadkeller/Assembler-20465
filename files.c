void createObject(FILE *f)
{
        fprintf(f, "%X   %X\n, IC - IC_start, DC);
        
        struct binarycode *cur = binaryTable;
        while(cur && cur->next)
        {
                fprintf("%X   %s", cur->address, cur->binary);
                cur = cur->next;
        }
}

void createEntry(FILE *f)
{
        struct ent cur = table.entHead;
        while(ent && ent->next)
        {
                struct symbol symbolCur = symbolTable;
                while(symbolCur && symbolCur->next)
                {
                        if(strcmp(symbolCur->name, cur->symbol) == 0)
                                fprintf("%s   %X", cur->symbol, symbolCur->address);
                        symbolCur = symbolCur->next;
                }
                ent = ent->next;
        }
        
}

void createExtern(FILE *f)
{
        struct cmd cur = table.cmdHead;
        while(cur && cur->next)
        {
                if(cur->group > 0)
                {
                        if(cur->firstAddressing == ADDRESS)
                        {
                                struct symbol symbolCur = symbolTable;
                                while(symbolCur && symbolCur->next)
                                {
                                        if(strcmp(symbolCur->name, cur->firstOperand) == 0)
                                                if(symbolCur->type == EXT)
                                                        fprintf("%s   %X", symbolCur->name, cur->address);
                                        symbolCur = symbolCur->next;
                                }
                        }
                }
        
                if(cur->group > 1)
                {
                        if(cur->secndAddressing == ADDRESS)
                        {
                                struct symbol symbolCur = symbolTable;
                                while(symbolCur && symbolCur->next)
                                {
                                        if(strcmp(symbolCur->name, cur->secndOperand) == 0)
                                                if(symbolCur->type == EXT)
                                                        fprintf("%s   %X", symbolCur->name, cur->address);
                                        symbolCur = symbolCur->next;
                                }
                        }
                }  
                cur = cur->next;
        }
}
