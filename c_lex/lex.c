//
// Created by 谢卫凯 on 2022/9/23.
//
#include <stdio.h>

#include "lex.h"

#define END_STATE -1

#define IS_LETTER(x) (((x)>='a' && (x) <= 'z') || ((x)>='A' && (x) <= 'Z'))
#define IS_NUMBER(x) ((x) >= '0' && (x) <= '9')
#define IS_WHITE_SPACE(x) ((x) == ' ' || (x) == '\n' || (x) == '\t' || x == '\r')


//auto      break     case       char      const         continue  default     do
//double  else       enum      extern   float           for             goto         if
//int long register return short signed sizeof static
//struct   switch    typedef  union    unsigned  void            volatile   while
static char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

// insert build-in symbols (keywords) into the symbol table
void insert_buildin_symbols(lex *l) {
    item data;
    data.token = AUTO;
    trie_insert(l->symbols,"auto",&data,sizeof(data));
    data.token = BREAK;
    trie_insert(l->symbols,"break",&data,sizeof(data));
    data.token = CASE;
    trie_insert(l->symbols,"case",&data,sizeof(data));
    data.token = CHAR;
    trie_insert(l->symbols,"char",&data,sizeof(data));
    data.token = CONST;
    trie_insert(l->symbols,"const",&data,sizeof(data));
    data.token = CONTINUE;
    trie_insert(l->symbols,"continue",&data,sizeof(data));
    data.token = DEFAULT;
    trie_insert(l->symbols,"default",&data,sizeof(data));
    data.token = DO;
    trie_insert(l->symbols,"do",&data,sizeof(data));
    data.token = DOUBLE;
    trie_insert(l->symbols,"double",&data,sizeof(data));
    data.token = ELSE;
    trie_insert(l->symbols,"else",&data,sizeof(data));
    data.token = ENUM;
    trie_insert(l->symbols,"enum",&data,sizeof(data));
    data.token = EXTERN;
    trie_insert(l->symbols,"extern",&data,sizeof(data));
    data.token = FLOAT;
    trie_insert(l->symbols,"float",&data,sizeof(data));
    data.token = FOR;
    trie_insert(l->symbols,"for",&data,sizeof(data));
    data.token = GOTO;
    trie_insert(l->symbols,"goto",&data,sizeof(data));
    data.token = IF;
    trie_insert(l->symbols,"if",&data,sizeof(data));
    data.token = INT;
    trie_insert(l->symbols,"int",&data,sizeof(data));
    data.token = LONG;
    trie_insert(l->symbols,"long",&data,sizeof(data));
    data.token = REGISTER;
    trie_insert(l->symbols,"register",&data,sizeof(data));
    data.token = RETURN;
    trie_insert(l->symbols,"return",&data,sizeof(data));
    data.token = SHORT;
    trie_insert(l->symbols,"short",&data,sizeof(data));
    data.token = SIGNED;
    trie_insert(l->symbols,"signed",&data,sizeof(data));
    data.token = SIZEOF;
    trie_insert(l->symbols,"sizeof",&data,sizeof(data));
    data.token = STATIC;
    trie_insert(l->symbols,"static",&data,sizeof(data));
    data.token = STRUCT;
    trie_insert(l->symbols,"struct",&data,sizeof(data));
    data.token = SWITCH;
    trie_insert(l->symbols,"switch",&data,sizeof(data));
    data.token = TYPEDEF;
    trie_insert(l->symbols,"typedef",&data,sizeof(data));
    data.token = UNION;
    trie_insert(l->symbols,"union",&data,sizeof(data));
    data.token = UNSIGNED;
    trie_insert(l->symbols,"unsigned",&data,sizeof(data));
    data.token = VOID;
    trie_insert(l->symbols,"void",&data,sizeof(data));
    data.token = VOLATILE;
    trie_insert(l->symbols,"volatile",&data,sizeof(data));
    data.token = WHILE;
    trie_insert(l->symbols,"while",&data,sizeof(data));
}

int init_lex(char *file_name, lex *l) {
    if ((l->lex_in = fopen(file_name, "r")) == NULL)
        return -1; // failed to open file

    l->cur_ch = getc(l->lex_in);  // read first char

    if ((l->symbols = new_dict()) == NULL) { // fail to create symbol table
        fclose(l->lex_in);
        return -1;
    }

    l->text_ptr = l->text;

    l->line = 1;
    l->pos = 1;
    l->num_ch = 0;

    insert_buildin_symbols(l);

    return 1;
}

typedef int (*func)(lex *);


int state0(lex *l){ // initial state
    if(IS_LETTER(l->cur_ch) || l->cur_ch == '_'){ // ID goto state 1
        *(l->text_ptr++) = l->cur_ch;
        l->cur_ch = getc(l->lex_in);
        return 1;
    }

    if(IS_WHITE_SPACE(l->cur_ch)) { // skip white space
        l->cur_ch = getc(l->lex_in);
        return 0;
    }

    l->token = l->cur_ch;
    l->cur_ch = getc(l->lex_in);
    *(l->text_ptr++) = l->token;
    *(l->text_ptr++) = 0;
    l->text_ptr = l->text;
    return END_STATE;
}

int state1(lex *l){ // state to read an ID
    item entry;
    item *tmp;
    if(IS_LETTER(l->cur_ch) || IS_NUMBER(l->cur_ch) || l->cur_ch == '_'){
        *(l->text_ptr++) = l->cur_ch;
        l->cur_ch = getc(l->lex_in);
        return 1;
    }else{
        *l->text_ptr = 0;
        l->text_ptr = l->text;
        if((tmp = get_key_data_ptr(l->symbols,l->text)) != NULL)
            l->token = tmp->token;
        else{
            l->token = entry.token = ID;
            tmp = trie_insert(l->symbols,l->text,&entry, sizeof(entry));
        }

        l->val.type = ID_TYPE;
        l->val.value.data = tmp; // entry to symbol table

        return END_STATE;
    }
}

static func states[128] = {
        state0, state1,
};

int next(lex *l){
    int state = 0;

    // every time the automaton consumes a token ,state becomes 0
    int last_ch = l->cur_ch;
    while((state = states[state](l)) != END_STATE){
        if(last_ch == '\n'){
            l->line ++;
            l->pos = 1;
        }else
            l->pos ++;
        l->num_ch ++;

        last_ch = l->cur_ch;
    }
    return l->token;
}

