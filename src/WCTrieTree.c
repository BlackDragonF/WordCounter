#include "WCTrieTree.h"

#define MAX_CHARACTER_NUMBER (26)

struct WCTrieNode {
    char character;
    struct WCTrieNode * child[MAX_CHARACTER_NUMBER];
    WCIndex * index;
};

struct WCTrieTree {
    struct WCTrieNode * root;
    int count;
    int unique;
};

static inline int wc_chararcter_index_convert(char character) {
    return tolower(character) - 'a';
}

static void wc_trie_node_destroy(struct WCTrieNode * node) {
    int index;
    for (index = 0 ; index < MAX_CHARACTER_NUMBER ; ++index) {
        if (node->child[index] != NULL) {
            wc_trie_node_destroy(node->child[index]);
        }
    }
    if (node->index != NULL) {
        WCError internalError;
        wc_index_destroy(node->index, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
    }
    free(node);
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
    tree->count = 0;
    tree->unique = 0;
    *error = WCNoneError;
    return tree;
}

void wc_trie_tree_destroy(WCTrieTree * tree, WCError * error) {
    printf("%d\n", tree->count);
    if (tree == NULL) {
        *error = WCNullPointerError;
        return;
    }
    wc_trie_node_destroy(tree->root);
    free(tree);
    *error = WCNoneError;
}

void wc_trie_tree_insert_word(WCTrieTree * tree, WCWord * word, WCError * error) {
    if (tree == NULL || word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCError internalError;
    tree->count++;
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
        tree->unique++;
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

static struct WCTrieNode * wc_trie_tree_search_node(WCTrieTree * tree, WCWord * word, WCError * error) {
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
    return node;
}

WCIndex * wc_trie_tree_search_word(WCTrieTree * tree, WCWord * word, WCError * error) {
    if (tree == NULL || word == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    WCError internalError;
    struct WCTrieNode * node = wc_trie_tree_search_node(tree, word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    *error = WCNoneError;
    if (node != NULL) {
        return node->index;
    } else {
        return NULL;
    }
}

void wc_trie_tree_delete_word(WCTrieTree * tree, WCWord * word, WCError * error) {
    if (tree == NULL || word == NULL) {
        *error = WCNullPointerError;
        return;
    }
    WCError internalError;
    struct WCTrieNode * node = wc_trie_tree_search_node(tree, word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    WCIndex * index = node->index;
    int count = wc_index_get_count(index, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    tree->count -= count;
    wc_index_destroy(index, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    node->index = NULL;
    *error = WCNoneError;
}

static void wc_trie_tree_traverse_recursively(struct WCTrieNode * node, WCWord * word, WCTrieTreeTraverseResult * result, int * result_index) {
    int index;
    WCError internalError;

    if (node == NULL) {
        return;
    }

    if (node->index != NULL) {
        int length = wc_word_get_length(word, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
        const char * wordPtr = wc_word_get_word(word, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
        (result->words)[*result_index] = malloc(sizeof(char) * (length + 1));
        if ((result->words)[*result_index] == NULL) {
            exit(WCMemoryOverflowError);
        }
        strcpy((result->words)[*result_index], wordPtr);
        WCIndexIterator * iterator = wc_index_iterator_create(node->index, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
        do {
            WCWordInfo info = wc_index_iterator_get_value(iterator, &internalError);
            if (internalError != WCNoneError) {
                exit(internalError);
            }
            (result->info)[*result_index] = info;
            (*result_index)++;

        } while (wc_index_iterator_next(iterator, &internalError), internalError != WCIndexRangeError);
        wc_index_iterator_destroy(iterator, &internalError);
        if (internalError != WCNoneError) {
            exit(internalError);
        }
    }
    for (index = 0 ; index < 26 ; ++index) {
        if (node->child[index] != NULL) {
            wc_character_expand(word, node->child[index]->character, &internalError);
            if(internalError != WCNoneError) {
                exit(internalError);
            }
            wc_trie_tree_traverse_recursively((node->child)[index], word, result, result_index);
        }
    }
    wc_character_shrink(word, &internalError);
    if(internalError != WCNoneError) {
        exit(internalError);
    }
}

WCTrieTreeTraverseResult * wc_trie_tree_traverse(WCTrieTree * tree, WCError * error) {
    if (tree == NULL) {
        *error = WCNullPointerError;
        return NULL;
    }
    WCTrieTreeTraverseResult * result = malloc(sizeof(WCTrieTreeTraverseResult));
    if (result == NULL) {
        *error = WCMemoryOverflowError;
        return NULL;
    }
    WCError internalError;
    result->count = tree->count;
    result->info = malloc(sizeof(WCWordInfo) * result->count);
    if (result->info == NULL) {
        *error = WCMemoryOverflowError;
        free(result);
        return NULL;
    }
    result->words = malloc(sizeof(char *) * result->count);
    if (result->words == NULL) {
        *error = WCMemoryOverflowError;
        free(result->info);
        free(result);
        return NULL;
    }
    WCWord * word = wc_word_create(WC_DEFAULT_WORD_LENGTH, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    int index = 0;
    wc_trie_tree_traverse_recursively(tree->root, word, result, &index);
    wc_word_destroy(word, &internalError);
    if (internalError != WCNoneError) {
        exit(internalError);
    }
    *error = WCNoneError;
    return result;
}

void wc_trie_tree_traverse_result_destroy(WCTrieTreeTraverseResult * result, WCError * error) {
    if (result == NULL) {
        *error = WCNullPointerError;
        return;
    }
    free(result->info);
    int index;
    for (index = 0 ; index < result->count ; ++index) {
        if ((result->words)[index] != NULL) {
            free((result->words)[index]);
        }
    }
    free(result->words);
    free(result);
    *error = WCNoneError;
}

int wc_trie_tree_get_count(WCTrieTree * tree, WCError * error) {
    if (tree == NULL) {
        *error = WCNullPointerError;
        return 0;
    }
    *error = WCNoneError;
    return tree->count;
}

int wc_trie_tree_get_unique(WCTrieTree * tree, WCError * error) {
    if (tree == NULL) {
        *error = WCNullPointerError;
        return 0;
    }
    *error = WCNoneError;
    return tree->unique;
}
