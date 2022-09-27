//
// Created by 谢卫凯 on 2022/9/23.
//
#include <stdio.h>

#include "lex.h"

#define END_STATE -1

#define IS_LETTER(x) (((x)>='a' && (x) <= 'z') || ((x)>='A' && (x) <= 'Z'))
#define IS_NUMBER(x) ((x) >= '0' && (x) <= '9')
#define IS_WHITE_SPACE(x) ((x) == ' ' || (x) == '\n' || (x) == '\t' || x == '\r')

#define NEXT_CHAR(l) do{*(l->text_ptr ++) = l->cur_ch;l->cur_ch = getc(l->lex_in);}while(0)


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
    trie_insert(l->symbols, "auto", &data, sizeof(data));
    data.token = BREAK;
    trie_insert(l->symbols, "break", &data, sizeof(data));
    data.token = CASE;
    trie_insert(l->symbols, "case", &data, sizeof(data));
    data.token = CHAR;
    trie_insert(l->symbols, "char", &data, sizeof(data));
    data.token = CONST;
    trie_insert(l->symbols, "const", &data, sizeof(data));
    data.token = CONTINUE;
    trie_insert(l->symbols, "continue", &data, sizeof(data));
    data.token = DEFAULT;
    trie_insert(l->symbols, "default", &data, sizeof(data));
    data.token = DO;
    trie_insert(l->symbols, "do", &data, sizeof(data));
    data.token = DOUBLE;
    trie_insert(l->symbols, "double", &data, sizeof(data));
    data.token = ELSE;
    trie_insert(l->symbols, "else", &data, sizeof(data));
    data.token = ENUM;
    trie_insert(l->symbols, "enum", &data, sizeof(data));
    data.token = EXTERN;
    trie_insert(l->symbols, "extern", &data, sizeof(data));
    data.token = FLOAT;
    trie_insert(l->symbols, "float", &data, sizeof(data));
    data.token = FOR;
    trie_insert(l->symbols, "for", &data, sizeof(data));
    data.token = GOTO;
    trie_insert(l->symbols, "goto", &data, sizeof(data));
    data.token = IF;
    trie_insert(l->symbols, "if", &data, sizeof(data));
    data.token = INT;
    trie_insert(l->symbols, "int", &data, sizeof(data));
    data.token = LONG;
    trie_insert(l->symbols, "long", &data, sizeof(data));
    data.token = REGISTER;
    trie_insert(l->symbols, "register", &data, sizeof(data));
    data.token = RETURN;
    trie_insert(l->symbols, "return", &data, sizeof(data));
    data.token = SHORT;
    trie_insert(l->symbols, "short", &data, sizeof(data));
    data.token = SIGNED;
    trie_insert(l->symbols, "signed", &data, sizeof(data));
    data.token = SIZEOF;
    trie_insert(l->symbols, "sizeof", &data, sizeof(data));
    data.token = STATIC;
    trie_insert(l->symbols, "static", &data, sizeof(data));
    data.token = STRUCT;
    trie_insert(l->symbols, "struct", &data, sizeof(data));
    data.token = SWITCH;
    trie_insert(l->symbols, "switch", &data, sizeof(data));
    data.token = TYPEDEF;
    trie_insert(l->symbols, "typedef", &data, sizeof(data));
    data.token = UNION;
    trie_insert(l->symbols, "union", &data, sizeof(data));
    data.token = UNSIGNED;
    trie_insert(l->symbols, "unsigned", &data, sizeof(data));
    data.token = VOID;
    trie_insert(l->symbols, "void", &data, sizeof(data));
    data.token = VOLATILE;
    trie_insert(l->symbols, "volatile", &data, sizeof(data));
    data.token = WHILE;
    trie_insert(l->symbols, "while", &data, sizeof(data));
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

static int is_in(int ch, char *s) {
    while (*s != 0)
        if (ch == *s++)
            return 1;
    return 0;
}

