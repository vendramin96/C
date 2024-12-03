#include "parser.h"

internal_function ast *ParseExpression(token *Token);

internal_function ast *
AllocateAST(ast_type Type, ast *Left, ast *Right)
{
    ast *Result = 0;

    Result = PlatformAllocateMemory(sizeof(ast));
    if(!Result)
    {
        Log("Failed to allocate memory for ast\n");
        return Result;
    }

    Result->Type = Type;
    Result->Left = (struct ast *)Left;
    Result->Right = (struct ast *)Right;

    return Result;
}

internal_function ast *
AllocateASTNumber(f64 Value)
{
    ast *Result = 0;

    Result = AllocateAST(AST_Number, 0, 0);
    if(!Result)
    {
        return Result;
    }

    Result->Number.Value = Value;

    return Result;
}

internal_function token_type
ConvertTokenTypeToASTType(token_type Type)
{
    token_type Result = 0;

    if(Type == Token_Invalid)
    {
        Result = AST_Invalid;
    }
    else if(Type == Token_Number)
    {
        Result = AST_Number;
    }
    else if(Type == Token_Plus)
    {
        Result = AST_Add;
    }
    else if(Type == Token_Minus)
    {
        Result = AST_Subtract;
    }
    else if(Type == Token_Star)
    {
        Result = AST_Multiply;
    }
    else if(Type == Token_Slash)
    {
        Result = AST_Divide;
    }
    else if(Type == Token_Keyword)
    {
        Result = AST_Keyword;
    }
    else if(Type == Token_Operator)
    {
        Result = AST_Operator;
    }
    else if(Type == Token_Identifier)
    {
        Result = AST_Identifier;
    }
    else
    {
        Log("Invalid token_type for ast_type\n");
        return Result;
    }

    return Result;
}

internal_function ast *
ParseFactor(token *Token)
{
    ast *Result = 0;

    if(!Token)
    {
        return Result;
    }

    if(Token->Type == AST_Number)
    {
        Result = AllocateASTNumber(Token->Value);
        if(!Result)
        {
            return Result;
        }
    }
    else
    {
        Log("Invalid ast type: %s\n", ConvertTokenTypeToASTType(Token->Type));
        return Result;
    }

    return Result;
}

internal_function ast *
ParseTerm(token *Token)
{
    ast *Result = 0;

    return Result;
}

internal_function ast *
ParseExpression(token *Token)
{
    ast *Result = 0;

    return Result;
}

internal_function ast *
Parser(interpreter *Interpreter, token *Token)
{
    ast *Result = 0;

    ParseFactor(Token);

    return Result;
}