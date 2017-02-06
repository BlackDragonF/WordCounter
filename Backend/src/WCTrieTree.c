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
    tree->root = malloc(sizeof(struct WCTrieNode));
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
    const char * wordPtr = wc_word_get_word(word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    int index;
    struct WCTrieNode * node = tree->root;
    while (*wordPtr) {
        index = wc_chararcter_index_convert(*wordPtr);
        if (node->child[index] == NULL) {
            struct WCTrieNode * temp = malloc(sizeof(struct WCTrieNode));
            temp->character = *wordPtr;
            memset(temp->child, 0, sizeof(struct WCTrieNode *) * MAX_CHARACTER_NUMBER);
            temp->index = NULL;
            node->child[index] = temp;
        }
        node = node->child[index];
        wordPtr++;
    }
    if (node->index == NULL) {
        node->index = wc_index_create(&internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
    }
    WCWordInfo info = wc_word_get_info(word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    wc_index_add(node->index, info, &internalError);
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
    const char * wordPtr = wc_word_get_word(word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    int index;
    struct WCTrieNode * node = tree->root;
    while (*wordPtr) {
        index = wc_chararcter_index_convert(*wordPtr);
        if (node->child[index] == NULL) {
            *error = WCNoneError;
            return NULL;
        }
        node = node->child[index];
        wordPtr++;
    }
    *error = WCNoneError;
    return node->index;
}
