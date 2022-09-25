#include <stdio.h>
#include <string.h>

#include "trie.h"
#include "lex.h"
#include "format.h"

char *keywords[] = {
        "auto","break","case","char","const","continue","default","do",
        "double","else","enum","extern","float","for","goto","if",
        "int","long","register","return","short","signed","sizeof","static",
        "struct","switch","typedef","union","unsigned","void","volatile","while",
};

void upper(char *buff,char *s){
    char ch;
    while((ch = *s++) != 0)
        *buff++ = ch - 'a' + 'A';
    *buff = 0;
}

void generate_enum(){
    char buff[100];

    printf("typedef enum token{\n\t");
    for (int i = 0; i < 32; i++) {
        upper(buff,keywords[i]);
        printf("%s,",buff);
    }
    printf("\n}token;\n");
}

void test_trie_tree(){
    dict d = new_dict();
    int score;

    score = 95;
    trie_insert(d, "xwk", &score, sizeof(score));

    score = 96;
    trie_insert(d, "wql", &score, sizeof(score));

    score = 98;
    trie_insert(d, "yr", &score, sizeof(score));

    if(get_key_data(d,"wql",&score))
        printf("wql:%d\n", score);

    if(get_key_data(d,"yr",&score))
        printf("yr:%d\n", score);

    if(get_key_data(d,"xwk",&score))
        printf("xwk:%d\n", score);

    int *p = get_key_data_ptr(d,"xwk");
    *p = 99;

    if(get_key_data(d,"xwk",&score))
        printf("after modifying score;xwk:%d\n", score);

    destroy_trie_tree(d);

    printf("destroyed the trie tree\n");
}

void generate_buildin_code(){
    printf("item data;\n");
    char buff[128];
    for (int i = 0; i < 32; i++) {
        upper(buff,keywords[i]);
        printf("data.token = %s;\n", buff);
        printf("trie_insert(l->symbols,\"%s\",&data,sizeof(data));\n",keywords[i]);
    }
}

int main() {
    char *file_name = "test.c";
    lex l;
    init_lex(file_name,&l);

    int token;
    char buff[128];
    while((token = next(&l)) != EOF){
        format_token(token,buff);
        printf("%s:%d:%d <%s,>  text:%s\n",file_name,l.line,(int)(l.pos-strlen(l.text)),buff,l.text);
    }

    return 0;
}
