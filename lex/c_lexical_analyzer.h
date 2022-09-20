//
// Created by 谢卫凯 on 2022/9/18.
//

#ifndef LEX_C_LEXICAL_ANALYZER_H
#define LEX_C_LEXICAL_ANALYZER_H

#include <stdio.h>

// token 可能的取值 多为关键字
// ? : ; , ( ) { } 等直接作为token值
enum token{FOR,WHILE,CONTINUE,BREAK,IF,ELSE,SWITCH,CASE,
        DEFAULT,DO,STRUCT,UNION,ENUM,STATIC,EXTERN,TYPEDEF,RETURN,CONST,
        CHAR,SHORT,INT,FLOAT,DOUBLE,UNSIGNED,VOID,
        //以上为关键字
        OP,CHARACTER,STR,INTEGER,FLOAT_NUM,ID};

// op 取值
// * / + - = & | ^ ~ ! % 直接用对应字符ascii表示
// 下面的依次代表
// <= == != < >= > || && += -= *= /= ++ -- << >>
// 或token == CHARACTOR时 token_val为对应字符
enum token_value{LE,EQ,NE,LT,GE,GT,OR,AND,ASSIGN_ADD,ASSIGN_SUB,ASSIGN_MUL,ASSIGN_DIV,INC,DEC,LEFT_SHIFT,RIGHT_SHIFT};

// token为OP 时token_val为token_val中的一个
// token为CHARACTER时 token_val存着对应字符值
extern int token_val; // 记号值

// save integer value when token is INTEGER
extern int integer_val; // 整数

// save float number when token is FLOAT_NUM
extern double float_val; // 浮点数值

// save string when token is STR
extern char str_buff[];  // 存字符串值
extern char *str_buff_ptr;  // used together with str_buff
extern char error_msg[];

extern int line_num;     // 当前行
extern int cur_line_pos; // 当前行第几个字符
extern int char_num;     // 字符个数

extern char *yytext;
extern size_t yyleng;
extern FILE *yyin;
extern FILE *yyout;

extern int yylex();

void print_token(int token);

#endif //LEX_C_LEXICAL_ANALYZER_H
