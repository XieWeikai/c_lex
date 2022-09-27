//
// Created by 谢卫凯 on 2022/9/25.
//
#include <string.h>

#include "lex.h"
#include "format.h"

static char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
        "id","str","integer","decimal","charactor","error"
};

static char *punctuators[] = {
        "ARROW","INC","DEC","LEFT_SHIFT","RIGHT_SHIFT","LE","GE","EQ","NE","AND","OR","MUL_ASSIGN","DIV_ASSIGN","MOD_ASSIGN","ADD_ASSIGN","SUB_ASSIGN",
};

static void upper(char *buff,char *s){
    char ch;
    while((ch = *s++) != 0)
        *buff++ = ch - 'a' + 'A';
    *buff = 0;
}

void format_token(int token, char *buff) {
    if(token >= AUTO && token <= ERROR)
        upper(buff,keywords[token - AUTO]);
    else if(token >= ARROW && token <= SUB_ASSIGN){
        strcpy(buff,punctuators[token - ARROW]);
    } else{
        sprintf(buff,"'%c'",token);
    }
}
