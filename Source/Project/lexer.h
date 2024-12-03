#ifndef LEXER_H
#define LEXER_H

#include "Header/definitions.h"
#include "Library/platform.h"

typedef enum
{
    Token_Invalid,
    Token_Number, //@NOTE: Only f64
    Token_Plus,
    Token_Minus,
    Token_Star,
    Token_Slash,
    //@TODO: Bellow is not yet handled
    Token_Keyword,
    Token_Operator,
    Token_Identifier,
} token_type;

typedef struct
{
    token_type Type;
    f64 Value;
    struct token *Previous;
    struct token *Next;
} token;

#endif