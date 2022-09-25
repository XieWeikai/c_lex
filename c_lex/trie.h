//
// Created by 谢卫凯 on 2022/9/23.
//

#ifndef C_LEX_TRIE_H
#define C_LEX_TRIE_H

#define TRIE_NEXT_LEN 128

#include <stdio.h>

typedef struct node{
    void *data;
    size_t size;

    struct node *children[TRIE_NEXT_LEN];
}trie_node;

typedef trie_node *dict;

dict new_dict();

void *trie_insert(dict d,char *key,void *data,size_t size);

int get_key_data(dict d,char *key,void *data);

int key_in(dict d, char *key);

void *get_key_data_ptr(dict d,char *key);

void destroy_trie_tree(dict d);

#endif //C_LEX_TRIE_H
