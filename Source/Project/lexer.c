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

    //@TODO: Free its tail and then the head?

    if(!PlatformFreeMemory(Token))
    {
        return Result;
    }

    Result = 1;
    return Result;
}

internal_function token *
InsertToken(token **Head, token *Token, int Index)
{
    token *Result = 0;

    bool WasFound = 0;
    token *Current = *Head;
    token *LastToken = *Head;
    for(int I = 0; Current != 0; I++)
    {
        if(I == Index)
        {            
            Token->Previous = Current->Previous;
            Token->Next = (struct token *)Current;
            token *Previous = (token *)Current->Previous;
            Previous->Next = (struct token *)Token;
            Current->Previous = (struct token *)Token;                        
            WasFound = 1;
        }

        LastToken = Current;
        Current = (token *)Current->Next;
    }

    if(!WasFound)
    {
        LastToken->Next = (struct token *)Token;
        Token->Previous = 0;
        Token->Next = 0;        
    }
    
    return Result;
}

internal_function bool
Lexer(interpreter *Interpreter, token **Token)
{
    bool Result = 0;

    //*Token = AllocateToken(0, 0, 0, 0);
    token *Previous = 0;
    char *c = (!Interpreter->SourcePosition) ? (Interpreter->Source) : (Interpreter->SourcePosition);
    for(int Index = 1; *c != '\0'; Index++)
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
            f64 Value = 0.0; //@TODO:
            token *CurrentToken = AllocateToken(Token_Number, Value, Previous, 0);
            Previous = CurrentToken;
            InsertToken(Token, CurrentToken, Index);
            c++;
        }
        else if(*c == '+')
        {
            token *CurrentToken = AllocateToken(Token_Plus, 0, Previous, 0);
            Previous = CurrentToken;
            InsertToken(Token, CurrentToken, Index);
            c++;
        }
        else if(*c == '-')
        {
            token *CurrentToken = AllocateToken(Token_Minus, 0, Previous, 0);
            Previous = CurrentToken;
            InsertToken(Token, CurrentToken, Index);
            c++;
        }
        else if(*c == '*')
        {
            token *CurrentToken = AllocateToken(Token_Star, 0, Previous, 0);
            Previous = CurrentToken;
            InsertToken(Token, CurrentToken, Index);
            c++;
        }
        else if(*c == '/')
        {
            token *CurrentToken = AllocateToken(Token_Slash, 0, Previous, 0);
            Previous = CurrentToken;
            InsertToken(Token, CurrentToken, Index);
            c++;
        }
        else
        {
            Log("Invalid token: %c\n", *c);
            Interpreter->SourcePosition = c;
            return Result;
        }
    }

    Interpreter->SourcePosition = c;
    Assert(Interpreter->SourcePosition == (Interpreter->Source + StringLength(Interpreter->Source)));
    
    Result = 1;
    return Result;
}