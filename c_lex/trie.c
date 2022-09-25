//
// Created by 谢卫凯 on 2022/9/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

// return a new trie tree node
// if data is NULL then there will be no data in the node
// or data will be stored in the node
static trie_node *new_trie_node(void *data, size_t size) {
    trie_node *t = calloc(1, sizeof(*t));
    if (t == NULL)  // fail to allocate memory
        return NULL;
    t->data = NULL;
    t->size = 0;
    if (data != NULL) {
        t->size = size;
        t->data = malloc(size);
        memcpy(t->data, data, size);
    }
    for (int i = 0; i < TRIE_NEXT_LEN; i++)
        t->children[i] = NULL;

    return t;
}

// return a new dict
dict new_dict(){
    return new_trie_node(NULL,0);
}

// insert some data into the trie tree
// return address that store data
void *trie_insert(dict d,char *key,void *data,size_t size){
    trie_node *t = d;
    char ch;
    while((ch = *key++) != 0){
        if(t->children[ch] == NULL)
            t->children[ch] = new_trie_node(NULL,0);
        t = t->children[ch];
    }
    if(t->data != NULL)
        free(t->data); // override data
    t->data = malloc(size);
    t->size = size;
    memcpy(t->data,data,size);
    return t->data;
}

// try to get the data insert with a key earlier
// if success return 1
// else 0
int get_key_data(dict d,char *key,void *data){
    trie_node  *t = d;
    char ch;
    while((ch = *key++) != 0 && t != NULL)
        t = t->children[ch];
    if(t == NULL)
        return 0;
    memcpy(data,t->data,t->size);
    return 1;
}

// check if a key is already in trie tree
int key_in(dict d, char *key) {
    trie_node  *t = d;
    char ch;
    while((ch = *key++) != 0 && t != NULL)
        t = t->children[ch];
    if(t == NULL)
        return 0;
    return 1;
}

void *get_key_data_ptr(dict d, char *key) {
    trie_node  *t = d;
    char ch;
    while((ch = *key++) != 0 && t != NULL)
        t = t->children[ch];
    return t != NULL ? t->data : NULL;
}

void destroy_trie_tree(dict d) {
    if(d == NULL)
        return;
    if(d->size > 0)
        free(d->data);
    for (int i = 0; i < TRIE_NEXT_LEN; i++)
        destroy_trie_tree(d->children[i]);
    free(d);
}


