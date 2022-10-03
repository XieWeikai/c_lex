%{

#include <stdio.h>
#include <stdlib.h>
#include "c_lexical_analyzer.h"

#define COUNT() do{cur_line_pos += yyleng;char_num += yyleng;}while(0)
%}
NUM [0-9]
POSITIVE_NUM [1-9]
LETTER   [a-zA-Z]
PUNCT   "?"|":"|","|";"|"."|"("|")"|"{"|"}"

%x comment
%x character
%x str
%%
"/*" {BEGIN(comment);COUNT();}
<comment>{
    "\n" {line_num++;COUNT();cur_line_pos = 1;}
    "*/" {COUNT();BEGIN(INITIAL);}
    . /*do nothing*/
}
"//".* COUNT();

    /*科学计数法暂时不做考虑*/
{NUM}+"."{NUM}+[a-df-zA-DF-Z]+ {COUNT();sprintf(error_msg,"bad float number!");return -2;}
{NUM}+"."{NUM}+ {COUNT();float_val = atof(yytext);return FLOAT_NUM;}

{POSITIVE_NUM}{NUM}*|{NUM}    {COUNT();integer_val = atoi(yytext);return INTEGER;}
0x[0-9a-fA-F]+                {COUNT();integer_val = strtol(yytext,NULL,0);return INTEGER;}
0[0-7]+                       {COUNT();integer_val = strtol(yytext,NULL,0);return INTEGER;}

"'" {COUNT();BEGIN(character);}

<character>{
    \\n' {COUNT();token_val = '\n';BEGIN(INITIAL);return CHARACTER;}
    \\t' {COUNT();token_val = '\t';BEGIN(INITIAL);return CHARACTER;}
    \\r' {COUNT();token_val = '\r';BEGIN(INITIAL);return CHARACTER;}
    ..+'  {
        COUNT();sprintf(error_msg,"multiple characters in ''");
        BEGIN(INITIAL);
        return -2;
    }
    .'  {COUNT();token_val = yytext[0];BEGIN(INITIAL);return CHARACTER;}
    ' {
        COUNT();
        sprintf(error_msg,"no charater in ''");
        BEGIN(INITIAL);
        return -2;
    }
}

\" {COUNT();str_buff_ptr = str_buff;BEGIN(str);}
<str>{
    \" {
        BEGIN(INITIAL);
        COUNT();
        *str_buff_ptr = 0;
        return STR;
    }
    \n {
        COUNT();
        sprintf(error_msg,"unterminated string");
        BEGIN(INITIAL);
        return -2;
    }

    \\n  {COUNT();*str_buff_ptr++ = '\n';}
    \\t  {COUNT();*str_buff_ptr++ = '\t';}
    \\r  {COUNT();*str_buff_ptr++ = '\r';}
    \\b  {COUNT();*str_buff_ptr++ = '\b';}
    \\f  {COUNT();*str_buff_ptr++ = '\f';}

    \\(.|\n)  {COUNT();*str_buff_ptr++ = yytext[1];}

    [^\\\n\"]+        {
        char *yptr = yytext;
        COUNT();
        while ( *yptr )
                *str_buff_ptr++ = *yptr++;
        }
}


"for" {COUNT();return FOR;}
"while" {COUNT();return WHILE;}
"continue" {COUNT();return CONTINUE;}
"break" {COUNT();return BREAK;}
"if" {COUNT();return IF;}
"else" {COUNT();return ELSE;}
"switch" {COUNT();return SWITCH;}
"case" {COUNT();return CASE;}
"default" {COUNT();return DEFAULT;}
"do" {COUNT();return DO;}
"struct" {COUNT();return STRUCT;}
"union" {COUNT();return UNION;}
"enum" {COUNT();return ENUM;}
"static" {COUNT();return STATIC;}
"extern" {COUNT();return EXTERN;}
"typedef" {COUNT();return TYPEDEF;}
"return" {COUNT();return RETURN;}
"const" {COUNT();return CONST;}
"char" {COUNT();return CHAR;}
"short" {COUNT();return SHORT;}
"int" {COUNT();return INT;}
"float" {COUNT();return FLOAT;}
"double" {COUNT();return DOUBLE;}
"unsigned" {COUNT();return UNSIGNED;}
"void" {COUNT();return VOID;}

({LETTER}|_)({LETTER}|{NUM}|_)* {COUNT(); return ID;}
{PUNCT}  {COUNT();return yytext[0];}

"<=" {COUNT();token_val=LE;return OP;}
"==" {COUNT();token_val=EQ;return OP;}
">=" {COUNT();token_val=GE;return OP;}
"!=" {COUNT();token_val=NE;return OP;}
"&&" {COUNT();token_val=AND;return OP;}
"||" {COUNT();token_val=OR;return OP;}
"<<" {COUNT();token_val=LEFT_SHIFT;return OP;}
">>" {COUNT();token_val=RIGHT_SHIFT;return OP;}
"++" {COUNT();token_val=INC;return OP;}
"--" {COUNT();token_val=DEC;return OP;}
"+=" {COUNT();token_val=ASSIGN_ADD;return OP;}
"-=" {COUNT();token_val=ASSIGN_SUB;return OP;}
"*=" {COUNT();token_val=ASSIGN_MUL;return OP;}
"/=" {COUNT();token_val=ASSIGN_DIV;return OP;}

"<" {COUNT();token_val=LT;return OP;}
">" {COUNT();token_val=GT;return OP;}
"+" {COUNT();token_val=yytext[0];return OP;}
"-" {COUNT();token_val=yytext[0];return OP;}
"*" {COUNT();token_val=yytext[0];return OP;}
"/" {COUNT();token_val=yytext[0];return OP;}
"=" {COUNT();token_val=yytext[0];return OP;}
"&" {COUNT();token_val=yytext[0];return OP;}
"|" {COUNT();token_val=yytext[0];return OP;}
"^" {COUNT();token_val=yytext[0];return OP;}
"~" {COUNT();token_val=yytext[0];return OP;}
"!" {COUNT();token_val=yytext[0];return OP;}
"%" {COUNT();token_val=yytext[0];return OP;}


"\n" {line_num++;COUNT();cur_line_pos = 1;}

" "|"\r"|"\t" {COUNT();}

. {COUNT();sprintf(error_msg,"unknown character");return -2;}

<<EOF>> return EOF;
%%

int yywrap(){
    return 1;
}
