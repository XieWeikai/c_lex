#include <stdio.h>
#include <stdlib.h>

#include "lex.h"
#include "format.h"

int main(int argc,char *argv[]) {
    if(argc <= 1){
        fprintf(stderr,"no input file\n");
        exit(-1);
    }
    lex l;
    char *file_name = argv[1];
    if(init_lex(file_name,&l) == -1){
        fprintf(stderr,"lex initialization failed\n");
        exit(-1);
    }

    int punctuator = 0;
    int id = 0;
    int keyword = 0;
    int str = 0;
    int error = 0 ;
    int integer = 0;
    int decimal = 0;
    int character = 0;


    int token;
    char buff[128];
    while((token = next(&l)) != EOF){
        format_token(token,buff);
        if(token < ID) {
            printf("%s:%d:%d < %s, >  text:%s\n", file_name, l.token_line, l.token_pos, buff, l.text);
            if(token >= AUTO)
                keyword ++;
        } else punctuator ++;
        if(token == ID) {
            printf("%s:%d:%d < %s, %p >  text:%s\n", file_name, l.token_line, l.token_pos, buff, l.val.value.data,
                   l.text);
            id ++;
        }
        if(token == STR) {
            printf("%s:%d:%d < %s, %s>  text:%s\n", file_name, l.token_line, l.token_pos, buff, l.val.value.str,
                   l.text);
            str ++;
        }
        if(token == ERROR) {
            printf("%s:%d:%d:error:%s\n", file_name, l.token_line, l.token_pos, l.err_msg);
            error ++;
        }
        if(token == CHARACTER) {
            printf("%s:%d:%d < %s, %c>  text:%s\n", file_name, l.token_line, l.token_pos, buff, l.val.value.ch, l.text);
            character ++;
        }
        if(token == INTEGER) {
            printf("%s:%d:%d < %s, %d>  text:%s\n", file_name, l.token_line, l.token_pos, buff, l.val.value.integer,
                   l.text);
            integer ++;
        }
        if(token == DECIMAL) {
            printf("%s:%d:%d < %s, %lf>  text:%s\n", file_name, l.token_line, l.token_pos, buff, l.val.value.decimal,
                   l.text);
            decimal ++;
        }
    }

    printf("-----------------------------------------\n");
    printf("punctuator:  %d\n",punctuator);
    printf("id:  %d\n",id);
    printf("keywords:  %d\n",keyword);
    printf("string:  %d\n",str);
    printf("error:  %d\n",error);
    printf("integer:  %d\n",integer);
    printf("decimal:  %d\n",decimal);
    printf("character:  %d\n",character);

    printf("total number of character:%d\n",l.num_ch);


    return 0;
}
