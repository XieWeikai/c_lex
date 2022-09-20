//
// Created by 谢卫凯 on 2022/9/18.
//
#include <stdio.h>

#include "c_lexical_analyzer.h"

#define STR_MAX_LEN 1024

int token_val; // 记号值
int integer_val; // 整数
double float_val; // 浮点数值

char str_buff[STR_MAX_LEN + 1];
char *str_buff_ptr;

char error_msg[STR_MAX_LEN + 1];

int line_num = 1;     // 当前行
int cur_line_pos = 1; // 当前行第几个字符
int char_num = 0;     // 字符个数

static char *token2str[] = {"FOR", "WHILE", "CONTINUE", "BREAK", "IF", "ELSE", "SWITCH",
                     "CASE", "DEFAULT", "DO", "STRUCT", "UNION", "ENUM", "STATIC",
                     "EXTERN", "TYPEDEF", "RETURN", "CONST", "CHAR", "SHORT", "INT", "FLOAT",
                     "DOUBLE", "UNSIGNED", "VOID", "OP", "CHARACTER", "STR", "INTEGER", "FLOAT_NUM","ID"};

static char *op2str[128] = {"LE","EQ","NE","LT","GE","GT","OR","AND","ASSIGN_ADD","ASSIGN_SUB",
                            "ASSIGN_MUL","ASSIGN_DIV","INC","DEC","LEFT_SHIFT","RIGHT_SHIFT"};

static void init(){
    static int done = 0;
    if(!done){
        // * / + - = & | ^ ~ ! 直接用对应字符ascii表示
        op2str['*'] = "*";
        op2str['/'] = "/";
        op2str['+'] = "+";
        op2str['-'] = "-";
        op2str['='] = "=";
        op2str['&'] = "&";
        op2str['|'] = "|";
        op2str['^'] = "^";
        op2str['~'] = "~";
        op2str['!'] = "!";
        done = 1;
    }
}

void print_token(int token){
    init();
    printf("<");

    if(token > 30){
        printf(" %c , >\n",token);
        return;
    }

    printf("%s , ", token2str[token]);
    if(token == OP)
        printf(" %s ",op2str[token_val]);
    else if(token == CHARACTER)
        printf(" %c ",token_val);
    else if(token == INTEGER)
        printf(" %d ",integer_val);
    else if(token == STR)
        printf(" %s ",str_buff);
    else if(token == FLOAT_NUM)
        printf(" %lf ",float_val);
    else if(token == ID)// ID
        printf(" %s ",yytext);
    printf(">\n");
}
