
#include "dict.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Trie *create() {
    struct Trie *new = NULL;
    new = (struct Trie *)malloc(sizeof(struct Trie));

    if (new) {
        new->end = false;
        for (int i = 0; i < alphabetsize; i++) {
            new->child[i] = NULL;
        }
    }
    return new;
}

void insert(struct Trie *dict, char *str) {
    int length = strlen(str);
    int index;
    struct Trie *current = dict;
    for (int i = 0; i < length; i++) {
        index = CHAR_TO_INDEX(str[i]);
        if (!current->child[index]) {
            current->child[index] = create();
        }
        current = current->child[index];
    }
    current->end = true;
}

bool lookup(Trie *dict, char *str) {
    int index;
    int length = strlen(str);
    struct Trie *current = dict;
    if (str == NULL) {
        return false;
    }
    if (current == NULL) {
        return false;
    }
    for (int i = 0; i < length; i++) {
        index = CHAR_TO_INDEX(str[i]);
        if (!current->child[index]) {
            return false;
        }
        current = current->child[index];
    }

    return current->end;
}

void destroy(Trie *dict) {
    if (dict == NULL) {
        return;
    }
    for (int i = 0; i < alphabetsize; i++) {
        destroy(dict->child[i]);
    }
    free(dict);
}