int state0(lex *l) { // initial state
    if (IS_LETTER(l->cur_ch) || l->cur_ch == '_') { // ID    goto state 1
        *(l->text_ptr++) = l->cur_ch;
        l->cur_ch = getc(l->lex_in);
        return 1;
    }

    if (IS_WHITE_SPACE(l->cur_ch)) { // skip white space
        l->cur_ch = getc(l->lex_in);
        return 0;
    }

    // multi-char punctuators
    // need to look ahead
    if (is_in(l->cur_ch, "&*+-!/%<=>|")) {
        l->token = l->cur_ch; // store current char so next state can decide which punctuator it is
        *(l->text_ptr++) = l->cur_ch;
        l->cur_ch = getc(l->lex_in);
        return 2;
    }

    if (l->cur_ch == '"') {
        *(l->text_ptr++) = l->cur_ch;
        l->cur_ch = getc(l->lex_in);
        l->val.str_ptr = l->val.value.str;
        return 6;
    }

    // []{}().^?:;,~
    // return char as token directly
    l->token = l->cur_ch;
    l->cur_ch = getc(l->lex_in);
    *(l->text_ptr++) = l->token;
    *(l->text_ptr) = 0;
    l->text_ptr = l->text;
    return END_STATE;
}

// skip line comment
int state3(lex *l) {
    int tmp_ch = l->cur_ch;
    l->cur_ch = getc(l->lex_in);
    if (tmp_ch == '\n')
        return 0;
    else
        return 3;
}

// skip block comment
int state4(lex *l) {
    int tmp_ch = l->cur_ch;
    l->cur_ch = getc(l->lex_in);
    if (tmp_ch == '*')
        return 5;
    else
        return 4;
}

// skip block comment
int state5(lex *l) {
    int tmp_ch = l->cur_ch;
    l->cur_ch = getc(l->lex_in);
    switch (tmp_ch) {
        case '*':
            return 5;
        case '/':
            return 0;
        default:
            return 4;
    }
}

// decide which punctuator
int state2(lex *l) {
    int mul_char = 0;
    switch (l->token) {
        case '&':
            if (l->cur_ch == '&') {
                mul_char = 1;
                l->token = AND;
            }
            break;
        case '*':
            if (l->cur_ch == '=') {
                mul_char = 1;
                l->token = MUL_ASSIGN;
            }
            break;
        case '+':
            if (l->cur_ch == '+' || l->cur_ch == '=') {
                mul_char = 1;
                if (l->cur_ch == '=')
                    l->token = ADD_ASSIGN;
                else
                    l->token = INC;
            }
            break;
        case '-':
            if (l->cur_ch == '-' || l->cur_ch == '=' || l->cur_ch == '>') {
                mul_char = 1;
                if (l->cur_ch == '-')
                    l->token = DEC;
                else if (l->cur_ch == '>')
                    l->token = ARROW;
                else
                    l->token = SUB_ASSIGN;
            }
            break;
        case '!':
            if (l->cur_ch == '=') {
                mul_char = 1;
                l->token = NE;
            }
            break;
        case '/':
            if (l->cur_ch == '=') {
                mul_char = 1;
                l->token = DIV_ASSIGN;
            } else if (l->cur_ch == '/') { // line comment
                l->text_ptr = l->text;
                l->cur_ch = getc(l->lex_in);
                return 3;
            } else if (l->cur_ch == '*') { // block comment
                l->text_ptr = l->text;
                l->cur_ch = getc(l->lex_in);
                return 4;
            }
            break;
        case '%':
            if (l->cur_ch == '=') {
                mul_char = 1;
                l->token = MOD_ASSIGN;
            }
            break;
        case '<':
            if (l->cur_ch == '<' || l->cur_ch == '=') {
                mul_char = 1;
                if (l->cur_ch == '<')
                    l->token = LEFT_SHIFT;
                else
                    l->token = LE;
            }
            break;
        case '>':
            if (l->cur_ch == '>' || l->cur_ch == '=') {
                mul_char = 1;
                if (l->cur_ch == '>')
                    l->token = RIGHT_SHIFT;
                else
                    l->token = GE;
            }
            break;
        case '=':
            if (l->cur_ch == '=') {
                mul_char = 1;
                l->token = EQ;
            }
            break;
        case '|':
            if (l->cur_ch == '|') {
                mul_char = 1;
                l->token = OR;
            }
            break;
    }
    if (mul_char) {
        *(l->text_ptr++) = l->cur_ch;
        l->cur_ch = getc(l->lex_in);
        // count here (this function return END_STATE,meaning that next function will not count when return to next function)
        l->pos++;
        l->num_ch++;
    }

    *l->text_ptr = 0;
    l->text_ptr = l->text;
    return END_STATE;
}

