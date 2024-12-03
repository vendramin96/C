#ifndef PARSER_H
#define PARSER_H

#include "Header/definitions.h"
#include "Library/platform.h"
#include "lexer.h"

typedef enum
{
    AST_Invalid,
    AST_Number,
    AST_Add,
    AST_Subtract,
    AST_Multiply,
    AST_Divide,
    AST_Keyword,
    AST_Operator,
    AST_Identifier,
} ast_type;

struct ast;

typedef struct
{
    f64 Value;
} number;

typedef struct
{
    struct ast *LeftValue;
    struct ast *RightValue;
    char Operation;
} binary_operator;

typedef struct
{
    ast_type Type;
    struct ast *Left;
    struct ast *Right;
    
    union
    {
        number Number;
        binary_operator BinaryOperator;
    };    
} ast;

#endif