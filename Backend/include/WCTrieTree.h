#ifndef WC_TRIE_TREE_H
#define WC_TRIE_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WCWord.h"
#include "WCIndex.h"

struct WCTrieTree;
typedef struct WCTrieTree WCTrieTree;

WCTrieTree * wc_trie_tree_create(WCError * error);
void wc_trie_tree_destroy(WCTrieTree * tree, WCError * error);
void wc_trie_tree_insert_word(WCTrieTree * tree, WCWord * word, WCError * error);
WCIndex * wc_trie_tree_search_word(WCTrieTree * tree, WCWord * word, WCError * error);












#endif
