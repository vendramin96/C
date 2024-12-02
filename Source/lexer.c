#include "lexer.h"

internal_function token *
AllocateToken(token_type Type, f64 Value, token *Previous, token *Next)
{
    token *Result = 0;

    Result = PlatformAllocateMemory(sizeof(token));
    if(!Result)
    {
        return Result;
    }

    Result->Type = Type;
    Result->Value = Value;
    Result->Previous = (struct token *)Previous;
    Result->Next = (struct token *)Next;

    return Result;
}

internal_function bool
FreeToken(token *Token)
{
    bool Result = 0;

    //@TODO: Free its linked list first and itself?

    if(!PlatformFreeMemory(Token))
    {
        return Result;
    }

    Result = 1;
    return Result;
}

// TOKEN TOKEN TOKEN

internal_function void
Lexer(interpreter *Interpreter, token **Token)
{
    *Token = AllocateToken(0, 0, 0, 0);

    char *c = (!Interpreter->SourcePosition) ? (Interpreter->Source) : (Interpreter->SourcePosition);
    while(*c != '\0')
    {
        while(IsSpace(*c))
        {
            c++;
        }

        if(*c == '\0')
        {
            break;
        }

        if(IsDigit(*c))
        {
            Assert(0);
            c++;
        }
        else if(*c == '+')
        {
            Assert(0);
            c++;
        }
        else if(*c == '-')
        {
            Assert(0);
            c++;
        }
        else if(*c == '*')
        {
            Assert(0);
            c++;
        }
        else if(*c == '/')
        {
            Assert(0);
            c++;
        }
        else
        {
            Log("Invalid token: %c\n", *c);
            Interpreter->SourcePosition = c;
            return;
        }
    }

    Interpreter->SourcePosition = c;
    Assert(Interpreter->SourcePosition == (Interpreter->Source + StringLength(Interpreter->Source)));
}