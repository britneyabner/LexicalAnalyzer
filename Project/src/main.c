#include <stdio.h>
#include <string.h>

int main()
{
    return 0;
}

typedef enum
{
    Range_Seperator,
    ASSIGN,
    LCURLY,
    RCURLY,
    COMMA,
    LPAREN,
    RPAREN,
    TypeRef,
    Identifier,
    Number,
} token_type;

typedef struct
{
    token_type token_type;
    char lexeme[];
} token;

typedef enum 
{
    T,
    TA,
    TAG,
    TAGS,
} reserved_word_tags;

typedef enum
{
    B,
    BE,
    BEG,
    BEGI,
    BEGIN,
} reserved_word_begin;

typedef enum 
{
    S,
    SE,
    SEQ,
    SEQU,
    SEQUE,
    SEQUENC,
    SEQUENCE,
} reserved_word_sequence;

typedef enum
{
    I,
    IN,
    INT,
    INTE,
    INTEG,
    INTEGE,
    INTEGER,
} reserved_word_integer;

typedef enum
{
    D,
    DA,
    DAT,
    DATE,
} reserved_word_date;

typedef enum
{
    E,
    EN,
    END,
} reserved_word_end;
