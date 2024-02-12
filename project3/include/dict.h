#pragma once

#ifndef DICT_H
#define DICT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')
#define alphabetsize 26

typedef struct Trie {
    struct Trie *child[alphabetsize];
    bool end;
} Trie;

Trie *create();
void insert(Trie *dict, char *str);
bool lookup(Trie *dict, char *str);
void destroy(Trie *dict);

#endif
