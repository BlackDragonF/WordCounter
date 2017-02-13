#ifndef WC_TRIE_TREE_H
#define WC_TRIE_TREE_H

/* include necessary libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* include word/index/file/error modules */
#include "WCWord.h"
#include "WCIndex.h"
#include "WCFileHandler.h"
#include "WCErrorHandler.h"

/* struct declaration */
struct WCTrieTree;
typedef struct WCTrieTree WCTrieTree;

/* struct declaration */
typedef struct WCTrieTreeTraverseResult {;
    WCWordInfo * info;
    char ** words;
    int count;
}WCTrieTreeTraverseResult;

/* function list */

/* construct and destruct */
WCTrieTree * wc_trie_tree_create(WCError * error);
void wc_trie_tree_destroy(WCTrieTree * tree, WCError * error);

/* insert/search/delete */
void wc_trie_tree_insert_word(WCTrieTree * tree, WCWord * word, WCError * error);
WCIndex * wc_trie_tree_search_word(WCTrieTree * tree, WCWord * word, WCError * error);
void wc_trie_tree_delete_word(WCTrieTree * tree, WCWord * word, WCError * error);

/* get operations */
int wc_trie_tree_get_count(WCTrieTree * tree, WCError * error);
int wc_trie_tree_get_unique(WCTrieTree * tree, WCError * error);

/* traverse related */
WCTrieTreeTraverseResult * wc_trie_tree_traverse(WCTrieTree * tree, WCError * error);
void wc_trie_tree_traverse_result_destroy(WCTrieTreeTraverseResult * result, WCError * error);

/* serialization and deserialization */
void wc_trie_tree_write_to_file(WCTrieTree * trie, WCFileHandler * handler, const char * path, WCError * error);
WCTrieTree * wc_trie_tree_read_from_file(WCFileHandler * handler, WCError * error);

#endif
