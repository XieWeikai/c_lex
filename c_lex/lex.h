//
// Created by 谢卫凯 on 2022/9/23.
//

#ifndef C_LEX_LEX_H
#define C_LEX_LEX_H

#include <stdio.h>

#include "trie.h"

#define MAX_TOKEN_TEXT_LEN 128
#define LEX_MAX_STR_LEN 128

#define ID_TYPE 0
#define STRING_TYPE 1
#define INT_TYPE 2
#define DECIMAL_TYPE 3
#define CHAR_TYPE 4

// stored in symbol table
typedef struct item{
    int token;   // exact keyword or ID

    // other field not defined yet
}item;

typedef struct token_value {
    int type;                          // one of macros defined above
    union {
        int ch;                        // CHAR_TYPE
        int integer;                   // INT_TYPE
        double decimal;                // DECIMAL_TYPE
        char str[LEX_MAX_STR_LEN + 1]; // STRING_TYPE
        void *data;                    // ID_TYPE   this is the entry in symbol table
    } value;
} token_value;

typedef struct lex {
    FILE *lex_in; // input file
    int token;    // current token
    token_value val; // token value
    dict symbols;    // symbol table

    int cur_ch;   // current char
    char text[MAX_TOKEN_TEXT_LEN + 1]; // text corresponding with the token
    char *text_ptr; // point to the next position to write in text

    int line; // line number
    int pos;  // position in current line
    int num_ch; // number of chars read

    char err_msg[LEX_MAX_STR_LEN + 1]; // only used when token is ERROR
} lex;

typedef enum token{

    // [ ] { } ( ) . & * + - ~  ! / % < > ^ | ? : ; = ,
    // 这些只有一个符号的直接当成token
    // -> ++ -- << <= >= == != && ||  *= /= %= += -=

    // >= 256 防止出现ascii 码中的数字
    ARROR=256,INC,DEC,LEFT_SHIFT,LE,GE,EQ,NE,AND,OR,MUL_ASSIGN,DIV_ASSIGN,MOD_ASSIGN,ADD_ASSIGN,SUB_ASSIGN,
    // 上面的都是punctuator
    AUTO,BREAK,CASE,CHAR,CONST,CONTINUE,DEFAULT,DO,
    DOUBLE,ELSE,ENUM,EXTERN,FLOAT,FOR,GOTO,IF,
    INT,LONG,REGISTER,RETURN,SHORT,SIGNED,SIZEOF,STATIC,
    STRUCT,SWITCH,TYPEDEF,UNION,UNSIGNED,VOID,VOLATILE,WHILE,
    // 以上为关键字
    ID, // identifier
    STR,INTEGER,DECIMAL,CHARACTER,
    // error token
    ERROR,
}token;

int init_lex(char *file_name,lex *l);

int next(lex *l);

#endif //C_LEX_LEX_H
