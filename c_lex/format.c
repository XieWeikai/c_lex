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
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

void format_token(int token, char *buff) {
    if(token >= AUTO && token <= WHILE)
        strcpy(buff,keywords[token - AUTO]);
    else if(token == ID){
        strcpy(buff,"ID");
    }else{
        buff[0] = token;
        buff[1] = 0;
    }
}
