#include <stdio.h>

#include "c_lexical_analyzer.h"

int main(int argc,char *argv[]) {
    if(argc < 2){
        fprintf(stderr,"no input file\n");
        return -1;
    }

    yyin = fopen(argv[1],"r");
    
    int token;
    while((token = yylex()) != EOF){
        if(token == -2){ // error occur
            fprintf(stderr,"%s:%d:%d: error: %s\n",argv[1],line_num,cur_line_pos,error_msg);
            continue;
        }
        printf("%s:%d:%d     ",argv[1],line_num,cur_line_pos);
        print_token(token);
    }
    return 0;
}
