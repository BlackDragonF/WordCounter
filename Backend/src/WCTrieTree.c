#include "WCTrieTree.h"
#include "WCIndex.h"
#define MAX_CHARACTER_NUMBER (26)

struct WCTrieNode {
    char character;
    struct WCTrieNode * child[MAX_CHARACTER_NUMBER];
    WCIndex * index;
};

struct WCTrieTree {
    struct WCTrieNode * root;
};

static inline int wc_chararcter_index_convert(char character) {
    return tolower(character) - 'a';
}

WCTrieTree * wc_trie_tree_create(WCError * error) {
    WCTrieTree * tree = malloc(sizeof(WCTrieTree));
    if (tree == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    tree->root = malloc(sizeof(WCTrieNode));
    if (tree->root == NULL) {
        *error = WCMemoryOverflowError;
        free(tree);
        return NULL;
    }
    tree->root->character = '\0';
    memset(tree->root->child, 0, sizeof(struct WCTrieNode *) * MAX_CHARACTER_NUMBER);
    tree->root->index = NULL;
    *error = WCNoneError;
    return tree;
}

void wc_trie_tree_insert_word(WCTrieTree * tree, WCWord * word, WCError * error) {
    if (tree == NULL || word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCError internalError;
    char * word = wc_word_get_word(word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    int index;
    WCTrieNode * node = tree->root;
    while (*word) {
        index = wc_chararcter_index_convert(*word);
        if (node->child[index] == NULL) {
            WCTrieNode * temp = malloc(sizeof(WCTrieNode));
            temp->character = *word;
            memset(temp->child, 0, sizeof(struct WCTrieNode *) * MAX_CHARACTER_NUMBER);
            temp->index = NULL;
            node->child[index] = temp;
        }
        node = node->child[index];
        word++;
    }
    if (node->index == NULL) {
        node->index = wc_index_create(&internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
    }
    wc_index_add(node->index, word->info, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    *error = WCNoneError;
}

WCIndex * wc_trie_tree_search_word(WCTrieTree * tree, WCWord * word, WCError * error) {
    if (tree == NULL || word == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    WCError internalError;
    char * word = wc_word_get_word(word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    int index;
    WCTrieNode * node = tree->root;
    while (*word) {
        index = wc_chararcter_index_convert(*word);
        if (node->child[index] == NULL) {
            *error = WCNoneError;
            return NULL;
        }
        node = node->child[index];
        word++;
    }
    *error = WCNoneError;
    return node->index;
}