int state1(lex *l) { // state to read an ID
    item entry;
    item *tmp;
    if (IS_LETTER(l->cur_ch) || IS_NUMBER(l->cur_ch) || l->cur_ch == '_') {
        *(l->text_ptr++) = l->cur_ch;
        l->cur_ch = getc(l->lex_in);
        return 1;
    } else {
        *l->text_ptr = 0;
        l->text_ptr = l->text;
        if ((tmp = get_key_data_ptr(l->symbols, l->text)) != NULL)
            l->token = tmp->token;
        else {
            l->token = entry.token = ID;
            tmp = trie_insert(l->symbols, l->text, &entry, sizeof(entry));
        }

        l->val.type = ID_TYPE;
        l->val.value.data = tmp; // entry to symbol table

        return END_STATE;
    }
}

// start to handle string
int state6(lex *l) {
    while(1) {
        if(l->cur_ch == '"'){
            NEXT_CHAR(l);
            l->pos ++; l->num_ch ++;
            while(IS_WHITE_SPACE(l->cur_ch)){  // skip white space
                l->pos ++; l->num_ch ++;
                if(l->cur_ch == '\n'){
                    l->line ++;
                    l->pos = 1;
                }
                NEXT_CHAR(l);
            }
            if(l->cur_ch == '"') {
                NEXT_CHAR(l);
                l->pos ++; l->num_ch ++;
                continue;
            }
            while(*l->text_ptr != '"')
                l->text_ptr --;
            l->text_ptr[1] = 0;
            l->text_ptr = l->text;
            l->token = STR;
            l->val.type = STRING_TYPE;
            *(l->val.str_ptr) = 0;
            return END_STATE;
        }else if(l->cur_ch == '\n'){ // ERROR unclosed string
            NEXT_CHAR(l);
            l->text_ptr = l->text;
            l->pos = 1;
            l->line ++;
            l->num_ch ++;
            l->token = ERROR;
            sprintf(l->err_msg,"unclosed string");
            return END_STATE;
        }else if(l->cur_ch == '\\'){
            NEXT_CHAR(l);
            l->pos ++;l->num_ch ++;
            switch (l->cur_ch) {
                case 'n':
                    *l->val.str_ptr++ = '\n';
                    break;
                case 't':
                    *l->val.str_ptr++ = '\t';
                    break;
                case 'r':
                    *l->val.str_ptr++ = '\r';
                    break;
                case '0':
                    *l->val.str_ptr++ = '\0';
                    break;
                default:
                    *l->val.str_ptr++ = l->cur_ch;
                    break;
            }
            NEXT_CHAR(l);
            l->pos ++;l->num_ch ++;
        }else if(l->cur_ch == EOF){
            *l->text_ptr = 0;
            l->text_ptr = l->text;
            l->token = ERROR;
            sprintf(l->err_msg,"reach EOF parsing string");
            return END_STATE;
        } else{
            *(l->val.str_ptr++) = l->cur_ch;
            NEXT_CHAR(l);
            l->pos ++;l->num_ch ++;
        }
    }
}

static func states[128] = {
        state0, state1, state2, state3, state4, state5, state6
};

int next(lex *l) {
    int state = 0;

    // every time the automaton consumes a token ,state becomes 0
    int last_ch = l->cur_ch;
    l->token_pos = l->pos;
    l->token_line = l->line;
    while ((state = states[state](l)) != END_STATE) {
        if (last_ch == '\n') {
            l->line++;
            l->pos = 1;
        } else
            l->pos++;
        l->num_ch++;

        last_ch = l->cur_ch;

        // record current token position
        if(state == 0) {
            l->token_pos = l->pos;  // record position before starting to scan a token (state 0 means to scan a new token)
            l->token_line = l->line;
        }

    }
    return l->token;
}

