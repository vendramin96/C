#include "Header/definitions.h"
#include "Library/platform.h"

typedef struct
{
    char Source[Kilobytes(64)];
    char *SourcePosition;
} interpreter;

#include "Code/string.c"
#include "lexer.c"

int main(int ArgumentCount, char **Argument)
{
    interpreter Interpreter = {0};
    file File = {0};
    PlatformReadFile(&File, "lexer1.txt");//@TODO:
    CopyStringToBuffer(Interpreter.Source, sizeof(Interpreter.Source), File.Memory, File.Size, File.Size);

    token *Token = 0;
    Lexer(&Interpreter, &Token);

    while(Token != 0)
    {
        Print("Token Type = %d\n", Token->Type);

        if(Token->Type == Token_Number)
        {
            Print("Token Value: %d\n", Token->Value);
        }

        Token = (token *)Token->Next;
    }
    
    return 0;
}