//
// Created by 谢卫凯 on 2022/9/29.
//

#include <stdio.h>

char *keywords[] = {
        "auto","break","case","char","const","continue","default","do",
        "double","else","enum","extern","float","for","goto","if",
        "int","long","register","return","short","signed","sizeof","static",
        "struct","switch","typedef","union","unsigned","void","volatile","while",
};

static void upper(char *buff,char *s){
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

void generate_buildin_code(){
    printf("item data;\n");
    char buff[128];
    for (int i = 0; i < 32; i++) {
        upper(buff,keywords[i]);
        printf("data.token = %s;\n", buff);
        printf("trie_insert(l->symbols,\"%s\",&data,sizeof(data));\n",keywords[i]);
    }
}

int main(){
    generate_buildin_code();
    printf("\n\n-----------------------------------------------------------\n\n");
    generate_enum();

    return 0;
}
